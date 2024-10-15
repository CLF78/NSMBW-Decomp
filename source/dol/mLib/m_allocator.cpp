#include <game/mLib/m_allocator_dummy_heap.hpp>
#include <game/mLib/m_allocator.hpp>
#include <game/mLib/m_heap.hpp>
#include <lib/egg/heap/eggFrmHeap.hpp>

mAllocatorDummyHeap_c *mAllocatorDummyHeap_c::m_instance;

void *dummyHeapAlloc(MEMAllocator *, u32) { return nullptr; }
void dummyHeapFree(MEMAllocator *, void *) {}

mAllocatorDummyHeap_c::mAllocatorDummyHeap_c() {
    m_instance = this;
}

mAllocatorDummyHeap_c::~mAllocatorDummyHeap_c() {}

int mAllocatorDummyHeap_c::getHeapKind() const {
    return 0;
}

void mAllocatorDummyHeap_c::initAllocator(EGG::Allocator *allocator, long param1) {
    const static MEMAllocatorFuncs funcs = { dummyHeapAlloc, dummyHeapFree };
    allocator->funcs = &funcs;
    allocator->heap = mpHeapHandle;
    allocator->heapParam1 = param1;
    allocator->heapParam2 = 0;
}

void *mAllocatorDummyHeap_c::alloc(unsigned long, long) {
    return nullptr;
}

void mAllocatorDummyHeap_c::free(void *) {}
void mAllocatorDummyHeap_c::destroy() {}

u32 mAllocatorDummyHeap_c::resizeForMBlock(void*, unsigned long) {
    return 0;
}

u32 mAllocatorDummyHeap_c::getTotalFreeSize() {
    return 0;
}

u32 mAllocatorDummyHeap_c::getAllocatableSize(long) {
    return 0;
}

bool mAllocatorDummyHeap_c::adjust() {
    return 0;
}

mAllocatorDummyHeap_c *mAllocatorDummyHeap_c::getInstance() {
    return m_instance;
}

mAllocator_c::mAllocator_c() :
    EGG::Allocator(mAllocatorDummyHeap_c::getInstance(), 4) {
}

mAllocator_c::~mAllocator_c() {
}

bool mAllocator_c::attach(EGG::Heap *heap, int align) {
    if (heap == nullptr) {
        heap = EGG::Heap::sCurrentHeap;
    }
    mAlign = align;
    mHeap = heap;
    heap->initAllocator(this, align);
    return true;
}

void *mAllocator_c::alloc(unsigned long size) {
    return EGG::Allocator::alloc(size);
}

void mAllocator_c::free(void *ptr) {
    EGG::Allocator::free(ptr);
}

void mAllocator_c::init(EGG::Heap *heap) {
    new(heap, 4) mAllocatorDummyHeap_c();
}

mHeapAllocator_c::mHeapAllocator_c() {}

mHeapAllocator_c::~mHeapAllocator_c() {
    destroyHeap();
}

bool mHeapAllocator_c::createHeap(unsigned long size, EGG::Heap *parent, const char *name, u32 align, u32 opt) {
    destroyHeap();
    EGG::FrmHeap *heap = mHeap::createFrmHeap(size, parent, name, align, opt);
    if (heap == nullptr) {
        return false;
    }
    attach(heap, align);
    return true;
}

void mHeapAllocator_c::destroyHeap() {
    EGG::Heap *dummyHeap = mAllocatorDummyHeap_c::getInstance();
    if (mHeap != dummyHeap) {
        mHeap->destroy();
        mHeap = dummyHeap;
    }
}

unsigned long mHeapAllocator_c::adjustFrmHeap() {
    EGG::Heap *dummyHeap = mAllocatorDummyHeap_c::getInstance();
    if (mHeap == dummyHeap) {
        return 0;
    }
    EGG::FrmHeap *heap = mHeap->toFrmHeap();
    return mHeap::adjustFrmHeap(heap);
}

bool mHeapAllocator_c::createHeapRestoreCurrent(unsigned long size, EGG::Heap *parent, const char *name, u32 align, u32 opt) {
    if (!createHeap(size, parent, name, align, opt)) {
        return false;
    }
    mHeap::saveCurrentHeap();
    mHeap::setCurrentHeap(mHeap);
    return true;
}

unsigned long mHeapAllocator_c::adjustFrmHeapRestoreCurrent() {
    mHeap::restoreCurrentHeap();
    EGG::FrmHeap *heap = mHeap->toFrmHeap();
    return mHeap::adjustFrmHeap(heap);
}
