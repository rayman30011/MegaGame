#pragma once

class ActorComponent;

class Actor
{
public:
	friend class ActorFactory;

	explicit Actor(uint64_t id);
	~Actor();

	uint64_t GetId() const { return _id; }

	bool Init(const xml::XMLElement* el);
	void PostInit();
	void Destroy();
	void Update(float delta) const;

	template<class TComponent>
	weak_ptr<TComponent> GetComponent(uint64_t compId)
	{
		const auto it = _components.find(compId);
		if (it != _components.end())
		{
			const std::shared_ptr base(it->second);
			shared_ptr<TComponent> sub(std::static_pointer_cast<TComponent>(base));
			weak_ptr<TComponent> weak(sub);
			
			return weak;
		}
		return weak_ptr<TComponent>();
	}

private:
	void AddComponent(shared_ptr<ActorComponent> component);

private:
	std::map<uint64_t, std::shared_ptr<ActorComponent>> _components;
	uint64_t _id;
};