#include <game/mLib/m_3d.hpp>

#include <game/mLib/m_heap.hpp>

unsigned long m3d::anmTexSrt_c::child_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt anmClr, bool b) {
    unsigned long cost = 0;
    nw4r::g3d::AnmObjTexSrtRes::Construct(nullptr, &cost, anmClr, mdl, false);
    if (b) {
        cost = (mHeap::frmHeapCost(cost, 0x20) + 0x1f) & 0xffffffe0;
    }
    return cost;
}

bool m3d::anmTexSrt_c::child_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt anmClr, mAllocator_c *allocator, unsigned long *idk) {
    if (allocator == nullptr) {
        allocator = m3d::internal::l_allocator_p;
    }
    unsigned long tmp;
    if (idk == nullptr) {
        idk = &tmp;
    }
    *idk = heapCost(mdl, anmClr, false);
    if (!createAllocator(allocator, idk)) {
        return false;
    }
    mpObj = nw4r::g3d::AnmObjTexSrtRes::Construct(&mAllocator, nullptr, anmClr, mdl, false);
    if (mpObj == nullptr) {
        return false;
    }
    if (!mpObj->Bind(mdl)) {
        remove();
        return false;
    }
    setFrmCtrlDefault(anmClr, FOUR);
    return true;
}

void m3d::anmTexSrt_c::child_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmTexSrt anmClr, m3d::playMode_e playMode) {
    releaseAnm();
    mpObj = nw4r::g3d::AnmObjTexSrtRes::Construct(&mAllocator, nullptr, anmClr, mdl.getResMdl(), false);
    mpObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(anmClr, playMode);
}

void m3d::anmTexSrt_c::child_c::releaseAnm() {
    if (mpObj == nullptr) {
        return;
    }
    mpObj->Release();
    mpHeap->free(3);
    mpObj = nullptr;
}

void m3d::anmTexSrt_c::child_c::setFrmCtrlDefault(nw4r::g3d::ResAnmTexSrt &anmClr, m3d::playMode_e playMode) {
  if (playMode == 4) {
    playMode = (playMode_e) (anmClr.p->x28 == 0);
  }
  fanm_c::set(anmClr.getX20(), playMode, 1.0, -1.0);
}

unsigned long m3d::anmTexSrt_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt anmClr, long l, bool b) {
    unsigned long cost = 0;
    nw4r::g3d::AnmObjTexSrtOverride::Construct(nullptr, &cost, mdl, l);
    cost += align32Bit(l * sizeof(child_c));
    cost += align32Bit(child_c::heapCost(mdl, anmClr, true)) * l;
    if (b) {
        cost = align32Bit(mHeap::frmHeapCost(cost, 0x20));
    }
    return cost;
}

bool m3d::anmTexSrt_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt anmClr, mAllocator_c *allocator, unsigned long *idk, long l) {
    if (allocator == nullptr) {
        allocator = m3d::internal::l_allocator_p;
    }
    unsigned long tmp = 0;
    if (idk == nullptr) {
        idk = &tmp;
    }
    *idk = heapCost(mdl, anmClr, l, false);
    if (!createAllocator(allocator, idk)) {
        return false;
    }
    mpObj = nw4r::g3d::AnmObjTexSrtOverride::Construct(&mAllocator, nullptr, mdl, l);
    if (mpObj == nullptr) {
        return false;
    }
    children = (m3d::anmTexSrt_c::child_c *) MEMAllocFromAllocator(&mAllocator, align32Bit(l * sizeof(child_c)));
    if (children == nullptr) {
        remove();
        return false;
    }

    nw4r::g3d::AnmObjTexSrtOverride *matClr = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpObj);

    child_c *child = children;
    for (int i = 0; i < l; i++) {
        new(child) child_c();
        if (!child->create(mdl, anmClr, &mAllocator, nullptr)) {
            mHeap::destroyFrmHeap(mpHeap);
            EGG::Heap::free(mpHeap, nullptr);
            return false;
        }
        if (i == 0) {
            nw4r::g3d::AnmObjTexSrtRes *clrRes = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtRes>(child->mpObj);
            matClr->Attach(i, clrRes);
        } else {
            child->releaseAnm();
        }
        child++;
    }
    return true;
}

m3d::anmTexSrt_c::~anmTexSrt_c() {
    anmTexSrt_c::remove();
}

void m3d::anmTexSrt_c::remove() {
    nw4r::g3d::AnmObjTexSrtOverride *matClr = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpObj);
    if (matClr != nullptr && children != nullptr) {
        int count = matClr->getCount();
        for (int i = 0; i < count; i++) {
            children[i].remove();
        }
        children = nullptr;
    }
    banm_c::remove();
}

void m3d::anmTexSrt_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmTexSrt clr, long i, m3d::playMode_e playMode) {
    nw4r::g3d::AnmObjTexSrtOverride *matClr = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpObj);
    matClr->Detach(i);
    children[i].setAnm(mdl, clr, playMode);
    nw4r::g3d::AnmObjTexSrtRes *clrRes = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtRes>(children[i].mpObj);
    matClr->Attach(i, clrRes);
}

void m3d::anmTexSrt_c::releaseAnm(long i) {
    nw4r::g3d::AnmObjTexSrtOverride *matClr = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpObj);
    matClr->Detach(i);
    children[i].releaseAnm();
}

void m3d::anmTexSrt_c::play() {
    nw4r::g3d::AnmObjTexSrtOverride *matClr = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpObj);
    int count = matClr->getCount();
    for (int i = 0; i < count; i++) {
        play(i);
    }
}

void m3d::anmTexSrt_c::play(long i) {
    if (children[i].IsBound()) {
        children[i].play();
    }
}

float m3d::anmTexSrt_c::getFrame(long i) const {
    return children[i].getFrame();
}

void m3d::anmTexSrt_c::setFrame(float frame, long i) {
    children[i].setFrame(frame);
}

float m3d::anmTexSrt_c::getRate(long i) const {
    return children[i].getRate();
}

void m3d::anmTexSrt_c::setRate(float rate, long i) {
    children[i].setRate(rate);
}

bool m3d::anmTexSrt_c::isStop(long i) const {
    return children[i].isStop();
}

void m3d::anmTexSrt_c::setPlayMode(m3d::playMode_e mode, long i) {
    children[i].mPlayMode = mode;
}

float m3d::anmTexSrt_c::getFrameMax(long i) const {
    return children[i].mF1;
}

void m3d::anmTexSrt_c::setFrameStart(float frame, long i) {
    children[i].mF2 = frame;
}

int m3d::anmTexSrt_c::getType() const {
    return 4;
}
