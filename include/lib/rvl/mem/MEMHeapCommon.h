#pragma once
#include <types.h>
#include <lib/rvl/mem/MEMList.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OSMutex; // TODO: fill in struct

typedef struct {
    u32 mTag;
    MEMLink mpLink;
    MEMList mpList;
    void *mpHeapStart;
    void *mpHeapEnd;
    OSMutex *mpMutex;
} MEMiHeapHeader;

struct _MEMAllocatorFuncs;
typedef struct {
    struct _MEMAllocatorFuncs *mpFuncs;
    void *mpHeap;
    u32 mHeapParam1;
    u32 mHeapParam2;
} MEMAllocator;

typedef struct _MEMAllocatorFuncs {
    void *(*alloc)(MEMAllocator *, size_t);
    void (*free)(MEMAllocator *, void *);
} MEMAllocatorFuncs;

#ifdef __cplusplus
}
#endif
