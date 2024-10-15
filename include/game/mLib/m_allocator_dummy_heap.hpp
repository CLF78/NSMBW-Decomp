#pragma once
#include <types.h>
#include <lib/egg/heap/eggHeap.hpp>
#include <lib/egg/eggDisposer.hpp>

class mAllocatorDummyHeap_c : public EGG::Heap {
public:
    mAllocatorDummyHeap_c();
    virtual ~mAllocatorDummyHeap_c();
    virtual int getHeapKind() const;
    virtual void initAllocator(EGG::Allocator *, long);
    virtual void *alloc(unsigned long, long);
    virtual void free(void *);
    virtual void destroy();
    virtual u32 resizeForMBlock(void *, unsigned long);
    virtual u32 getTotalFreeSize();
    virtual u32 getAllocatableSize(long);
    virtual bool adjust();

    static mAllocatorDummyHeap_c *getInstance();

    static mAllocatorDummyHeap_c *m_instance;
};
