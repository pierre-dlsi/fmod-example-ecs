#include <FmodSoundEngine.hpp>
#include <Logger.hpp>

void ERRCHECK_FMOD (FMOD_RESULT result, const char * file, int line)
{
	if(result != FMOD_OK)
	{
        SOUND_ERROR("{0} - {1}", result, FMOD_ErrorString(result));
		exit(-1);
	}
}
#define ERRCHECK(_result) ERRCHECK_FMOD(_result, __FILE__, __LINE__)

 
FmodSoundEngine::FmodSoundEngine(const std::string& soundAssetsFolder) {
    SOUND_TRACE("Initializing FMOD engine in our game...");
    this->soundAssetsFolder = soundAssetsFolder;
    ERRCHECK(FMOD::Studio::System::create(&soundSystem));
    ERRCHECK(soundSystem->getCoreSystem(&low_level_system));
    ERRCHECK(low_level_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(low_level_system->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(soundSystem->initialize(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));
    SOUND_INFO("Successfully initialized FMOD engine");    
}

void
FmodSoundEngine::init(std::string conf_filename) {
    SOUND_TRACE("Initializing FMOD engine in our game...");
    ERRCHECK(soundSystem->loadBankFile((soundAssetsFolder+"/music_bank.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile((soundAssetsFolder+"/music_bank.strings.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    SOUND_INFO("Successfully loaded FMOD banks");
}

void 
FmodSoundEngine::setListenerPosition(const PositionComponent& pos) {
        FMOD_3D_ATTRIBUTES listenerAttr;
        ERRCHECK(soundSystem->getListenerAttributes(0, &listenerAttr));
        listenerAttr.position = FMOD_VECTOR{-pos.x, pos.y, pos.z};
        ERRCHECK(soundSystem->setListenerAttributes(0, &listenerAttr));

        //FMOD_3D_ATTRIBUTES l_listenerAttributes {FMOD_VECTOR{x, y, z}, FMOD_VECTOR{0,0,0}, FMOD_VECTOR{normalizedForward.x, 0.0f, -normalizedForward.z}, FMOD_VECTOR{0,1,0}};
        //ERRCHECK(soundSystem->setListenerAttributes(0, &l_listenerAttributes));

}

void 
FmodSoundEngine::setRadioTuning(int dial) {
    
}

void 
FmodSoundEngine::update() {

}

void 
FmodSoundEngine::loadSoundBank(const std::string& bank_name) 
{
    if (bankMap.find(bank_name) == bankMap.end()) {
        SOUND_TRACE("Loading sound bank "+bank_name);
        FMOD::Studio::Bank *bank;
        ERRCHECK(soundSystem->loadBankFile((soundAssetsFolder+bank_name).c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
        bankMap[bank_name] = bank;       
        SOUND_TRACE("Sound bank "+bank_name+ " succesfully loaded");
    } else
        SOUND_TRACE("Sound bank "+bank_name+ " already loaded");

}

void 
FmodSoundEngine::unloadSoundBank(const std::string& bank_name)
{
        if (bankMap.find(bank_name) == bankMap.end()) 
            SOUND_TRACE("Sound bank "+bank_name+ " already unloaded");
        else {
          FMOD::Studio::Bank* bank = bankMap[bank_name]; 
            //TODO : delete EventDescription and EventInstances generated from this bank from eventMap and eventInstanceMap
          ERRCHECK(bank->unload()); // This will destroy all objects created from the bank, unload all sample data inside the bank, and invalidate all API handles referring to the bank.
          bankMap.erase(bank_name);
          SOUND_TRACE("Sound bank "+bank_name+" succesfully unloaded");
        }

}

SoundComponent* 
FmodSoundEngine::getSoundComponent(const std::string& eventID) 
{
    return NULL;
}

void 
FmodSoundEngine::setSoundPosition(SoundComponent* sc, const PositionComponent& pos)
{}

void 
FmodSoundEngine::start(const SoundComponent* sc)
{}

void 
FmodSoundEngine::stop(const SoundComponent* sc)
{}

void 
FmodSoundEngine::updateSoundEventParameter(const SoundComponent* sc, const std::string& paramName, float paramValue)
{}

  
