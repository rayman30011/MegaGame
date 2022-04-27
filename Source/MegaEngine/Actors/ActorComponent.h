#pragma once
#include <MegaEngineStd.h>


class Actor;

class ActorComponent
{
	friend class ActorFactory;

public:
	virtual ~ActorComponent();

	shared_ptr<Actor> GetOwner() { return _owner; }

	virtual bool Init(const xml::XMLElement* el);
	virtual void Inited() {};
	virtual void Update(int delta) {};

	virtual string GetName() const = 0;
	virtual uint64_t GetId() const = 0;

private:
	void SetOwner(shared_ptr<Actor> owner) { _owner = owner; }

protected:


private:
	shared_ptr<Actor> _owner;
};