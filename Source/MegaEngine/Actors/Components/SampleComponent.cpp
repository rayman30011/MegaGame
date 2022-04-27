#include "MegaEngineStd.h"

#include "SampleComponent.h"

const string SampleComponent::Name = "SampleComponent";

ActorComponent* SampleComponent::Create()
{
    return _NEW SampleComponent();
}

bool SampleComponent::Init(const xml::XMLElement* el)
{
    return true;
}
