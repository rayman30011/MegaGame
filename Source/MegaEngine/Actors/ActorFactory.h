#pragma once
#include <MegaEngineStd.h>

class ActorComponent;
class Actor;

typedef ActorComponent* (*CreateActorComponentDelegate)(void);

class ActorFactory
{
public:
	ActorFactory();
	shared_ptr<Actor> Create(const std::string& resource);

protected:
	template <typename TComponent>
	void RegisterComponent()
	{
		string id = TComponent::ID;
		_createDelegateMap[id] = []() -> ActorComponent*
		{
			return _NEW TComponent();
		};
	}

	shared_ptr<ActorComponent> CreateComponent(xml::XMLElement* el);

private:
	uint64_t GetNextActorId() { ++_lastActorId; return _lastActorId; }
	uint64_t _lastActorId = 0;

protected:
	std::map<std::string, CreateActorComponentDelegate> _createDelegateMap;
};