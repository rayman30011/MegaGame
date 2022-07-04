#include "MegaEngineStd.h"

#include "EventManagerImp.h"

EventManager::EventManager(char const* name, bool setAsGlobal) : IEventManager(name, setAsGlobal)
{
    _activeQueue = 0;
}

EventManager::~EventManager()
{
}

bool EventManager::AddListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type)
{
    auto listeners = _eventListeners[type];
    for (const auto delegate : listeners)
    {
        if (delegate == eventListenerDelegate)
        {
            _ERROR(L"Attempting to double-register a delegate");
            return false;
        }
    }

    listeners.push_back(eventListenerDelegate);
    return true;
}

bool EventManager::RemoveListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type)
{
    bool success = false;
    if (const auto found = _eventListeners.find(type); found != _eventListeners.end())
    {
        auto listeners = found->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it)
        {
            if (eventListenerDelegate == *it)
            {
                listeners.erase(it);
                success = true;
                break;
            }
        }
    }
    
    return success;
}

bool EventManager::TriggerEvent(const shared_ptr<IEventData>& event)
{
    bool processed = false;
    if (const auto found = _eventListeners.find(event->GetEventType()); found != _eventListeners.end())
    {
        auto listeners = found->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it)
        {
            auto listener = *it;
            listener(event);
            processed = true;
        }
    }

    return processed;
}

bool EventManager::QueueEvent(const shared_ptr<IEventData>& event)
{
    if (const auto found = _eventListeners.find(event->GetEventType()); found != _eventListeners.end())
    {
        _queues[_activeQueue].push_back(event);
        return true;
    }
    
    return false;
}

bool EventManager::AbortEvent(const EventType& type, bool allOfType)
{
    bool success = false;
    if (const auto found = _eventListeners.find(type); found != _eventListeners.end())
    {
        auto& queue = _queues[_activeQueue];
        auto it = queue.begin();

        while (it != queue.end())
        {
            auto thisIt = it;
            ++it;

            if ((*thisIt)->GetEventType() == type)
            {
                queue.erase(thisIt);
                success = true;
                if (!allOfType)
                    break;
            }
        }
    }
    
    return success;
}

bool EventManager::TickUpdate(uint64_t maxMills)
{
    uint64_t currMs = GetTickCount();
    uint64_t maxMs = maxMills == K_INFINITE ? maxMills == K_INFINITE : currMs + maxMills;

    const int queueToProcess = _activeQueue;
    _activeQueue = (_activeQueue + 1) % EVENT_MANAGER_NUM_QUEUES;
    _queues[_activeQueue].clear();

    while (!_queues[queueToProcess].empty())
    {
        const auto event = _queues[queueToProcess].front();
        _queues[queueToProcess].pop_front();

        if (const auto found = _eventListeners.find(event->GetEventType()); found != _eventListeners.end())
        {
            auto listeners = found->second;
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                auto listener = *it;
                listener(event);
            }
        }

        currMs = GetTickCount();
        if (maxMills != K_INFINITE && currMs >= maxMs)
        {
            break;
        }
    }

    const bool queueFlushed = _queues[queueToProcess].empty();
    if (!queueFlushed)
    {
        while (!_queues[queueToProcess].empty())
        {
            auto event = _queues[queueToProcess].back();
            _queues[queueToProcess].pop_back();
            _queues[_activeQueue].push_front(event);
        }
    }
    return queueFlushed;
}
