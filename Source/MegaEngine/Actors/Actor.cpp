#include <MegaEngineStd.h>

#include "Actor.h"

#include "ActorComponent.h"

Actor::Actor(uint64_t id) : _id(id)
{
}

Actor::~Actor()
{
}

bool Actor::Init(const xml::XMLElement* el)
{
	return true;
}

void Actor::AddComponent(shared_ptr<ActorComponent> component)
{
}

void Actor::PostInit()
{
}

void Actor::Destroy()
{
}

void Actor::Update(float delta) const
{
	for (auto component : _components)
	{
		component.second->Update(delta);
	}
}
