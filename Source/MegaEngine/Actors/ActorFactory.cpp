#include <MegaEngineStd.h>

#include "ActorFactory.h"
#include "Actor.h"
#include "ActorComponent.h"

ActorFactory::ActorFactory()
{
}

shared_ptr<Actor> ActorFactory::Create(const std::string& resource)
{
	xml::XMLDocument doc;
	const auto error = doc.LoadFile(resource.c_str());
	if (error != xml::XMLError::XML_SUCCESS)
	{
		_ERROR(TEXT("Resource are missing"));
		return shared_ptr<Actor>();
	}

	const auto root = doc.RootElement();

	shared_ptr<Actor> actor(_NEW Actor(GetNextActorId()));
	if (!actor->Init(root))
	{
		_ERROR(TEXT("FAILED Initialize"));
		return shared_ptr<Actor>();
	}

	for (auto node = root->FirstChildElement(); node; node = node->NextSiblingElement())
	{
		shared_ptr<ActorComponent> component(CreateComponent(node));
		if (component)
		{
			actor->AddComponent(component);
			component->SetOwner(actor);
		}
		else
		{
			return shared_ptr<Actor>();
		}
	}

	actor->PostInit();

	return actor;
}

shared_ptr<ActorComponent> ActorFactory::CreateComponent(xml::XMLElement* el)
{
	std::string name(el->Value());

	shared_ptr<ActorComponent> component;
	auto it = _createDelegateMap.find(name);
	if (it != _createDelegateMap.end())
	{
		const auto action = it->second;
		component.reset(action());
	}
	else
	{
		_ERROR(TEXT("Couldn't find ActorComponent"));
		return shared_ptr<ActorComponent>();
	}

	if (component)
	{
		if (!component->Init(el))
		{
			_ERROR(TEXT("Component failed to initialize"));
		}
	}

	return component;
}
