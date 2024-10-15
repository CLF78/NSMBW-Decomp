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
} MEMiHeapHead;

#ifdef __cplusplus
}
#endif
