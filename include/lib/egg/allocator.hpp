#pragma once
#include <types.h>
#include <lib/egg/heap/eggHeap.hpp>

typedef void* (*AllocFunc)(struct MEMAllocator* allocator, u32 size);
typedef void (*FreeFunc)(struct MEMAllocator* allocator, void *block);

struct MEMAllocatorFuncs {
    AllocFunc allocFunc;
    FreeFunc freeFunc;
};

struct MEMAllocator {
    const MEMAllocatorFuncs* funcs;
    struct MEMiHeapHead* heap;
    u32 heapParam1;
    u32 heapParam2;
};


namespace EGG {
    class Allocator : public MEMAllocator {
    public:
        Allocator(Heap *h, long l);

        virtual ~Allocator();
        virtual void *alloc(unsigned long);
        virtual void free(void *);

    protected:
        EGG::Heap *mHeap;
        int mAlign;
    };
}