#pragma once
#include "IEventManager.h"

class EventDestroyActor : public BaseEventData
{
public:
    static const EventType SkEventType;

    explicit EventDestroyActor(uint64_t actorId): _actorId(actorId) {}
    explicit EventDestroyActor(std::istream& in) { in >> _actorId; }

    const EventType& GetEventType() const override { return SkEventType; }
    shared_ptr<IEventData> Copy() const override { return std::make_shared<EventDestroyActor>(_actorId); }
    void Serialize(std::ostrstream& out) const override { out << _actorId; }
    const char* GetName() const override { return "EventDestroyActor"; }

    uint32_t GetId() const { return _actorId; }
    
private:
    uint64_t _actorId;
};