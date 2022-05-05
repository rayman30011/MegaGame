#pragma once

#include "../ActorComponent.h"

class SampleComponent : public ActorComponent 
{
public:
	static const string Name;

	static ActorComponent* Create();
	
	string GetName() const override { return Name; }
	uint64_t GetId() const override { return 1; }
	
	bool Init(const xml::XMLElement* el) override;
	void Update(int delta) override;
	
private:
	string _title;
};