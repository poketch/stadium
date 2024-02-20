#ifndef _STADIUM_ALLOCATOR_H
#define _STADIUM_ALLOCATOR_H

#include <stdint.h>
typedef struct {
    size_t current_size;
    size_t capacity;
    uint8_t *data;
} Arena;

Arena stadium_init(size_t capacity);
void *stadium_alloc(Arena *stadium, size_t size);
void *stadium_reset(Arena *stadium);
void *stadium_free(Arena *stadium);

#endif // HEADER



// IMPLEMENTATION
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
#define STADIUM_INITIAL_CAPACITY 1024
#endif

Arena stadium_init(size_t capacity)
{   
    uint8_t *items = (uint8_t*)STADIUM_ALLOC(sizeof(uint8_t) * capacity);
    STADIUM_ASSERT(items != NULL);
    Arena a  = {.current_size = 0, .capacity = capacity, .data = items};

    return a;
} 

void *stadium_alloc(Arena *stadium, size_t size)
{
    STADIUM_ASSERT(stadium->current_size + size <= stadium->capacity && "Arena does not have enough space"); 
    uint8_t *data = &stadium->data[stadium->current_size];
    stadium->current_size += size;
    return (void *)data;
}

void *stadium_reset(Arena *stadium)
{
    stadium->current_size = 0;
    return NULL;
}

void *stadium_free(Arena *stadium)
{
    stadium->capacity = 0;
    stadium->current_size = 0;
    STADIUM_FREE(stadium->data);
    return NULL; 
}

#endif // IMPLEMENTATION