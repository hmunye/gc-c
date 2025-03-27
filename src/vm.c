#include <stdlib.h>

#include "vm.h"

vm_t *vm_create() {
    vm_t *vm = malloc(sizeof(vm_t));
    if (!vm) {
        return NULL;
    }

    /* Uses INITIAL_CAPACITY for vector */
    vm->frames = vec_t_create();
    if (!vm->frames) {
        free(vm);
        return NULL;
    }

    /* Uses INITIAL_CAPACITY for vector */
    vm->objects = vec_t_create();
    if (!vm->objects) {
        free(vm->frames);
        free(vm);
        return NULL;
    }

    return vm;
}

void vm_destroy(vm_t *vm) {
    if (!vm) {
        return;
    }

    for (size_t i = 0; i < vm->frames->size; ++i) {
        vm_frame_destroy(vm->frames->data[i]);
    }
    free(vm->frames->data);
    free(vm->frames);

    for (size_t i = 0; i < vm->objects->size; ++i) {
        obj_destroy(vm->objects->data[i]);
    }
    free(vm->objects->data);
    free(vm->objects);

    free(vm);
}

void vm_frame_push(vm_t *vm, frame_t *frame) {
    if (!vm || !frame) {
        return;
    }

    vec_t_push_back(vm->frames, (void *)frame);
}

frame_t *vm_frame_create(vm_t *vm) {
    if (!vm) {
        return NULL;
    }

    frame_t *frame = malloc(sizeof(frame_t));

    /* Uses INITIAL_CAPACITY for vector */
    frame->refs = vec_t_create();
    if (!frame->refs) {
        free(frame);
        return NULL;
    }

    vm_frame_push(vm, frame);

    return frame;
}

void vm_frame_destroy(frame_t *frame) {
    if (!frame) {
        return;
    }

    free(frame->refs->data);
    free(frame->refs);
    free(frame);
}

void vm_track_object(vm_t *vm, obj_t *obj) {
    if (!vm || !obj) {
        return;
    }

    vec_t_push_back(vm->objects, (void *)obj);
}

void vm_frame_track_object(frame_t *frame, obj_t *obj) {
    if (!frame || !obj) {
        return;
    }

    vec_t_push_back(frame->refs, (void *)obj);
}

void vm_mark(vm_t *vm) {
    if (!vm) {
        return;
    }

    for (size_t i = 0; i < vm->frames->size; ++i) {
        frame_t *frame = vm->frames->data[i];
        for (size_t j = 0; j < frame->refs->size; ++j) {
            /* Need to cast `void *` to `obj_t *` */
            ((obj_t *)(frame->refs->data)[j])->is_marked = true;
        }
    }
}

void vm_trace(vm_t *vm) {
    if (!vm || !vm->objects) {
        return;
    }

    /* Uses INITIAL_CAPACITY for vector */
    vec_t *grey_objs = vec_t_create();
    if (!grey_objs) {
        return;
    }

    for (size_t i = 0; i < vm->objects->size; ++i) {
        obj_t *obj = vm->objects->data[i];
        if (obj && obj->is_marked) {
            vec_t_push_back(grey_objs, (void *)obj);
        }
    }

    while (grey_objs->size > 0) {
        obj_t *obj = vec_t_pop_back(grey_objs);
        if (obj) {
            vm_trace_blacken_objects(grey_objs, obj);
        }
    }

    free(grey_objs->data);
    free(grey_objs);
}

void vm_trace_blacken_objects(vec_t *grey_objs, obj_t *obj) {
    switch (obj->type) {
        /* These are not collection types and don't contain references to other
         * objects */
        case INT:
        case FLOAT:
        case STRING:
            break;
        case TUPLE:
            vm_trace_mark_objects(grey_objs, obj->data.v_tuple.x);
            vm_trace_mark_objects(grey_objs, obj->data.v_tuple.y);
            break;
        case VECTOR: {
            vec_t *vec = obj->data.v_vec;
            if (!vec->data) {
                break;
            }

            for (size_t i = 0; i < vec->size; ++i) {
                vm_trace_mark_objects(grey_objs, vec->data[i]);
            }

            break;
        }
    }
}

void vm_trace_mark_objects(vec_t *grey_objs, obj_t *obj) {
    if (!grey_objs || !obj || obj->is_marked == true) {
        return;
    }

    obj->is_marked = true;

    vec_t_push_back(grey_objs, (void *)obj);
}

void vm_sweep(vm_t *vm) {
    if (!vm || !vm->objects) {
        return;
    }

    for (size_t i = 0; i < vm->objects->size; ++i) {
        obj_t *obj = vm->objects->data[i];
        if (obj && obj->is_marked) {
            obj->is_marked = false;
        } else {
            obj_destroy(obj);
            vm->objects->data[i] = NULL;
        }
    }

    size_t count = 0;

    /* Compact non-NULL pointers, shifting them to the right */
    for (size_t i = 0; i < vm->objects->size; ++i) {
        if (vm->objects->data[i]) {
            vm->objects->data[count++] = vm->objects->data[i];
        }
    }

    vm->objects->size = count;
}

void vm_gc(vm_t *vm) {
    vm_mark(vm);
    vm_trace(vm);
    vm_sweep(vm);
}
