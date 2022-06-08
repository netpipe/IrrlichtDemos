
// (c) Tank @ War
 
#include "SoundSceneNode.h"
 
using namespace irr;
using namespace scene;
 
SoundSceneNode::SoundSceneNode(cAudio::IAudioSource* sound, cAudio::IAudioManager* soundManager, scene::ISceneNode* parent, scene::ISceneManager* smgr) :
        scene::ISceneNode(parent, smgr, -1) {
    this->sound = sound;
    this->soundManager = soundManager;
    deleteWhenFinished = true;
    box = core::aabbox3df(0, 0, 0, 0, 0, 0);
 
    if (sound) {
        // Play sound
        cAudio::cVector3 position = irrlichtToCAudioVector(getAbsolutePosition());
        sound->setPosition(position);
        soundVolume = sound->getVolume();
        if (sound->getVolume() == 0) {
            soundVolume = -1;
        }
 
        OnAnimate(0);
        sound->play3d(position, sound->getStrength(), sound->isLooping());
    }
}
 
SoundSceneNode::~SoundSceneNode() {
    if (sound) {
        if (sound->isPlaying()) {
            sound->stop();
        }
        soundManager->release(sound);
    }
}
 
void SoundSceneNode::setDeleteWhenFinished(bool deleteWhenFinished) {
    this->deleteWhenFinished = deleteWhenFinished;
}
 
void SoundSceneNode::OnRegisterSceneNode() {
    ISceneNode::OnRegisterSceneNode();
}
 
void SoundSceneNode::render() {
}
 
void SoundSceneNode::OnAnimate(u32 timeMs) {
    if (sound) {
        // Move to new position
        sound->move(irrlichtToCAudioVector(getAbsolutePosition()));
 
        // Get transformed bounding box
        const core::aabbox3df transformedBox = getTransformedBoundingBox();
        core::vector3df listenerPosition = cAudioToIrrlichtVector(soundManager->getListener()->getPosition());
 
        // Get distance from position
        f32 distance = 0;
        if (box.getVolume() == 0) {
            distance = listenerPosition.getDistanceFrom(getAbsolutePosition());
        }
        // Get distance from bounding box
        else if (!transformedBox.isPointInside(listenerPosition)) {
            irr::core::vector3df edges[8];
            transformedBox.getEdges(edges);
 
            // Create planes
            irr::core::plane3df planes[6];
            planes[0] = irr::core::plane3df(edges[3], edges[1], edges[0]); // Left plane
            planes[1] = irr::core::plane3df(edges[5], edges[7], edges[6]); // Right plane
            planes[2] = irr::core::plane3df(edges[1], edges[5], edges[4]); // Front plane
            planes[3] = irr::core::plane3df(edges[7], edges[3], edges[2]); // Back plane
            planes[4] = irr::core::plane3df(edges[3], edges[7], edges[5]); // Top plane
            planes[5] = irr::core::plane3df(edges[0], edges[4], edges[6]); // Bottom plane
 
            // Get distance to nearest plane
            core::vector3df center = transformedBox.getCenter();
            core::vector3df intersectionPosition;
 
            // Get distance to nearest plane
            for (u32 i = 0; i < 6; i++) {
                // Check if there is an intersection with this plane
                if (planes[i].getIntersectionWithLimitedLine(listenerPosition, center, intersectionPosition)) {
                    f32 distanceToPlane = planes[i].getDistanceTo(listenerPosition);
                    if (distance == 0) {
                        distance = distanceToPlane;
                    } else if (distanceToPlane > distance) {
                        distance = distanceToPlane;
                    }
                }
            }
        }
 
        // Set volume
        if (distance > sound->getMaxDistance() || soundVolume <= 0) {
            sound->setVolume(0);
        } else if (distance <= sound->getMinDistance()) {
            sound->setVolume(soundVolume);
        } else {
            f32 gain = (sound->getRolloffFactor() * (distance - sound->getMinDistance()) / (sound->getMaxDistance() - sound->getMinDistance()));
            sound->setVolume(soundVolume - (soundVolume * gain));
        }
 
        // Delete sound when not looping and finished playing
        if (deleteWhenFinished && !sound->isLooping() && sound->isStopped()) {
            SceneManager->addToDeletionQueue(this);
        }
    } else {
        SceneManager->addToDeletionQueue(this);
    }
 
    ISceneNode::OnAnimate(timeMs);
}
 
f32 SoundSceneNode::getVolume() const {
    return soundVolume;
}
 
void SoundSceneNode::setVolume(f32 volume) {
    soundVolume = volume;
    if (soundVolume == 0) {
        sound->setVolume(0);
        soundVolume = -1;
    }
}
 
const core::aabbox3d<f32>& SoundSceneNode::getBoundingBox() const {
    return box;
}
 
void SoundSceneNode::setBoundingBox(const core::aabbox3df box) {
    this->box = box;
}
 
scene::ESCENE_NODE_TYPE SoundSceneNode::getType() const {
    return (scene::ESCENE_NODE_TYPE) SOUND_SCENE_NODE_ID;
}
 
cAudio::IAudioSource* SoundSceneNode::getSound() const {
    return sound;
}
 
cAudio::cVector3 SoundSceneNode::irrlichtToCAudioVector(core::vector3df v) {
    return cAudio::cVector3(v.X, v.Y, v.Z);
}
 
core::vector3df SoundSceneNode::cAudioToIrrlichtVector(cAudio::cVector3 v) {
    return core::vector3df(v.x, v.y, v.z);
}
 
