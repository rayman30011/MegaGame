#pragma once

#include <strstream>
#include "../ThirdParty/delegates/delegate.h"

class IEventData;

typedef unsigned long EventType;

typedef delegates::delegate<void, shared_ptr<IEventData>> EventListenerDelegate;

class IEventData
{
public:
    virtual const EventType& GetEventType() const = 0;
    virtual float GetTimeStamp() const = 0;
    virtual void Serialize(std::ostrstream& out) const = 0;
    virtual shared_ptr<IEventData> Copy() const = 0;
    virtual const char* GetName() const = 0;
};

class BaseEventData: public IEventData
{
public:
    explicit BaseEventData(const float timeStamp = 0.f): _timeStamp(timeStamp){}
    virtual ~BaseEventData() {}

    const EventType& GetEventType(void) const override = 0;
    float GetTimeStamp() const override { return _timeStamp; }
    void Serialize(std::ostrstream& out) const override {}
private:
    const float _timeStamp;
};


class IEventManager
{
public:
    enum Constants { K_INFINITE = 0xffffffff };

    explicit IEventManager(const char* name, bool setAsGlobal);
    virtual ~IEventManager();

    virtual bool AddListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type) = 0;
    virtual bool RemoveListener(const EventListenerDelegate& eventListenerDelegate, const EventType& type) = 0;
    virtual bool TriggerEvent(const shared_ptr<IEventData>& event) = 0;
    virtual bool QueueEvent(const shared_ptr<IEventData>& event) = 0;
    virtual bool AbortEvent(const EventType& type, bool allOfType = false) = 0;
    virtual bool TickUpdate(uint64_t maxMills = K_INFINITE) = 0;

    static IEventManager* Get();
};