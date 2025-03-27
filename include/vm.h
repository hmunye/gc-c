#ifndef VM_H
#define VM_H

#include "obj.h"

struct vm_t {
    vec_t *frames;  /* Tracks frames of the virtual machine, representing
                       different scopes/contexts. */
    vec_t *objects; /* Currently referenced objects. */
};

typedef struct {
    vec_t *refs; /* Tracks the references to objects in the current scope. */
} frame_t;

/* Allocate and return pointer to `vm_t` instance or NULL. */
vm_t *vm_create();

/* Free the `vm` instance and all associated memory. */
void vm_destroy(vm_t *vm);

/* Appends `frame` to `vm` frames. */
void vm_frame_push(vm_t *vm, frame_t *frame);

/* Allocate and return pointer to `frame_t` instance or NULL. */
frame_t *vm_frame_create(vm_t *vm);

/* Free the `frame` instance and all associated memory. */
void vm_frame_destroy(frame_t *frame);

/* Associate `obj` to the given `vm`. */
void vm_track_object(vm_t *vm, obj_t *obj);

/* Associate `obj` to the given `frame`. */
void vm_frame_track_object(frame_t *frame, obj_t *obj);

/* Set `is_marked` field to `true` for objects directly referenced by a stack
 * frame. */
void vm_mark(vm_t *vm);

/* Identify and mark reachable objects in the VM. */
void vm_trace(vm_t *vm);

/* Marks objects as reachable (black). */
void vm_trace_blacken_objects(vec_t *grey_objs, obj_t *obj);

/* Marks objects as visited (grey). */
void vm_trace_mark_objects(vec_t *grey_objs, obj_t *obj);

/* Iterate over `vm` objects, freeing unmarked objects and resetting marked
 * objects to `false`. */
void vm_sweep(vm_t *vm);

/* Run the GC (mark(), trace(), sweep()) */
void vm_gc(vm_t *vm);

#endif  // VM_H
