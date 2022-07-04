#pragma once
#include "IEventManager.h"

constexpr uint32_t EVENT_MANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
public:
    EventManager(char const* name, bool setAsGlobal);
    ~EventManager() override;
    bool AddListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type) override;
    bool RemoveListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type) override;
    bool TriggerEvent(const shared_ptr<IEventData>& event) override;
    bool QueueEvent(const shared_ptr<IEventData>& event) override;
    bool AbortEvent(const EventType& type, bool allOfType) override;
    bool TickUpdate(uint64_t maxMills) override;
    
private:
    std::map<EventType, std::list<EventListenerDelegate>> _eventListeners;
    std::list<shared_ptr<IEventData>> _queues[EVENT_MANAGER_NUM_QUEUES];
    int _activeQueue;
};