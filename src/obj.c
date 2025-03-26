#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"

static obj_t *internal_obj_t_create(void) {
    obj_t *obj = malloc(sizeof(obj_t));
    if (!obj) {
        return NULL;
    }

    return obj;
}

obj_t *obj_create_int(int data) {
    obj_t *obj = internal_obj_t_create();
    if (!obj) {
        return NULL;
    }

    obj->type = INT;
    obj->data.v_int = data;

    return obj;
}

obj_t *obj_create_float(float data) {
    obj_t *obj = internal_obj_t_create();
    if (!obj) {
        return NULL;
    }

    obj->type = FLOAT;
    obj->data.v_float = data;

    return obj;
}

obj_t *obj_create_string(const char *restrict data) {
    if (!data) {
        return NULL;
    }

    obj_t *obj = internal_obj_t_create();
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

obj_t *obj_create_tuple(obj_t *x, obj_t *y) {
    if (!x || !y) {
        return NULL;
    }

    obj_t *obj = internal_obj_t_create();
    if (!obj) {
        return NULL;
    }

    obj->type = TUPLE;
    obj->data.v_tuple.x = x;
    obj->data.v_tuple.y = y;

    return obj;
}

void obj_destroy(obj_t *obj) {
    if (!obj) {
        return;
    }

    switch (obj->type) {
        case INT:
        case FLOAT:
            free(obj);
            break;
        case STRING:
            if (obj->data.v_str) {
                free(obj->data.v_str);
                obj->data.v_str = NULL;
            }

            free(obj);
            break;
        case TUPLE:
            if (obj->data.v_tuple.x) {
                obj_destroy(obj->data.v_tuple.x);
                obj->data.v_tuple.x = NULL;
            }

            if (obj->data.v_tuple.y) {
                obj_destroy(obj->data.v_tuple.y);
                obj->data.v_tuple.y = NULL;
            }
            free(obj);
            break;
        default:
            printf("UNKNOWN TYPE");
            break;
    }
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
        default:
            printf("UNKNOWN TYPE");
            break;
    }
}
