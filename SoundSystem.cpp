#include <SoundSystem.hpp>
#include <Logger.hpp>
#include <Components.hpp>
#include <raymath.h>

void ERRCHECK_FMOD (FMOD_RESULT result, const char * file, int line)
{
	if(result != FMOD_OK)
	{
        SOUND_ERROR("{0} - {1}", result, FMOD_ErrorString(result));
		exit(-1);
	}
}
#define ERRCHECK(_result) ERRCHECK_FMOD(_result, __FILE__, __LINE__)

SoundSystem::SoundSystem()
{
    SOUND_TRACE("Initializing FMOD engine in our game...");
    ERRCHECK(FMOD::Studio::System::create(&soundSystem));
    ERRCHECK(soundSystem->getCoreSystem(&low_level_system));
    ERRCHECK(low_level_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(low_level_system->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(soundSystem->initialize(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));
    SOUND_INFO("Successfully initialized FMOD engine");
}

SoundSystem::~SoundSystem()
{
    SOUND_TRACE("Dropping FMOD...");
    for(const auto& eventDesc : eventMap)
    {
        eventDesc.second->releaseAllInstances();
    }

    for(const auto& instance : eventInstances)
    {
        instance.second->release();
    }
    master_bank->unload();
    strings_bank->unload();
    SOUND_INFO("FMOD dropped");
}

void SoundSystem::InitBanks(const std::string& master_bank_location, const std::string& strings_bank_location)
{
    SOUND_TRACE("Initializing FMOD engine in our game...");
    ERRCHECK(soundSystem->loadBankFile(master_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile(strings_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    SOUND_INFO("Successfully loaded FMOD banks");
}

void SoundSystem::AddSoundComponent(entt::entity& ent, entt::registry& reg, const std::string& eventName, std::vector<std::string> floatvars, std::vector<std::string> intvars)
{
    std::string eventID = eventName + std::to_string((uint32_t)ent);
    SOUND_TRACE("Loading {0} as a component for entity {1}", eventName, ent);
    FMOD::Studio::EventInstance * soundInstance = nullptr;
    if(eventMap.find(eventName) == eventMap.end())
    {
        FMOD::Studio::EventDescription * eventDescription = nullptr;
        ERRCHECK(soundSystem->getEvent(eventName.c_str(), &eventDescription));
        eventMap[eventName] = eventDescription;
        ERRCHECK(eventDescription->createInstance(&soundInstance));
    }
    else
    {
        ERRCHECK(eventMap[eventName]->createInstance(&soundInstance));
    }
    
    if(reg.all_of<PositionComponent>(ent))
    {   
        SOUND_TRACE("Including event into 3D position");
        auto poscomp = reg.get<PositionComponent>(ent); 
        FMOD_3D_ATTRIBUTES l_eventAttributes {FMOD_VECTOR{-poscomp.x, poscomp.y, poscomp.z}, FMOD_VECTOR{0,0,0}, FMOD_VECTOR{0,0,1}, FMOD_VECTOR{0,1,0}};
        ERRCHECK(soundInstance->set3DAttributes(&l_eventAttributes));
    }

    eventInstances[eventID] = soundInstance;
    std::map<std::string, float> floatmap;
    std::map<std::string, int> intmap;
    for(const auto& element : floatvars)
    {
        floatmap[element] = 0.0f;
    }
    for(const auto& element : intvars)
    {
        intmap[element] = 0.0f;
    }
    reg.emplace<SoundComponent>(ent, eventID, floatmap, intmap, 0, 0, 1);
    
    ERRCHECK(soundInstance->start());
    SOUND_TRACE("Entity {0} has registered a new Sound Component", ent);
}

void SoundSystem::Update(entt::registry& reg)
{
    //Update Listener Position
    auto view = reg.view<const ListenerComponent, const PositionComponent, const CameraComponent>();
    for(auto [lc, p_comp, c_comp]: view.each()) 
    {
        Vector3 normalizedForward = Vector3Normalize((Vector3){p_comp.x-c_comp.tarX, p_comp.y-c_comp.tarY, p_comp.z-c_comp.tarZ});
        FMOD_3D_ATTRIBUTES l_listenerAttributes {FMOD_VECTOR{-p_comp.x, p_comp.y, p_comp.z}, FMOD_VECTOR{0,0,0}, FMOD_VECTOR{normalizedForward.x, 0.0f, -normalizedForward.z}, FMOD_VECTOR{0,1,0}};
        ERRCHECK(soundSystem->setListenerAttributes(0, &l_listenerAttributes));
    }

    auto view_sc = reg.view<SoundComponent>();

    for(auto [e, sc] : view_sc.each())
    {
        if(sc.marked_for_play)
        {
            ERRCHECK(eventInstances[sc.event_id]->start());
            sc.marked_for_play = 0;
        }
        if(sc.marked_for_stop)
        {
            ERRCHECK(eventInstances[sc.event_id]->stop(FMOD_STUDIO_STOP_IMMEDIATE));
            sc.marked_for_stop = 0;
        }
        if(sc.marked_for_parameter_update)
        {
            for(const auto& element : sc.floatparameers)
                ERRCHECK(eventInstances[sc.event_id]->setParameterByName(element.first.c_str(), element.second));
            
            for(const auto& element : sc.intParameters)
                ERRCHECK(eventInstances[sc.event_id]->setParameterByName(element.first.c_str(), element.second));
            
            reg.replace<SoundComponent>(e, sc.event_id, sc.floatparameers, sc.intParameters, 0, sc.marked_for_play, sc.marked_for_stop);
        }
            
    }

    //Update 
    soundSystem->update();
}