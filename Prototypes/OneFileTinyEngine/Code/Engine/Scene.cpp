#include "Scene.hpp"

namespace Tiny
{

	//----------------------------------------------------------------------------//
	// Entity
	//----------------------------------------------------------------------------//
	
	//----------------------------------------------------------------------------//
	Entity::Entity(void)
	{
	}
	//----------------------------------------------------------------------------//
	Entity::~Entity(void)
	{
		ASSERT(m_inScene == false);

		while (m_child)
			m_child->SetParent(nullptr, false);

		while (m_component)
			_Unlink(m_component);
	}
	//----------------------------------------------------------------------------//
	Component* Entity::GetComponent(uint _type)
	{
		for (Component* i = m_component; i != nullptr; i = i->m_next)
		{
			if (i->ClassId() == _type)
				return i;
		}
		return nullptr;
	}
	//----------------------------------------------------------------------------//
	Component* Entity::AddComponent(uint _type)
	{
		Component* _exists = GetComponent(_type);
		if (_exists && _exists->IsUnique())
			return _exists;

		ComponentRef _component = Create(_type).Cast<Component>();
		_Link(_component);

		if (m_inScene)
		{
			_component->m_inScene = true;
			_component->_AddToScene();
		}

		return _component;
	}
	//----------------------------------------------------------------------------//
	void Entity::RemoveComponent(Component* _component)
	{
		if (_component && _component->m_entity == this && !_component->IsPermanent())
		{
			if (m_inScene)
			{
				_component->_RemoveFromScene();
				_component->m_inScene = false;
			}

			_Unlink(_component);
		}
	}
	//----------------------------------------------------------------------------//
	void Entity::SetName(const std::string& _name)
	{
		m_name = _name;
	}
	//----------------------------------------------------------------------------//
	void Entity::SetParent(Entity* _parent, bool _keepWorldTransform)
	{
		if (m_parent == _parent)
			return;

		for (Entity* i = _parent; i != nullptr; i = i->m_parent)
		{
			if (i == this)
				return;	// child entity cannot be parent
		}

		if (!m_parent && !m_inScene)
			AddRef();
		
		mat4 _worldTM;
		//if(_keepWorldTransform)
		//_worldTM = GetWorldTransform();
		
		if (m_parent)
			Unlink(m_parent->m_child, this, m_prev);

		m_parent = _parent;

		if (m_parent)
			Link(m_parent->m_child, this, m_prev);

		if (m_inScene != (_parent && _parent->m_inScene))
		{
			if (m_inScene)
				_RemoveFromScene();
			else
				_AddToScene();
		}

		//if(_keepWorldTransform)
		//SetWorldTransform(_worldTM);

		if (!m_parent && !m_inScene)
			Release();
	}
	//----------------------------------------------------------------------------//
	void Entity::Attach(Entity* _entity, bool _keepWorldTransform)
	{
		if (_entity)
			_entity->SetParent(this, _keepWorldTransform);
	}
	//----------------------------------------------------------------------------//
	void Entity::Detach(Entity* _entity, bool _keepWorldTransform)
	{
		if (_entity && _entity->m_parent == this)
			_entity->SetParent(nullptr, _keepWorldTransform);
	}
	//----------------------------------------------------------------------------//
	Entity* Entity::CreateChild(const std::string& _name)
	{
		EntityRef _child = new Entity;

		_child->SetName(_name);
		_child->SetParent(this);

		return _child;
	}
	//----------------------------------------------------------------------------//
	void Entity::_Link(Component* _component)
	{
		_component->AddRef();
		_component->m_entity = this;

		Link(m_component, _component, _component->m_prev);
	}
	//----------------------------------------------------------------------------//
	void Entity::_Unlink(Component* _component)
	{
		Unlink(m_component, _component, _component->m_prev);

		_component->m_entity = nullptr;
		_component->Release();
	}
	//----------------------------------------------------------------------------//
	void Entity::_AddToScene(void)
	{
		m_inScene = true;

		for (auto i = m_component; i != nullptr; i = i->m_next)
			i->_AddToScene();

		for (auto i = m_child; i != nullptr; i = i->m_next)
			i->_AddToScene();
	}
	//----------------------------------------------------------------------------//
	void Entity::_RemoveFromScene(void)
	{
		for (auto i = m_child; i != nullptr; i = i->m_next)
			i->_RemoveFromScene();

		for (auto i = m_component; i != nullptr; i = i->m_next)
			i->_RemoveFromScene();

		m_inScene = false;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// SceneManager
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	SceneManager::SceneManager(void)
	{

	}
	//----------------------------------------------------------------------------//
	SceneManager::~SceneManager(void)
	{
		m_subsystems.clear();
	}
	//----------------------------------------------------------------------------//
	bool SceneManager::Startup(void)
	{
		LOG("Startup: SceneManager");

		m_root = new Entity;
		m_root->AddRef();
		m_root->_AddToScene();

		m_permanent = m_root->CreateChild("Permanent");
		m_scene = m_root->CreateChild("Scene");

		return true;
	}
	//----------------------------------------------------------------------------//
	void SceneManager::Shutdown(void)
	{
		LOG("Shutdown: SceneManager");

		// todo: unload all objects

		//m_root->DetachAll();

		m_root->_RemoveFromScene();
		m_root->Release();

		m_root = nullptr;
		m_permanent = nullptr;
		m_scene = nullptr;
	}
	//----------------------------------------------------------------------------//
	bool SceneManager::OnEvent(int _id, void* _arg)
	{
		return false;
	}
	//----------------------------------------------------------------------------//
	void SceneManager::AddSubsystem(SceneSubsystemRef _system)
	{
		m_subsystems.push_back(_system);
		LOG("Add Scene subsystem : %s", _system->ClassName().c_str());

	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
