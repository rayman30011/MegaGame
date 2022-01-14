#include <MegaEngineStd.h>

#include "Actor.h"

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

void Actor::Update()
{
}
