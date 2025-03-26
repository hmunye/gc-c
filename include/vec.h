#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 10
#endif

#ifndef RESIZE_FACTOR
#define RESIZE_FACTOR 2
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE -1
#endif

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} vec_t;

/* Returns a pointer to new `vec_t` vector or NULL, initialized with default
 * INITIAL_CAPACITY. */
vec_t *vec_t_create();

/* Returns a pointer to the element at the location `pos` or NULL, with
 * bounds checking. */
void *vec_t_at(const vec_t *vec, size_t pos);

/* Returns a pointer to the first element in the vector or NULL. */
void *vec_t_front(const vec_t *vec);

/* Returns a pointer to the last element in the vector or NULL. */
void *vec_t_back(const vec_t *vec);

/* Returns a pointer to new `vec_t` vector or NULL, with the specified initial
 * capacity. */
vec_t *vec_t_reserve(size_t capacity);

/* Reduces vector's capacity to match current size, returning SUCCESS (0) or
 * FAILURE (-1). */
int vec_t_shrink_to_fit(vec_t *vec);

/* Frees all elements from the vector, then sets `size` to 0, returning SUCCESS
 * (0) or FAILURE (-1). */
int vec_t_clear(vec_t *vec);

/* Inserts the given element at the specified location `pos` in the vector,
 * returning SUCCESS (0) or FAILURE (-1). */
int vec_t_insert(vec_t *vec, size_t pos, void *value);

/* Removes the element at the specified location `pos` from the vector,
 * returning pointer to removed element, or NULL. Caller is responsible for
 * freeing the memory of the removed element. */
void *vec_t_remove(vec_t *vec, size_t pos);

/* Appends the given element `value` to the end of the vector, returning SUCCESS
 * (0) or FAILURE (-1). */
int vec_t_push_back(vec_t *vec, void *value);

/* Removes the last element of the vector, returning pointer to removed element
 * or NULL. Caller is responsible for freeing the memory of the removed
 * element. */
void *vec_t_pop_back(vec_t *vec);

/* Resizes the vector to contain `count` element. If `count == size` nothing is
 * done. If `size > count`, the vector is reduced to it's first `count`
 * elements. If `size < count`, additional elements are appended. Returns
 * SUCCESS (0) or FAILURE (-1). */
int vec_t_resize(vec_t *vec, size_t count);

#endif  // VEC_H
