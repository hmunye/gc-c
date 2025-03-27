#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

static obj_t *internal_obj_t_create(vm_t *vm) {
    obj_t *obj = calloc(1, sizeof(obj_t));
    if (!obj) {
        return NULL;
    }

    vm_track_object(vm, obj);
    /* Each new object is initially not marked  */
    obj->is_marked = false;

    return obj;
}

obj_t *obj_create_int(vm_t *vm, int data) {
    obj_t *obj = internal_obj_t_create(vm);
    if (!obj) {
        return NULL;
    }

    obj->type = INT;
    obj->data.v_int = data;

    return obj;
}

obj_t *obj_create_float(vm_t *vm, float data) {
    obj_t *obj = internal_obj_t_create(vm);
    if (!obj) {
        return NULL;
    }

    obj->type = FLOAT;
    obj->data.v_float = data;

    return obj;
}

obj_t *obj_create_string(vm_t *vm, const char *data) {
    if (!data) {
        return NULL;
    }

    obj_t *obj = internal_obj_t_create(vm);
    if (!obj) {
        return NULL;
    }

    obj->type = STRING;

    /* Allocate enough memory for string and null terminator */
    obj->data.v_str = malloc(strlen(data) + 1);
    if (!obj->data.v_str) {
        free(obj);
        return NULL;
    }

    strcpy(obj->data.v_str, data);

    return obj;
}

obj_t *obj_create_tuple(vm_t *vm, obj_t *restrict x, obj_t *restrict y) {
    if (!x || !y) {
        return NULL;
    }

    obj_t *obj = internal_obj_t_create(vm);
    if (!obj) {
        return NULL;
    }

    obj->type = TUPLE;
    obj->data.v_tuple.x = x;
    obj->data.v_tuple.y = y;

    return obj;
}

obj_t *obj_create_vec(vm_t *vm, size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    obj_t *obj = internal_obj_t_create(vm);
    if (!obj) {
        return NULL;
    }

    obj->type = VECTOR;
    if (!(obj->data.v_vec = vec_t_reserve(capacity))) {
        free(obj);
        return NULL;
    }

    return obj;
}

void obj_destroy(obj_t *obj) {
    if (!obj) {
        return;
    }

    switch (obj->type) {
        case INT:
        case FLOAT:
        case TUPLE:
            break;
        case STRING:
            /* Free dynamically allocated string before freeing the
             * object */
            free(obj->data.v_str);
            obj->data.v_str = NULL;
            break;
        case VECTOR: {
            /* Free dynamically allocated pointer array before freeing the
             * object */
            free(obj->data.v_vec->data);
            free(obj->data.v_vec);
            break;
        }
        default:
            printf("UNKNOWN TYPE");
            return;
    }

    free(obj);
}

void obj_debug_print(const obj_t *restrict obj) {
    if (!obj) {
        return;
    }

    switch (obj->type) {
        case INT:
            printf("%d", obj->data.v_int);
            break;
        case FLOAT:
            printf("%.1f", obj->data.v_float);
            break;
        case STRING:
            printf("\"%s\"", obj->data.v_str ? obj->data.v_str : "null");
            break;
        case TUPLE:
            printf("(");
            obj->data.v_tuple.x ? obj_debug_print(obj->data.v_tuple.x)
                                : (void)printf("null");

            printf(", ");

            obj->data.v_tuple.y ? obj_debug_print(obj->data.v_tuple.y)
                                : (void)printf("null");
            printf(")");

            break;
        case VECTOR: {
            vec_t *vec = obj->data.v_vec;

            if (!vec) {
                printf("LEN: {0} []");
                break;
            }

            printf("LEN: {%zu} [", vec->size);

            for (size_t i = 0; i < vec->size; ++i) {
                obj_t *elem = (obj_t *)(vec->data[i]);
                obj_debug_print(elem);

                if (i < vec->size - 1) {
                    printf(", ");
                }
            }

            printf("]");
            break;
        }
        default:
            printf("UNKNOWN TYPE");
            break;
    }
}
