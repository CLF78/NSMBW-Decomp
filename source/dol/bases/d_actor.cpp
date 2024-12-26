#include <game/bases/d_actor.hpp>
#include <game/bases/d_attention.hpp>
#include <game/bases/d_a_player.hpp>
#include <game/bases/d_a_player_manager.hpp>
#include <game/bases/d_a_yoshi.hpp>
#include <game/bases/d_audio.hpp>
#include <game/bases/d_bg.hpp>
#include <game/bases/d_cd.hpp>
#include <game/bases/d_effactor_mng.hpp>
#include <game/bases/d_fukidashiManager.hpp>
#include <game/bases/d_info.hpp>
#include <game/bases/d_game_com.hpp>
#include <game/bases/d_multi_mng.hpp>
#include <game/bases/d_s_stage.hpp>
#include <game/bases/d_score_mng.hpp>
#include <game/mLib/m_ef.hpp>
#include <lib/nw4r/g3d/scn_mdl.hpp>
#include <constants/sjis_constants.h>
#include <constants/sound_list.h>

bool dActor_c::mExecStopReq;
bool dActor_c::mDrawStopReq;
bool dActor_c::mExecStop;
bool dActor_c::mDrawStop;
dActor_c::searchNearPlayerFunc dActor_c::mSearchNearPlayerFunc;
dActor_c::getTrgToSrcDirFunc dActor_c::mGetTrgToSrcDirFunc;

const mVec2_c dActor_c::smc_FUKIDASHI_RANGE = mVec2_c(9.0f, 12.0f);

u8 dActor_c::m_tmpCtLayerNo;

const float dActor_c::smc_CULL_XLIMIT = 80.0f;
const float dActor_c::smc_CULL_YLIMIT = 256.0f;
const float dActor_c::smc_CULL_AREA_XLIMIT = 128.0f;
const float dActor_c::smc_CULL_AREA_YLIMIT = 128.0f;

const u8 l_Ami_Line[] = { 1, 2, 0, 0, 0, 0, 0, 0 };
const float l_Ami_Zpos[] = { 1500.0f, -2500.0f };

u32 dActor_c::m_relatedCreate1;
u8 dActor_c::m_relatedCreate2[8];
u32 dActor_c::m_relatedCreate3;
u32 dActor_c::m_relatedCreate4;
u8 dActor_c::m_relatedCreate5;

dActor_c::dActor_c() : _0(0), mCarryingPlayerNo(-1), _14(0), _18(1.0f) {

    visibleAreaSize.reset();
    visibleAreaOffset.reset();
    mCullLimit.reset();
    mCullAreaLimit.reset();

    mSomeBitField = nullptr;
    _228 = nullptr;
    eaterActorID = 0;
    _245 = 2;
    _258 = 0;

    mPlayerNo = -1;
    mWasSquished = false;

    setKind(0);

    mStopMask = 1;

    initCullLimits();
    mDestroyBound = BoundingBox(256.0f, 256.0f, 80.0f, 80.0f);

    dCdFile_c *course = dCd_c::m_instance->getFileP(dScStage_c::m_instance->currCourse);
    mAreaNo = course->getAreaNo(&mPos);

    mBc.rcRef = &mRc;
    mRc.set(this, 1);
    mRc.bcRef = &mBc;

    mBgCollFlags = 0;
    mComboCount = 0;

    mLayer = m_tmpCtLayerNo;
    mCc.mLayer = mLayer;
    mBc.mLayer = mLayer;
    mRc.mLayer = mLayer;

    _254 = 0;
    mBlockHitRequested = false;
}

dActor_c::~dActor_c() {
    mCc.release();
}


int dActor_c::preCreate() {
    return dBaseActor_c::preCreate() != NOT_READY;
}

void dActor_c::postCreate(fBase_c::MAIN_STATE_e status) {
    dBaseActor_c::postCreate(status);
}

int dActor_c::preDelete() {
    if (dBaseActor_c::preDelete() == NOT_READY) {
        return NOT_READY;
    }
    clrComboCnt();
    return SUCCEEDED;
}

void dActor_c::postDelete(fBase_c::MAIN_STATE_e status) {
    dBaseActor_c::postDelete(status);
}


int dActor_c::preExecute() {
    if (dBaseActor_c::preExecute() == NOT_READY) {
        return NOT_READY;
    }
    if (dGameCom::isGameStop(0xffffffff)) {
        return NOT_READY;
    }
    if (mExecStop & mStopMask) {
        return NOT_READY;
    }
    mRc.clrLink();
    if (mBlockHitRequested == true) {
        mBlockHitRequested = false;
        block_hit_init();
    }
    return SUCCEEDED;
}

void dActor_c::postExecute(fBase_c::MAIN_STATE_e status) {
    if (status == SUCCESS) {
        if (!(mActorProperties & 0x400)) {
            mPos.x = dScStage_c::getLoopPosX(mPos.x);
        }
        mCc.clear();
        if (!mDeleteRequested) {
            mBc.checkLink();
            mRc.chkLink();
        }
        if (mActorProperties & 0x8) {
            dAttention_c::mspInstance->entry(mUniqueID);
        }
        _1e8 = 0.0f;
        _1ec = 0.0f;
    }
    dBaseActor_c::postExecute(status);
}

int dActor_c::preDraw() {
    if (dBaseActor_c::preDraw() == NOT_READY || (mDrawStop & getKindMask()) != 0) {
        return NOT_READY;
    }
    if (mNoDrawIf2 == 2) {
        return NOT_READY;
    }
    if ((mActorProperties & 2) != 0 && ActorDrawCullCheck() != 0) {
        return NOT_READY;
    }
    return SUCCEEDED;
}

void dActor_c::postDraw(fBase_c::MAIN_STATE_e status) {
    dBaseActor_c::postDraw(status);
}

const char *dActor_c::getKindString() const {
    return D_ACTOR_KIND_STRING;
}

void dActor_c::setTmpCtData(u8 layer) {
    m_tmpCtLayerNo = layer;
}

dActor_c *dActor_c::construct(ProfileName profName, unsigned long param, const mVec3_c *position, const mAng3_c *rotation, u8 layer) {
    setTmpCtData(layer);
    return (dActor_c *) dBaseActor_c::construct(profName, param, position, rotation);
}

dActor_c *dActor_c::construct(ProfileName profName, dBase_c *parent, unsigned long param, const mVec3_c *position, const mAng3_c *rotation, u8 layer) {
    setTmpCtData(layer);
    return (dActor_c *) dBaseActor_c::construct(profName, parent, param, position, rotation);
}

void dActor_c::setKind(u8 kind) {
    mKind = kind;
    mpKindString = getKindString();
}

void dActor_c::setSearchNearPlayerFunc(int loopType) {
    static const searchNearPlayerFunc searchFunc[3] = {
        searchNearPlayerNormal,
        searchNearPlayerLoop,
        searchNearPlayerLoop
    };
    mSearchNearPlayerFunc = searchFunc[loopType];
}

dAcPy_c *dActor_c::searchNearPlayer(mVec2_c &pos) {
    mVec3_c centerPos = getCenterPos();
    return searchNearPlayer_Main(pos, mVec2_c(centerPos.x, centerPos.y));
}

dAcPy_c *dActor_c::searchNearPlayer_Main(mVec2_c &delta, const mVec2_c &selfPos) {
    return mSearchNearPlayerFunc(delta, selfPos);
}

dAcPy_c *dActor_c::searchNearPlayerNormal(mVec2_c &delta, const mVec2_c &selfPos) {
    dAcPy_c *closestPlayer = nullptr;
    float closest = 1e9;
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = daPyMng_c::getPlayer(i);
        if (player == nullptr || !daPyMng_c::checkPlayer(i)) {
            continue;
        }
        float xDiff = player->mPos.x + player->mCenterOffs.x - selfPos.x;
        float yDiff = player->mPos.y + player->mCenterOffs.y - selfPos.y;
        float dist = xDiff * xDiff + yDiff * yDiff;
        if (closest > dist) {
            delta.set(xDiff, yDiff);
            closestPlayer = player;
            closest = dist;
        }
    }
    return closestPlayer;
}

dAcPy_c *dActor_c::searchNearPlayerLoop(mVec2_c &delta, const mVec2_c &selfPos) {
    dAcPy_c *closestPlayer = nullptr;

    float bgSmth = dBg_c::m_bg_p->mLoopOffset;

    mVec2_c loopSelfPos;
    loopSelfPos.x = dScStage_c::getLoopPosX(selfPos.x);
    loopSelfPos.y = selfPos.y;
    float closestDist = 1e9;

    dAcPy_c *player;
    for (int i = 0; i < 4; i++) {
        if ((player = daPyMng_c::getPlayer(i)) == nullptr || !daPyMng_c::checkPlayer(i)) {
            continue;
        }

        mVec2_c loopPlayerPos;
        loopPlayerPos.x = dScStage_c::getLoopPosX(player->getCenterX());
        loopPlayerPos.y = player->getCenterY();

        mVec2_c trueDelta;
        trueDelta.x = loopPlayerPos.x - loopSelfPos.x;
        if (trueDelta.x < 0.0f) {
            if (trueDelta.x < -bgSmth / 2) {
                trueDelta.x += bgSmth;
            }
        } else {
            if (trueDelta.x > bgSmth / 2) {
                trueDelta.x -= bgSmth;
            }
        }
        trueDelta.y = loopPlayerPos.y - loopSelfPos.y;

        float dist = trueDelta.x * trueDelta.x + trueDelta.y * trueDelta.y;
        if (closestDist > dist) {
            delta.set(trueDelta.x, trueDelta.y);
            closestPlayer = player;
            closestDist = dist;
        }
    }
    return closestPlayer;
}

void dActor_c::setGetTrgToSrcDirFunc(int loopType) {
    static const getTrgToSrcDirFunc getdirFunc[3] = {
        getTrgToSrcDirNormal,
        getTrgToSrcDirLoop,
        getTrgToSrcDirLoop
    };
    mGetTrgToSrcDirFunc = getdirFunc[loopType];
}


bool dActor_c::getTrgToSrcDir_Main(float f1, float f2) {
    return mGetTrgToSrcDirFunc(f1, f2);
}

bool dActor_c::getTrgToSrcDirNormal(float f1, float f2) {
    return f1 < f2;
}

bool dActor_c::getTrgToSrcDirLoop(float f1, float f2) {
    float loopPos1 = dScStage_c::getLoopPosX(f1);
    float loopPos2 = dScStage_c::getLoopPosX(f2);
    float diff = loopPos1 - loopPos2;
    float bgLoop = dBg_c::m_bg_p->mLoopOffset / 2;
    if (diff < 0.0f) {
        return !(diff < -bgLoop);
    } else {
        return diff > bgLoop;
    }
}

void dActor_c::changePosAngle(mVec3_c *pos, mAng3_c *ang, int unk) {
    dScStage_c::changePos(pos);
}

void dActor_c::setChangePosAngleFunc(int loopType) {
    dScStage_c::setChangePosFunc(loopType);
}

void dActor_c::setLoopFunc(int loopType) {
    setChangePosAngleFunc(loopType);
    setSearchNearPlayerFunc(loopType);
    setGetTrgToSrcDirFunc(loopType);
}

bool dActor_c::checkAreaNo() {
    for (int i = 0; i < 4; i++) {
        dActor_c *player = daPyMng_c::getPlayer(i);
        if (player != nullptr && player->mAreaNo == mAreaNo) {
            return true;
        }
    }
    return false;
}

void dActor_c::setSoftLight_Player(m3d::bmdl_c &mdl) {
    dGameCom::SetSoftLight_Player(mdl, 0);
}

void dActor_c::setSoftLight_Enemy(m3d::bmdl_c &mdl) {
    dGameCom::SetSoftLight_Enemy(mdl, 0);
}

void dActor_c::setSoftLight_Map(m3d::bmdl_c &mdl) {
    dGameCom::SetSoftLight_Map(mdl, 0);
}

void dActor_c::setSoftLight_MapObj(m3d::bmdl_c &mdl) {
    dGameCom::SetSoftLight_MapObj(mdl, 0);
}

void dActor_c::setSoftLight_Boss(m3d::bmdl_c &mdl) {
    dGameCom::SetSoftLight_Boss(mdl, 0);
}

void dActor_c::setSoftLight_Item(m3d::bmdl_c &mdl) {
    nw4r::g3d::ResMdl resMdl = mdl.getResMdl();
    m3d::replaceLightMapTexture("lm_01i", mdl, 0);
    m3d::replaceLightMapTexture("lm_02i", mdl, 0);
    for (unsigned int i = 0; i < resMdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ScnMdl::CopiedMatAccess cma(nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mdl.mpScn), i);
        nw4r::g3d::ResMatMisc matMisc = cma.GetResMatMisc(false);
        if (matMisc.smth != 0) {
            matMisc.SetLightSetIdx(6);
        }
    }
}

void dActor_c::deleteActor(u8 param_1) {
    deleteRequest();

    u16 *c228 = _228;
    if (mSomeBitField == nullptr || c228 == nullptr) {
        return;
    }

    if (param_1 == 0) {
        if (mSomeBitField != nullptr) {
            *mSomeBitField = *mSomeBitField & 0xfe;
        }
    } else {
        if (mSomeBitField != nullptr) {
            *mSomeBitField = *mSomeBitField | 8;
        }
        if (c228 != nullptr) {
            *c228 = 300;
        }
    }
}

bool dActor_c::cullCheck(const mVec3_c &pos, const AreaBound &bound, u8 areaID) const {
    dCdFile_c *course = dCd_c::m_instance->getFileP(dScStage_c::m_instance->currCourse);
    dCdArea_c *area = course->getAreaP(areaID, nullptr);
    if (area == nullptr) {
        return true;
    }

    // [Probably some more inlining going on here]
    mVec2_c b;
    mVec2_c bt = bound.withPos(pos);
    b.set(bt.x, bt.y);

    b.x -= area->bound.x;
    b.y += area->bound.y;

    mVec2_c doubleBoundSize = bound.getSize();
    doubleBoundSize *= 2.0f;

    mVec2_c mbd(mCullAreaLimit.x, mCullAreaLimit.y);

    if ((course->mpUnk->unk & 1) == 0) {
        if (b.x + doubleBoundSize.x < -mbd.x || b.x > area->bound.width + mbd.x) {
            return true;
        }
    }
    if (b.y - doubleBoundSize.y > mbd.y || b.y < -(area->bound.height + mbd.y)) {
        return true;
    }

    return false;
}

bool dActor_c::ActorScrOutCheck(u16 someBitfield) {
    if (mNoDrawIf2 == 2) {
        return false;
    }
    if ((someBitfield & 8) == 0 && mBc.checkRide()) {
        return false;
    }

    AreaBound bound;
    bound.x = visibleAreaOffset.x;
    bound.y = visibleAreaOffset.y;
    bound.width = visibleAreaSize.x * 0.5f;
    bound.height = visibleAreaSize.y * 0.5f;

    bool res = false;
    if (cullCheck(mPos, bound, mAreaNo)) {
        res = true;
    } else if ((someBitfield & 4) == 0) {
        if (otherCullCheck(mPos, bound, getDestroyBound(), mAreaNo)) {
            res = true;
        }
    }
    if (res && (someBitfield & 2) == 0) {
        deleteActor(mWasSquished);
    }
    return res;
}


bool dActor_c::ActorDrawCullCheck() {
    AreaBound bound;
    bound.x = visibleAreaOffset.x;
    bound.y = visibleAreaOffset.y;
    bound.width = visibleAreaSize.x * 0.5f;
    bound.height = visibleAreaSize.y * 0.5f + 16.0f;
    mVec3_c pos = mPos;
    changePosAngle(&pos, nullptr, 1);
    return dGameCom::someCheck(&pos, &bound);
}

bool dActor_c::checkBgColl() {
    if (mBgCollFlags != 0) {
        if ((mBgCollFlags & 1) && mBc.isHead()) {
            return true;
        }
        if ((mBgCollFlags & 2) && mBc.isWallL()) {
            return true;
        }
        if ((mBgCollFlags & 8) && mBc.isFoot()) {
            return true;
        }
        if ((mBgCollFlags & 4) && mBc.isWallR()) {
            return true;
        }
    }
    return false;
}

void FUN_800b3600(int, int);
bool FUN_800b3100(mVec3_c *, mVec3_c *, mVec3_c *, mVec3_c *, float);
void FUN_800b3720(int, int, int);
void FUN_800b3780(int, int);
void FUN_800b3750(int, int, int);

bool dActor_c::carryFukidashiCheck(int param_2, mVec2_c triggerSize) {
    mVec3_c center = getCenterPos();

    mVec3_c minPos(center.x - triggerSize.x, center.y - triggerSize.y, mPos.z);
    mVec3_c maxPos(center.x + triggerSize.x, center.y + triggerSize.y, mPos.z);

    if (mCarryingPlayerNo <= 3 && dInfo_c::m_instance->smth[mCarryingPlayerNo][param_2]) {
        mCarryingPlayerNo = -1;
    }

    if (mCarryingPlayerNo <= 3) {
        dAcPy_c *player = daPyMng_c::getPlayer(mCarryingPlayerNo);
        if (player != nullptr) {
            bool drawingCarryFukidashi = player->isDrawingCarryFukidashi();

            AreaBound b;
            player->getCcBounds(b);
            mVec3_c newPos(dScStage_c::getLoopPosX(b.x + player->mPos.x), b.y + player->mPos.y, player->mPos.z);

            mVec3_c v1(newPos.x - b.width - 2.0f, newPos.y - b.height, newPos.z);
            mVec3_c v2(newPos.x + b.width + 2.0f, newPos.y + b.height, newPos.z);

            if (dfukidashiManager_c::m_instance->smth[mCarryingPlayerNo].smth == 0) {
                FUN_800b3600(mCarryingPlayerNo, param_2);
            }
            bool smthOther = FUN_800b3100(&minPos, &maxPos, &v1, &v2, 0.0f);
            if (!drawingCarryFukidashi || !smthOther) {
                FUN_800b3750(mCarryingPlayerNo, param_2, 0);
                mCarryingPlayerNo = -1;
            }
        }
    } else {
        dAcPy_c *player = searchCarryFukidashiPlayer(param_2);
        if (player != nullptr) {
            bool drawingCarryFukidashi = player->isDrawingCarryFukidashi();

            AreaBound b;
            player->getCcBounds(b);
            mVec3_c newPos(dScStage_c::getLoopPosX(b.x + player->mPos.x), b.y + player->mPos.y, player->mPos.z);

            mVec3_c v1(newPos.x - b.width, newPos.y - b.height, newPos.z);
            mVec3_c v2(newPos.x + b.width, newPos.y + b.height, newPos.z);

            bool smthOther = FUN_800b3100(&minPos, &maxPos, &v1, &v2, 0.0f);
            if (drawingCarryFukidashi && smthOther) {
                mCarryingPlayerNo = *player->getPlrNo();
            }
        }
    }

    return false;
}

void dActor_c::carryFukidashiCancel(int idk, int plNo) {
    for (int i = 0; i < 4; i++) {
        if (i == plNo) {
            FUN_800b3720(i, idk, 0);
            FUN_800b3780(i, idk);
        } else {
            FUN_800b3750(i, idk, 0);
        }
    }
    mCarryingPlayerNo = -1;
}

dAcPy_c *dActor_c::searchCarryFukidashiPlayer(int fukidashiNum) {
    mVec3_c center = getCenterPos();
    dAcPy_c *closest = nullptr;
    float closestDist = 1e9;
    for (int i = 0; i < 4; i++) {
        if (daPyMng_c::checkPlayer(i) && !dInfo_c::m_instance->smth[i][fukidashiNum]) {
            dAcPy_c *player = daPyMng_c::getPlayer(i);
            if (player == nullptr) {
                continue;
            }
            float diffX = player->mPos.x + player->mCenterOffs.x - center.x;
            float diffY = player->mPos.y + player->mCenterOffs.y - center.y;
            float dist = diffX * diffX + diffY * diffY;
            if (closestDist > dist) {
                closest = player;
                closestDist = dist;
            }
        }
    }
    return closest;
}

mVec2_c dActor_c::getLookatPos() const {
    return mVec2_c(mPos.x + mCenterOffs.x, mPos.y + mCenterOffs.y);
}

void dActor_c::block_hit_init() {}

void dActor_c::allEnemyDeathEffSet() {
    mVec3_c center = getCenterPos();
    center.y += 4.0f;
    mEf::createEffect("Wm_en_burst_s", 0, &center, nullptr, nullptr);
}

void dActor_c::burst(s8 smth, int noScore) {
    // Used when the Wiimote platforms squish an actor
    mVec3_c burstPos = mPos;
    mVec3_c oldCenter = mCenterOffs;
    mWasSquished = true;
    for (int i = 0; i < daPyMng_c::mNum; i++) {
        dAcPy_c *player = daPyMng_c::getPlayer(i);
        if (player != nullptr && fManager_c::searchBaseByID(player->carryActorID) == this) {
            player->cancelCarry(this);
        }
    }
    burstPos += oldCenter;
    mEf::createEffect("Wm_en_burst_s", 0, &burstPos, nullptr, nullptr);
    deleteActor(1);
    if (!noScore) {
        if (smth < 0) {
            dScoreMng_c::m_instance->FUN_800e24b0(dScoreMng_c::smc_SCORE_X, dScoreMng_c::smc_SCORE_Y, this, true);
        } else {
            dScoreMng_c::m_instance->FUN_800e2190(dScoreMng_c::smc_SCORE_X, dScoreMng_c::smc_SCORE_Y, this, true, smth);
        }
    }
}

void dActor_c::setSpinLiftUpActor(dActor_c *actor) {}

void dActor_c::setEatTongue(dActor_c *actor) {
    mPreEatScale = mScale;
}

void dActor_c::setEatTongueOff(dActor_c *actor) {}

void dActor_c::setEatMouth(dActor_c *actor) {}

bool dActor_c::setEatSpitOut(dActor_c *actor) {
    return true;
}

bool dActor_c::setEatGlupDown(dActor_c *actor) {
    static const int yoshiEatPoints[] = { 1, 4 }; ///< @unofficial
    static const int yoshiEatPoints2[] = { 200, 1000 }; ///< @unofficial

    if (_254 != 2) {
        mVec3_c adjPos = actor->mPos;
        adjPos.y += 40.0f;
        s8 plrNo = *actor->getPlrNo();
        dGameCom::CreateSmallScore(adjPos, yoshiEatPoints[_254], plrNo, false);
        if (plrNo != -1) {
            daPyMng_c::addScore(yoshiEatPoints2[_254], plrNo);
            dMultiMng_c::mspInstance->incEnemyDown(plrNo);
        }
    }
    deleteRequest();

    return true;
}

void dActor_c::setAfterEatScale() {
    mScale = mPreEatScale;
}

void dActor_c::calcSpitOutPos(dActor_c *actor) {
    mMtx_c mouthMtx;
    ((daYoshi_c *) actor)->getMouthMtx(&mouthMtx);
    mMtx_c transposeMtx;
    PSMTXTrans(transposeMtx, 10.0f, -7.0f, 0.0f);
    mMtx_c resMtx;
    PSMTXConcat(mouthMtx, transposeMtx, mouthMtx);
    mPos.x = mouthMtx.transX();
    mPos.y = mouthMtx.transY();
}

float dActor_c::calcEatScaleRate(dActor_c *actor) {
    float res = 1.0f;
    int idk = ((daYoshi_c *) actor)->_a0;
    if (idk <= 1) {
        res = idk / 1.25f;
    }
    return res;
}

void dActor_c::calcEatInScale(dActor_c *actor) {
    mScale = mPreEatScale;
    float scaleRate = calcEatScaleRate(actor);
    if (scaleRate < 1.0f) {
        mScale *= scaleRate;
    }
}

void dActor_c::eatMove(dActor_c *actor) {
    mMtx_c tongueTipMtx;
    ((daYoshi_c *) actor)->getTongueTipMtx(&tongueTipMtx);
    mPos.x = tongueTipMtx.transX();
    mPos.y = tongueTipMtx.transY() - mCenterOffs.y;
}

void dActor_c::vfb4() {}

void dActor_c::cancelFunsuiActUpper() {}

void dActor_c::cancelFunsuiActSide(int) {}

void dActor_c::cancelFunsuiActVanish() {}

// [(Place this in a separate file, other actors make use of this too.)]
class SlideComboSELookup {
public:
    SlideComboSELookup(u32 v0, u32 v1, u32 v2, u32 v3, u32 v4, u32 v5, u32 v6, u32 v7, u32 v8) {
        lookup[0] = v0;
        lookup[1] = v1;
        lookup[2] = v2;
        lookup[3] = v3;
        lookup[4] = v4;
        lookup[5] = v5;
        lookup[6] = v6;
        lookup[7] = v7;
        lookup[8] = v8;
    }

    template<class T>
    void playObjSound(T *obj, int i, const mVec3_c &pos, int playerNo) const {
        obj->startSound(lookup[i], dAudio::cvtSndObjctPos(pos), playerNo);
    }

    void playEmySound(int i, const mVec3_c &pos, int playerNo) const {
        playObjSound(dAudio::g_pSndObjEmy, i, pos, playerNo);
    }

    void playMapSound(int i, const mVec3_c &pos, int playerNo) const {
        playObjSound(dAudio::g_pSndObjMap, i, pos, playerNo);
    }

    u32 size() const { return 9; }

    u32 lookup[9];
};

void dActor_c::slideComboSE(int param_1, bool param_2) {
    const static SlideComboSELookup cs_combo_se(
        SE_EMY_KAME_HIT_1,
        SE_EMY_KAME_HIT_2,
        SE_EMY_KAME_HIT_3,
        SE_EMY_KAME_HIT_4,
        SE_EMY_KAME_HIT_5,
        SE_EMY_KAME_HIT_6,
        SE_EMY_KAME_HIT_7,
        SE_EMY_KAME_HIT_7,
        SE_EMY_KAME_HIT_7
    );

    if ((u8) mPlayerNo <= 3) {
        int a = param_1;
        if (param_1 >= cs_combo_se.size()) {
            a = 8;
        };
        int countNeededForClaps = param_2 ? 4 : 7;
        if (a >= countNeededForClaps) {
            dMultiMng_c::mspInstance->setClapSE();
        }

        mVec3_c centerPos = getCenterPos();
        cs_combo_se.playEmySound(a, centerPos, dAudio::getRemotePlayer(mPlayerNo));
    }
}

void dActor_c::clrComboCnt() {
    mComboCount = 0;
}

void dActor_c::waterSplashEffect(const mVec3_c &pos, float param_2) {
    mVec3_c shiftedPos(pos, 6500.0f);

    int waterDepth = dBc_c::checkWaterDepth(shiftedPos.x, shiftedPos.y, mLayer, mBc._e5, nullptr);

    u32 idk = 0;
    if (waterDepth < 3) {
        idk = 1;
    }
    idk |= (mLayer << 16);

    mVec3_c weirdPos(param_2, param_2, param_2);
    dEffActorMng_c::m_instance->createWaterSplashEff(shiftedPos, idk, -1, weirdPos);

    dAudio::g_pSndObjMap->startSound(SE_OBJ_CMN_SPLASH, shiftedPos, 0);

    dBg_c::m_bg_p->setWaterInWave(pos.x, pos.y, 6);
}

void dActor_c::yoganSplashEffect(const mVec3_c &pos, float param_2) {
    mVec3_c shiftedPos(pos, 6500.0f);

    u32 idk = 4 | (mLayer << 16);
    mVec3_c weirdPos(param_2, param_2, param_2);
    dEffActorMng_c::m_instance->createWaterSplashEff(shiftedPos, idk, -1, weirdPos);

    dAudio::g_pSndObjMap->startSound(SE_OBJ_CMN_SPLASH_LAVA, shiftedPos, 0);

    if (param_2 < 1.0f) {
        dBg_c::m_bg_p->setWaterInWave(pos.x, pos.y, 16);
    } else {
        dBg_c::m_bg_p->setWaterInWave(pos.x, pos.y, 14);
    }
}

void dActor_c::poisonSplashEffect(const mVec3_c &pos, float param_2) {
    mVec3_c shiftedPos(pos, 6500.0f);

    u32 idk = 6 | (mLayer << 16);
    mVec3_c bruh(param_2, param_2, param_2);
    dEffActorMng_c::m_instance->createWaterSplashEff(shiftedPos, idk, -1, bruh);

    dAudio::g_pSndObjMap->startSound(SE_OBJ_CMN_SPLASH_POISON, pos, 0);

    if (param_2 < 1.0f) {
        dBg_c::m_bg_p->setWaterInWave(pos.x, pos.y, 23);
    } else {
        dBg_c::m_bg_p->setWaterInWave(pos.x, pos.y, 21);
    }
}

bool dActor_c::checkCarried(int *res) {
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = daPyMng_c::getPlayer(i);
        if (player != nullptr && fManager_c::searchBaseByID(player->carryActorID) == this) {
            if (res != nullptr) {
                *res = *player->getPlrNo();
            }
            return true;
        }
    }
    if (res != nullptr) {
        *res = -1;
    }
    return false;
}

void dActor_c::removeCc() {
    mCc.release();
}

void dActor_c::reviveCc() {
    mCc.entry();
}