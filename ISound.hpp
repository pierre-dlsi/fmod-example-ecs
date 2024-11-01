#pragma once

#include <string>
#include <map>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

#include <Components.hpp>

class ISound {
    public:
        virtual void init(const std::string& conf_filename) = 0;
        virtual void loadSoundBank(const std::string& bank_name) = 0;
        virtual void setListenerPosition(const PositionComponent& pos) = 0;
        virtual SoundComponent* getSoundComponent(const std::string& eventID) = 0;
        virtual void setSoundPosition(SoundComponent* sc, const PositionComponent& pos) = 0;
        virtual void start(const SoundComponent* sc) = 0;
        virtual void stop(const SoundComponent* sc) = 0;
        virtual void updateSoundEventParameter(const SoundComponent* sc, const std::string& paramName, float paramValue) = 0;
        virtual void setRadioTuning(int dial) = 0; // fachada específica
        virtual void update() = 0;
};

