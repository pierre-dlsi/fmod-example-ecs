
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


FmodSoundEngine::FmodSoundEngine() {
    SOUND_TRACE("Initializing FMOD engine in our game...");
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
    ERRCHECK(soundSystem->loadBankFile("assets/fmod_banks/music_bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile("assets/fmod_banks/music_bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    SOUND_INFO("Successfully loaded FMOD banks");
}

void 
FmodSoundEngine::setListenerPosition(float x, float y, float z) {
        FMOD_3D_ATTRIBUTES listenerAttr;
        ERRCHECK(soundSystem->getListenerAttributes(0, &listenerAttr));
        listenerAttr.position = FMOD_VECTOR{x, y, z};
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
     
