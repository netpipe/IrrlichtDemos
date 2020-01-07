
#ifndef _CFadeAnimator_h__
#define _CFadeAnimator_h__

#define CFA_FADEIN  1
#define CFA_FADEOUT 2

#include "ISceneNode.h"
#include <vector>

namespace irr {
namespace scene {
    class CFadeAnimator : public ISceneNodeAnimator {
    public:
        //! constructor
        CFadeAnimator(int mode, u32 timeForWay, u32 now, std::vector<video::ITexture*> *textures);

        //! destructor
        virtual ~CFadeAnimator();

        //! animates a scene node
        virtual void animateNode(ISceneNode* node, u32 timeMs);

        		virtual ISceneNodeAnimator* createClone(ISceneNode* node,
				ISceneManager* newManager=0) {};
    private:
        int FadeMode;
        f32 TimeFactor;
        u32 StartTime;
        u32 TimeForWay;
        int Start, End;
        std::vector<video::ITexture*> *Textures;
    };
}}

#endif
