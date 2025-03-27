#include <assert.h>
#include <stdio.h>

#include "vm.h"

int main(void) {
    vm_t *vm = vm_create();
    frame_t *f = vm_frame_create(vm);

    obj_t *s = obj_create_string(vm, "foo");
    vm_frame_track_object(f, s);

    // Nothing should be collected because
    // we haven't freed the frame
    assert(s->data.v_str);

    vm_gc(vm);

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

    assert(vm1->objects->size == 6);

    // Only free frame `f3`
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_gc(vm1);

    // Only string `s3` should be freed, because tuple `tup` is still
    // referenced in frame `f2`...

    // Free remaining frames
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_frame_destroy(vec_t_pop_back(vm1->frames));
    vm_gc(vm1);

    // Rest of objects should be freed...

    assert(vm1->objects->size == 0);

    vm_destroy(vm1);

    printf("TESTS PASSED\n");
    return 0;
}
