#pragma once

#include <string>
#include <map>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

#include <ISound.hpp>

class FmodSoundEngine : public ISound {
    private:
        std::map<std::string, FMOD::Studio::EventDescription*> eventMap;
        std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;
        FMOD::Studio::System * soundSystem;
        FMOD::System* low_level_system;
        FMOD::Studio::Bank* master_bank;
        FMOD::Studio::Bank* strings_bank;

    public:
        FmodSoundEngine();
        virtual void init(std::string conf_filename);
        virtual void setListenerPosition(float x, float y, float z);
        virtual void setRadioTuning(int dial);
        virtual void update();
        virtual void loadSoundBank(const std::string& bank_name) ;
        virtual SoundComponent* getSoundComponent(const std::string& eventID);
        virtual void setSoundPosition(SoundComponent* sc, const PositionComponent& pos);
        virtual void start(const SoundComponent* sc);
        virtual void stop(const SoundComponent* sc);
        virtual void updateSoundEventParameter(const SoundComponent* sc, const std::string& paramName, float paramValue);


};