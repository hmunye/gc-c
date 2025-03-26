#include <stddef.h>
#include <stdlib.h>

#include "vec.h"

vec_t *vec_t_create() {
    vec_t *vec = malloc(sizeof(vec_t));
    if (!vec) {
        return NULL;
    }

    vec->data = calloc(INITIAL_CAPACITY, sizeof(void *));
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    vec->size = 0;
    vec->capacity = INITIAL_CAPACITY;

    return vec;
}

void *vec_t_at(const vec_t *vec, size_t pos) {
    if (!vec || !vec->data || pos >= vec->size) {
        return NULL;
    }

    return vec->data[pos];
}

void *vec_t_front(const vec_t *vec) {
    if (!vec || !vec->data || vec->size == 0) {
        return NULL;
    }

    return vec->data[0];
}

void *vec_t_back(const vec_t *vec) {
    if (!vec || !vec->data || vec->size == 0) {
        return NULL;
    }

    return vec->data[vec->size - 1];
}

vec_t *vec_t_reserve(size_t capacity) {
    vec_t *vec = malloc(sizeof(vec_t));
    if (!vec) {
        return NULL;
    }

    vec->data = calloc(capacity, sizeof(void *));
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    vec->size = 0;
    vec->capacity = capacity;

    return vec;
}

int vec_t_shrink_to_fit(vec_t *vec) {
    if (!vec) {
        return FAILURE;
    }

    /* No need to shrink vector */
    if (vec->size == vec->capacity) {
        return SUCCESS;
    }

    /* If `vec->data` is NULL, then the call is equivalent to
     * `malloc(vec->size *sizeof(void *))`. */
    void *data = realloc(vec->data, vec->size * sizeof(void *));
    if (!data) {
        return FAILURE;
    }

    vec->data = data;
    vec->capacity = vec->size;

    return SUCCESS;
}

int vec_t_clear(vec_t *vec) {
    if (!vec) {
        return FAILURE;
    }

    for (size_t i = 0; vec->data && i < vec->size; ++i) {
        free(vec->data[i]);
        vec->data[i] = NULL;
    }

    vec->size = 0;

    return SUCCESS;
}

int vec_t_insert(vec_t *vec, size_t pos, void *value) {
    if (!vec || !vec->data || !value || pos > vec->size) {
        return FAILURE;
    }

    if (vec->size == vec->capacity) {
        if (vec_t_resize(vec, vec->capacity * RESIZE_FACTOR) == FAILURE) {
            return FAILURE;
        }
    }

    /* Only case where `pos` can equal `vec->size` is when appending */
    if (pos == vec->size) {
        return vec_t_push_back(vec, value);
    }

    /* Shift elements to right, creating space for the new element at `pos`
     */
    for (size_t i = vec->size; i > pos; --i) {
        vec->data[i] = vec->data[i - 1];
    }

    vec->data[pos] = value;
    ++vec->size;

    return SUCCESS;
}

void *vec_t_remove(vec_t *vec, size_t pos) {
    if (!vec || !vec->data || pos >= vec->size) {
        return NULL;
    }

    if (pos == vec->size - 1) {
        return vec_t_pop_back(vec);
    }

    /* Save reference to `vec->data[pos]` and shift elements to left,
     * filling the gap left after removal */
    void *elem = vec->data[pos];
    for (size_t i = pos; i < vec->size - 1; ++i) {
        vec->data[i] = vec->data[i + 1];
    }

    /* Set last element to NULL to mark as removed */
    vec->data[--vec->size] = NULL;

    return elem;
}

int vec_t_push_back(vec_t *vec, void *value) {
    if (!vec || !vec->data || !value) {
        return FAILURE;
    }

    if (vec->size == vec->capacity) {
        if (vec_t_resize(vec, vec->capacity * RESIZE_FACTOR) == FAILURE) {
            return FAILURE;
        }
    }

    /* Set position after last element to `value` then increment */
    vec->data[vec->size++] = value;

    return SUCCESS;
}

void *vec_t_pop_back(vec_t *vec) {
    if (!vec || !vec->data || vec->size == 0) {
        return NULL;
    }

    /* Save reference to last element, then set to NULL */
    void *elem = vec->data[--vec->size];
    vec->data[vec->size] = NULL;

    return elem;
}

int vec_t_resize(vec_t *vec, size_t count) {
    if (!vec || count == vec->size) {
        return FAILURE;
    }

    if (count == 0) {
        vec_t_clear(vec);
        vec->capacity = 0;
        return SUCCESS;
    }

    /* If `vec->data` is NULL, then the call is equivalent to
     * `malloc(vec->size *sizeof(void *))`. */
    void *data_ptr = realloc(vec->data, count * sizeof(void *));
    if (!data_ptr) {
        return FAILURE;
    }

    vec->capacity = count;
    if (vec->size > count) {
        vec->size = count;
    }
    vec->data = data_ptr;

    return SUCCESS;
}
