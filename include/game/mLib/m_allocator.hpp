#pragma once
#include <types.h>
#include <lib/rvl/mem/MEMHeapCommon.h>
#include <lib/egg/allocator.hpp>
#include <lib/egg/heap/eggFrmHeap.hpp>

class mAllocator_c : public EGG::Allocator {
public:
    mAllocator_c();

    virtual ~mAllocator_c();
    virtual void *alloc(unsigned long);
    virtual void free(void *);

    static void init(EGG::Heap *);
    bool attach(EGG::Heap *, int);
};

class mHeapAllocator_c : public mAllocator_c {
public:
    mHeapAllocator_c();
    virtual ~mHeapAllocator_c();

    bool createHeap(unsigned long size, EGG::Heap *parent, const char *name, u32 align, u32 opt);
    void destroyHeap();
    unsigned long adjustFrmHeap();
    unsigned long adjustFrmHeapRestoreCurrent();
    bool createHeapRestoreCurrent(unsigned long size, EGG::Heap *parent, const char *name, u32 align, u32 opt);
};
