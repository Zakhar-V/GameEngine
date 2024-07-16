#include "Scene.hpp"
#include "Factory.hpp"

//---------------------------------------------------------------------------//
// Entity
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
Entity::Entity(void)
{
}
//---------------------------------------------------------------------------//
Entity::~Entity(void)
{
	ASSERT(m_scene == nullptr);
	ASSERT(m_parent == nullptr);

	while (m_component)
	{
		ASSERT(m_component->m_scene == nullptr);

		Component* _c = m_component;
		_c->_OnRemoveFromEntity();
		List::Unlink(m_component, _c, _c->m_prev);
		_c->m_entity = nullptr;
		_c->Release();
	};

	while (m_child)
	{
		m_child->m_parent = nullptr;
		Entity* _c = m_child;
		List::Unlink(m_child, _c, _c->m_prev);
		_c->Release();
	}
}
//---------------------------------------------------------------------------//
void Entity::SetScene(Scene* _scene)
{
	if (m_scene == this)
		return;	// scene

	if (m_scene == _scene)
		return;

	if (m_parent && m_parent->m_scene != _scene)
		return;

	m_scene = _scene;

	for (Component* i = m_component; i; i = i->m_next)
	{
		i->_SetScene(m_scene);
	}

	for (Entity* i = m_child; i; i = i->m_next)
	{
		i->SetScene(m_scene);
	}
}
//---------------------------------------------------------------------------//
void Entity::SetParent(Entity* _parent)
{
	if (m_parent == _parent)
		return;

	if (_parent == this)
		return;

	if (IsChild(_parent))
		return;	 // child cannot be parent

	if (!m_parent)
		AddRef();

	Matrix44 _worldTM = GetTransform(); // get current world transform

	if (m_parent)
	{
		List::Unlink(m_parent->m_child, this, m_prev);
	}

	m_parent = _parent;

	if (m_parent)
	{
		List::Link(m_parent->m_child, this, m_prev);

		SetScene(m_parent->m_scene);
	}
	else
		SetScene(nullptr);


	SetTransform(_worldTM); // keep world transform

	if (!m_parent)
		Release();
}
//---------------------------------------------------------------------------//
void Entity::AddChild(Entity* _child)
{
	if (_child)
		_child->SetParent(this);
}
//---------------------------------------------------------------------------//
void Entity::RemoveChild(Entity* _child)
{
	if (_child && _child->m_parent == this)
		_child->SetParent(nullptr);
}
//---------------------------------------------------------------------------//
bool Entity::IsParent(Entity* _parent)
{
	for (Entity* i = this->m_parent; i != nullptr; i = i->m_parent)
	{
		if (i == _parent)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------//
bool Entity::IsChild(Entity* _child)
{
	return _child && _child->IsParent(this);
}
//---------------------------------------------------------------------------//
Component* Entity::GetComponent(const std::string& _class)
{
	for (Component* i = m_component; i != nullptr; i = i->GetNext())
	{
		if (i->IsClass(_class))
			return i;
	}
	return nullptr;
}
//---------------------------------------------------------------------------//
Component* Entity::AddComponent(const std::string& _class)
{
	SharedPtr<Component> _component = gFactory->Create<Component>(_class);

	if (_component)
	{
		_component->AddRef();

		List::Link(m_component, _component.Get(), _component->m_prev);

		_component->m_entity = this;
		_component->_SetScene(m_scene);
		_component->_OnAddToEntity();
	}
	else
		LOG("ERROR: Unable to create component \'%s\'", _class.c_str());

	return _component;
}
//---------------------------------------------------------------------------//
Component* Entity::GetOrAddComponent(const std::string& _class)
{
	Component* _component = GetComponent(_class);
	if (!_component)
		_component = AddComponent(_class);
	return _component;
}
//---------------------------------------------------------------------------//
void Entity::RemoveComponent(Component* _component)
{
	if (_component && _component->m_entity == this)
	{
		List::Unlink(m_component, _component, _component->m_prev);
		_component->_OnRemoveFromEntity();
		_component->m_entity = nullptr;
		_component->_SetScene(nullptr);
		_component->Release();
	}
}
//---------------------------------------------------------------------------//
Vector3 Entity::LocalToWorld(const Vector3& _v)
{
	return GetTransform().Transform(_v);
}
//---------------------------------------------------------------------------//
Vector3 Entity::WorldToLocal(const Vector3& _v)
{
	return GetTransform().Copy().Inverse().Transform(_v);
}
//---------------------------------------------------------------------------//
const Matrix44& Entity::GetTransform(void)
{
	_UpdateTransform();
	return m_worldTransform;
}
//---------------------------------------------------------------------------//
void Entity::SetTransform(const Matrix44& _tm)
{
	SetPosition(_tm.GetTranslation());
	SetRotation(_tm.GetRotation());
	SetScale(_tm.GetScale());
}
//---------------------------------------------------------------------------//
const Matrix44 Entity::GetLocalTransform(void)
{
	return Matrix44().CreateTransform(m_localPosition, m_localRotation, m_localScale);
}
//---------------------------------------------------------------------------//
void Entity::SetLocalTransform(const Matrix44& _tm)
{
	SetLocalPosition(_tm.GetTranslation());
	SetLocalRotation(_tm.GetRotation());
	SetLocalScale(_tm.GetScale());
}
//---------------------------------------------------------------------------//
void Entity::SetLocalPosition(const Vector3& _position)
{
	m_localPosition = _position;
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
const Vector3& Entity::GetLocalPosition(void)
{
	return m_localPosition;
}
//---------------------------------------------------------------------------//
void Entity::SetLocalRotation(const Quaternion& _rotation)
{
	m_localRotation = _rotation;
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
const Quaternion& Entity::GetLocalRotation(void)
{
	return m_localRotation;
}
//---------------------------------------------------------------------------//
void Entity::SetLocalScale(const Vector3& _scale)
{
	m_localScale = _scale;
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
const Vector3& Entity::GetLocalScale(void)
{
	return m_localScale;
}
//---------------------------------------------------------------------------//
void Entity::SetPosition(const Vector3& _position)
{
	m_localPosition = _position;
	if (m_inheritPosition && m_parent)
		m_localPosition = m_parent->GetTransform().Copy().Inverse().Transform(m_localPosition);
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
Vector3 Entity::GetPosition(void)
{
	_UpdateTransform();
	return m_worldTransform.GetTranslation();
}
//---------------------------------------------------------------------------//
void Entity::SetRotation(const Quaternion& _rotation)
{
	m_localRotation = _rotation;
	if (m_inheritRotation && m_parent)
		m_localRotation = m_parent->GetRotation().Copy().Inverse() * m_localRotation;
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
const Quaternion& Entity::GetRotation(void)
{
	_UpdateTransform();
	return m_worldRotation;
}
//---------------------------------------------------------------------------//
void Entity::SetScale(const Vector3& _scale)
{
	m_localScale = _scale;
	if (m_inheritScale && m_parent)
		m_localScale /= m_parent->GetTransform().GetScale();
	_InvalidateTransform();
}
//---------------------------------------------------------------------------//
Vector3 Entity::GetScale(void)
{
	_UpdateTransform();
	return m_worldTransform.GetScale();
}
//---------------------------------------------------------------------------//
Vector3 Entity::GetDirection(void)
{
	return -Vector3::UnitZ * GetRotation();
}
//---------------------------------------------------------------------------//
void Entity::SetDirection(const Vector3& _dir)
{
	SetRotation(Quaternion().FromRotationTo(-Vector3::UnitZ, _dir));
}
//---------------------------------------------------------------------------//
void Entity::SetInheritPosition(bool _enabled)
{
	if (m_inheritPosition != _enabled)
	{
		Vector3 _pos = GetPosition();
		m_inheritPosition = _enabled;
		SetPosition(_pos);
	}
}
//---------------------------------------------------------------------------//
void Entity::SetInheritRotation(bool _enabled)
{
	if (m_inheritRotation != _enabled)
	{
		Quaternion _rot = GetRotation();
		m_inheritRotation = _enabled;
		SetRotation(_rot);
	}
}
//---------------------------------------------------------------------------//
void Entity::SetInheritScale(bool _enabled)
{
	if (m_inheritScale != _enabled)
	{
		Vector3 _scl = GetScale();
		m_inheritScale = _enabled;
		SetScale(_scl);
	}
}
//---------------------------------------------------------------------------//
void Entity::_InvalidateTransform(void)
{
	if (m_transformUpdated)
	{
		m_transformUpdated = false;
		m_transformChanged = true;
		for (Entity* i = m_child; i; i = i->m_next)
			i->_InvalidateTransform();
	}
}
//---------------------------------------------------------------------------//
void Entity::_UpdateTransform(void)
{
	if (!m_transformUpdated)
	{
		Vector3 _pos = m_localPosition;
		Quaternion _rot = m_localRotation;
		Vector3 _scl = m_localScale;

		if (m_parent)
		{
			if (m_inheritPosition)
				_pos = m_parent->GetTransform().Transform(_pos);
			if (m_inheritRotation)
				_rot = m_parent->GetRotation() * _rot;
			if (m_inheritScale)
				_scl *= m_parent->GetScale();
		}

		m_worldTransform.CreateTransform(_pos, _rot, _scl);
		m_worldRotation = m_worldTransform.GetRotation();
		m_transformUpdated = true;
	}
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Component
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
Component::Component(void)
{
}
//---------------------------------------------------------------------------//
Component::~Component(void)
{
}
//---------------------------------------------------------------------------//
void Component::_SetScene(Scene* _scene)
{				   
	if (m_scene != _scene)
	{
		if (m_scene)
			_RemoveFromScene();

		m_scene = _scene;

		if (m_scene)
			_AddToScene();
	}
}
//---------------------------------------------------------------------------//
Component* Component::GetNext(const std::string& _class)
{
	for (Component* i = m_next; i != nullptr; i = i->m_next)
	{
		if (i->IsClass(_class))
			return i;
	}
	return nullptr;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Scene
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
Scene::Scene(void)
{
	m_scene = this;
}
//---------------------------------------------------------------------------//
Scene::~Scene(void)
{
}
//---------------------------------------------------------------------------//
EventResult Scene::OnEvent(int _id, const Variant& _args)
{
	for (SceneSystem* i = GetComponent<SceneSystem>(); i != nullptr; i->GetNext<SceneSystem>())
	{
		EventResult _r = i->OnEvent(_id, _args);
		if (_r != EventResult::Pass)
			return _r;
	}
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
