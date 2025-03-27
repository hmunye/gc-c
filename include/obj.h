#ifndef OBJ_H
#define OBJ_H

#include <stdbool.h>

#include "vec.h"

typedef struct vm_t vm_t;
typedef struct obj_t obj_t;

typedef enum {
    INT,
    FLOAT,
    STRING,
    TUPLE,
    VECTOR,
} obj_type_t;

typedef struct {
    obj_t *x;
    obj_t *y;
} tuple_t;

typedef union {
    int v_int;
    float v_float;
    char *v_str;
    tuple_t v_tuple;
    vec_t *v_vec;
} obj_data_t;

struct obj_t {
    obj_data_t data;
    obj_type_t type;
    bool is_marked;
};

/* Generates function prototypes for basic object types. */
#define OBJ_CREATE_FUNC(type)                                           \
    /* Allocate and return a new `type` `obj_t`, or NULL on failure. */ \
    obj_t *obj_create_##type(vm_t *vm, type data);

OBJ_CREATE_FUNC(int)
OBJ_CREATE_FUNC(float)

/* Allocate and return a new STRING `obj_t`, or NULL on failure. */
obj_t *obj_create_string(vm_t *vm, const char *data);

/* Allocate and return a new TUPLE `obj_t`, or NULL on failure. */
obj_t *obj_create_tuple(vm_t *vm, obj_t *restrict x, obj_t *restrict y);

/* Allocate and return a new VECTOR `obj_t`, or NULL on failure. */
obj_t *obj_create_vec(vm_t *vm, size_t capacity);

/* Free the `obj_t` instance and all associated memory. */
void obj_destroy(obj_t *obj);

/* Print the contents of the given `obj_t` to stdout. */
void obj_debug_print(const obj_t *obj);

#endif  // OBJ_H
