#pragma once

#include <string>
#include <map>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

class ISound {
    public:
        virtual void init(std::string conf_filename) = 0;
        virtual void setListenerPosition(float x, float y, float z) = 0;
        virtual void setRadioTuning(int dial) = 0;
        virtual void update() = 0;
};

