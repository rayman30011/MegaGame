#pragma once

#include "../ActorComponent.h"

class SampleComponent : public ActorComponent 
{
public:
	static const string Name;

	static ActorComponent* Create();
	
	virtual string GetName() const override { return Name; }
	virtual uint64_t GetId() const override { return 1; }
	
	virtual bool SampleComponent::Init(const xml::XMLElement* el) override;
};