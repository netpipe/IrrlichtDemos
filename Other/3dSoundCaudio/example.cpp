
irr::scene::SoundSceneNode* SoundController::createSoundNode(const irr::c8* filename, scene::ISceneNode* parent, const bool mute, const bool playLooped, const f32 minDist, const f32 maxDist) {
 
    cAudio::IAudioSource* sound = soundManager->create(filename, filename);
    if (sound) {
            if (mute) {
                    sound->setVolume(0);
            } else {
                    sound->setVolume(soundVolume);
            }
            sound->setMinDistance(minDist);
            sound->setMaxAttenuationDistance(maxDist);
            sound->loop(playLooped);
 
            scene::SoundSceneNode* soundNode = new scene::SoundSceneNode(sound, soundManager, parent, DeviceContainer::smgr);
            soundNode->drop();
            return soundNode;
    }
    return 0;
}
