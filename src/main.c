#include <assert.h>
#include <stdio.h>

#include "vm.h"

int main(void) {
    vm_t *vm = vm_create();
    frame_t *f = vm_frame_create(vm);

    obj_t *s = obj_create_string(vm, "foo");
    vm_frame_track_object(f, s);
    vm_gc(vm);

    // Nothing should be collected because
    // we haven't freed the frame
    assert(s->data.v_str);

    vm_frame_destroy(vec_t_pop_back(vm->frames));
    vm_gc(vm);

    // Now string is freed...

    vm_destroy(vm);

    // =========================================================

    vm_t *vm1 = vm_create();
    frame_t *f1 = vm_frame_create(vm1);
    frame_t *f2 = vm_frame_create(vm1);
    frame_t *f3 = vm_frame_create(vm1);

    obj_t *s1 = obj_create_string(vm1, "f00");
    vm_frame_track_object(f1, s1);

    obj_t *s2 = obj_create_string(vm1, "bar");
    vm_frame_track_object(f2, s2);

    obj_t *s3 = obj_create_string(vm1, "baz");
    vm_frame_track_object(f3, s3);

    obj_t *i0 = obj_create_int(vm1, 10);
    obj_t *i1 = obj_create_int(vm1, 20);
    obj_t *tup = obj_create_tuple(vm1, i0, i1);
    vm_frame_track_object(f2, tup);
    vm_frame_track_object(f3, tup);

    obj_t *v = obj_create_vec(vm1, 1);
    vec_t_push_back(v->data.v_vec, tup);
    vec_t_push_back(v->data.v_vec, s1);
    vm_frame_track_object(f1, v);
    vm_frame_track_object(f3, v);

    // `v2` references itself...
    obj_t *v2 = obj_create_vec(vm1, 1);
    vec_t_push_back(v2->data.v_vec, v2);
    vm_frame_track_object(f3, v2);

    assert(vm1->objects->size == 8);

    // Only free frame `f3`
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_gc(vm1);

    // Only string `s3` and vector `v2` should be freed, because
    // tuple `tup` is still referenced in frame `f2` and vector `v1` is
    // referenced in frame `f1`...

    assert(vm1->objects->size == 6);

    // Free remaining frames
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_gc(vm1);

    // Rest of the objects should be freed...

    assert(vm1->objects->size == 0);

    vm_destroy(vm1);

    printf("TESTS PASSED\n");
    return 0;
}
