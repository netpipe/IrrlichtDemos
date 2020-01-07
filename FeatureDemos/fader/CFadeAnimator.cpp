
#include "CFadeAnimator.h"

namespace irr {
namespace scene {

CFadeAnimator::CFadeAnimator(int mode, u32 timeForWay, u32 now,
    std::vector<video::ITexture*> *textures) : StartTime(now), TimeForWay(timeForWay),
    Textures(textures), FadeMode(mode) {
    #ifdef _DEBUG
   setDebugName("CFadeAnimator");
   #endif

    int max = 0;

    if (!textures) {
        Start = 0;
        End = 0;
        TimeFactor = 0;
        return;
    } else {
        if (textures->size() <= 0) {
            Start = 0;
            End = 0;
            TimeFactor = 0;
            return;
        }
    }

    max = textures->size() - 1;
    if (FadeMode == CFA_FADEIN) {
        Start = 0;
        End = max;
    } else if (FadeMode == CFA_FADEOUT) {
        Start = max;
        End = 0;
    } else {
        Start = 0;
        End = 0;
        TimeFactor = 0;
        return;
    }

    TimeFactor = (f32)max / TimeForWay;
}

CFadeAnimator::~CFadeAnimator() {

}

void CFadeAnimator::animateNode(ISceneNode* node, u32 timeMs) {
    if (!node) {
        return;
    }

    u32 t = timeMs - StartTime;
    int index = Start;

    if (t >= TimeForWay) {
        index = End;
    } else {
        if (FadeMode == CFA_FADEIN) {
            index = (int)((f32)fmod(t, TimeForWay) * TimeFactor);
        } else if (FadeMode == CFA_FADEOUT) {
            index = Start - (int)((f32)fmod(t, TimeForWay) * TimeFactor);
        }
    }

    node->setMaterialTexture(0, Textures->at(index));
}

}} 
