#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"
#include "vec.h"

int main(void) {
    obj_t *int_obj = obj_create_int(30);
    assert(int_obj);
    assert(int_obj->type == INT);
    assert(int_obj->data.v_int == 30);

    obj_debug_print(int_obj);
    printf("\n");

    obj_t *float_obj = obj_create_float(48.6f);
    assert(float_obj);
    assert(float_obj->type == FLOAT);
    assert(float_obj->data.v_float == 48.6f);

    obj_debug_print(float_obj);
    printf("\n");

    obj_t *string_obj = obj_create_string("foo");
    assert(string_obj);
    assert(string_obj->type == STRING);
    assert(strcmp(string_obj->data.v_str, "foo") == 0);

    obj_debug_print(string_obj);
    printf("\n");

    obj_t *tuple = obj_create_tuple(int_obj, float_obj);
    assert(tuple);
    assert(tuple->type == TUPLE);
    assert(tuple->data.v_tuple.x == int_obj);
    assert(tuple->data.v_tuple.y == float_obj);

    obj_debug_print(tuple);
    printf("\n");

    obj_t *outer_tuple = obj_create_tuple(string_obj, tuple);
    assert(outer_tuple);
    assert(outer_tuple->type == TUPLE);
    assert(outer_tuple->data.v_tuple.x == string_obj);
    assert(outer_tuple->data.v_tuple.y == tuple);

    obj_debug_print(outer_tuple);
    printf("\n");

    obj_t *vec_obj = obj_create_vec(4);
    vec_t *vec = vec_obj->data.v_vec;
    assert(vec_obj && vec);
    assert(vec_obj->type == VECTOR);
    assert(vec->size == 0);
    assert(vec->capacity == 4);

    obj_debug_print(vec_obj);
    printf("\n");

    assert(vec_t_push_back(vec, (void *)int_obj) == SUCCESS);
    assert(vec_t_push_back(vec, (void *)float_obj) == SUCCESS);
    assert(vec_t_push_back(vec, (void *)string_obj) == SUCCESS);
    assert(vec_t_push_back(vec, (void *)tuple) == SUCCESS);
    assert(vec_t_push_back(vec, (void *)outer_tuple) == SUCCESS);

    assert(vec->size == 5);
    assert(vec->capacity == 8);

    obj_debug_print(vec_obj);
    printf("\n");

    obj_destroy(vec_obj);

    printf("TESTS PASSED\n");
    return 0;
}
