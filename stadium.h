#ifndef _STADIUM_ALLOCATOR_H
#define _STADIUM_ALLOCATOR_H

#include <stdint.h>
typedef struct Region
{
    struct Region *next;
    size_t count;    // dynamic array counter. i.e. number of items of sizeof(uint8_t) allocated
    size_t capacity; // capacity in number of chunks
    uint8_t *data;
} Region;

Region *new_region(size_t capacity);
void free_region(Region *r);
void print_region(Region *r);

typedef struct
{
    Region *begin;
    Region *end;
} Stadium;

Stadium stadium_init(size_t capacity);
void *stadium_alloc(Stadium *stadium, size_t size);
void stadium_reset(Stadium *stadium);
void stadium_free(Stadium *stadium);
void dump_stadia(Stadium *stadium);

#endif // HEADER

// IMPLEMENTATION
// #define STADIUM_ALLOCATOR_IMPLEMENTATION
#ifdef STADIUM_ALLOCATOR_IMPLEMENTATION

#ifndef STADIUM_NO_STDLIB
#include <stdlib.h>
#endif

#ifndef STADIUM_ALLOC
#define STADIUM_ALLOC malloc
#endif

#ifndef STADIUM_FREE
#define STADIUM_FREE free
#endif

#ifndef STADIUM_ASSERT
#include <assert.h>
#define STADIUM_ASSERT assert
#endif

#ifndef STADIUM_INITIAL_CAPACTIY
#define STADIUM_INITIAL_CAPACITY (sizeof(uint8_t *) * 0)
#endif

Region *new_region(size_t capacity)
{
    size_t size = sizeof(Region) + capacity * sizeof(uint8_t *);
    Region *r = STADIUM_ALLOC(size);
    STADIUM_ASSERT(r != NULL && "LMFAO L + Ratio + Buy more RAM.");
    r->next = NULL;
    r->count = 0;
    r->capacity = capacity;
    return r;
}

void free_region(Region *r)
{
    STADIUM_FREE(r);
    return;
}

Stadium stadium_init(size_t size)
{
    Stadium s = {0};
    size_t capacity = STADIUM_INITIAL_CAPACITY;
    if (capacity < size)
        capacity = size;
    s.end = new_region(capacity);
    s.begin = s.end;
    return s;
}

void *stadium_alloc(Stadium *stadium, size_t size)
{
    STADIUM_ASSERT(stadium->begin->count + size <= stadium->begin->capacity && "Arena does not have enough space");

    void *data = &stadium->end->data[stadium->end->count];
    stadium->begin->count += size;
    return data;
}

void stadium_reset(Stadium *stadium)
{
    stadium->end->count = 0;
    return;
}

void stadium_free(Stadium *stadium)
{
    stadium->end->capacity = 0;
    stadium->end->count = 0;
    STADIUM_FREE(stadium->end->data);
    return;
}

void dump_stadia(Stadium *stadium)
{
    size_t count = 1;
    for (Region *r = stadium->begin; r != NULL; r = r->next)
    {
        printf("Region[%d]:", count++);
        print_region(r);
    }
}

void print_region(Region *r)
{
    printf("    Number of bytes currently allocated: %u\n", r->count);
    printf("    Number of space available: %u\n", r->capacity);
    printf("    Start of Region pointer location: %p\n", &r->data);
}

#endif // IMPLEMENTATION