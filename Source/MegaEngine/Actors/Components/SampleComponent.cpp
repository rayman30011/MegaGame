#include "MegaEngineStd.h"

#include "SampleComponent.h"

#include <iostream>

const string SampleComponent::Name = "SampleComponent";

ActorComponent* SampleComponent::Create()
{
    return _NEW SampleComponent();
}

bool SampleComponent::Init(const xml::XMLElement* el)
{
    _title = el->Attribute("Title");
    return true;
}

void SampleComponent::Update(int delta)
{
    std::cout << "[SampleComponent]: " + _title << std::endl;
}
