
/*
 * SoundSceneNode.h
 *
 * Tank @ War Project
 */
 
#ifndef SOUNDSCENENODE_H_
#define SOUNDSCENENODE_H_
 
#include <irrlicht.h>
#include <cAudio.h>
 
namespace irr {
namespace scene {
 
#ifdef MAKE_IRR_ID
const int SOUND_SCENE_NODE_ID = MAKE_IRR_ID('s','n','d','n');
#else
const int SOUND_SCENE_NODE_ID = 'sndn';
#endif // MAKE_IRR_ID
class SoundSceneNode: public scene::ISceneNode {
private:
    core::aabbox3df box;
    cAudio::IAudioSource* sound;
    cAudio::IAudioManager* soundManager;
    bool deleteWhenFinished;
    f32 soundVolume;
 
public:
    /**
     * Constructor of SoundSceneNode
     * @param sound
     * @param soundManager
     * @param parent
     * @param smgr
     */
    SoundSceneNode(cAudio::IAudioSource* sound, cAudio::IAudioManager* soundManager, scene::ISceneNode* parent, scene::ISceneManager* smgr);
 
    /**
     * Destructor
     */
    virtual ~SoundSceneNode();
 
    /**
     * Set delete when finished
     * @param deleteWhenFinished
     */
    void setDeleteWhenFinished(bool deleteWhenFinished);
 
    /**
     * On register scene node
     */
    virtual void OnRegisterSceneNode();
 
    /**
     * Render
     */
    virtual void render();
 
    /**
     * On Animate
     * @param timeMs
     */
    virtual void OnAnimate(u32 timeMs);
 
    /**
     * Get AA bounding box
     */
    virtual const core::aabbox3d<f32>& getBoundingBox() const;
 
    /**
     * Set bounding box
     * @param box
     */
    void setBoundingBox(const core::aabbox3df box);
 
    /**
     * Get type
     * @return type
     */
    virtual scene::ESCENE_NODE_TYPE getType() const;
 
    /**
     * Get sound source
     * @return sound
     */
    cAudio::IAudioSource* getSound() const;
 
    /**
     * Get sound volume
     * @return soundVolume
     */
    f32 getVolume() const;
 
    /**
     * Set sound volume
     * @param volume
     */
    void setVolume(const f32 volume);
 
    /**
     * Convert irrlicht to cAudio vector
     * @param v
     * @return vector
     */
    static cAudio::cVector3 irrlichtToCAudioVector(core::vector3df v);
 
    /**
     * Convert cAudio to irrlicht vector
     * @param v
     * @return vector
     */
    static core::vector3df cAudioToIrrlichtVector(cAudio::cVector3 v);
};
 
} // end namespace scene
 
} // end namespace irr
 
#endif /* SOUNDSCENENODE_H_ */
 
