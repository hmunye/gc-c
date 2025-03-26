#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"

int main(void) {
    obj_t *int_obj = obj_create_int(30);
    assert(int_obj);
    assert(int_obj->type == INT);
    assert(int_obj->data.v_int == 30);

    obj_t *float_obj = obj_create_float(48.6f);
    assert(float_obj);
    assert(float_obj->type == FLOAT);
    assert(float_obj->data.v_float == 48.6f);

    obj_t *string_obj = obj_create_string("foo");
    assert(string_obj);
    assert(string_obj->type == STRING);
    assert(strcmp(string_obj->data.v_str, "foo") == 0);

    free(int_obj);
    free(float_obj);
    free(string_obj->data.v_str);
    free(string_obj);

    printf("TESTS PASSED\n");
    return 0;
}
