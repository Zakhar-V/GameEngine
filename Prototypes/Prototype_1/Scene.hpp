#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Resource.hpp"
#include "Math.hpp"

//---------------------------------------------------------------------------//
// Types 
//---------------------------------------------------------------------------//

class Scene;
class Component;

//---------------------------------------------------------------------------//
// Entity 
//---------------------------------------------------------------------------//

//!
class Entity : public Resource
{
public:
	RTTI(Entity);

	//!
	Entity(void);
	//!
	~Entity(void);

	// [SCENE]

	//!
	void SetScene(Scene* _scene);
	//!
	Scene* GetScene(void) { return m_scene; }

	// [HIERARCHY]

	//!
	void SetParent(Entity* _parent);
	//!
	void AddChild(Entity* _child);
	//!
	void RemoveChild(Entity* _child);

	//!
	bool IsParent(Entity* _parent);
	//!
	bool IsChild(Entity* _child);
	//!
	Entity* GetParent(void) { return m_parent; }
	//!
	Entity* GetChild(void) { return m_child; }
	//!
	Entity* GetNext(void) { return m_next; }

	// [COMPONENT]

	//!
	Component* GetComponent(void) { return m_component; }
	//!
	Component* GetComponent(const std::string& _class);
	//!
	Component* AddComponent(const std::string& _class);
	//!
	Component* GetOrAddComponent(const std::string& _class);
	//!
	template <class T> T* GetComponent(void) { return static_cast<T*>(GetComponent(T::StaticClass())); }
	//!
	template <class T> T* GetOrAddComponent(void) { return static_cast<T*>(GetOrAddComponent(T::StaticClass())); }

	void RemoveComponent(Component* _component);

	// [TRANSFORM]

	//!
	Vector3 LocalToWorld(const Vector3& _v);
	//!
	Vector3 WorldToLocal(const Vector3& _v);

	//!
	const Matrix44& GetTransform(void);
	//!
	void SetTransform(const Matrix44& _tm);
	//!
	const Matrix44 GetLocalTransform(void);
	//!
	void SetLocalTransform(const Matrix44& _tm);
	//!
	void SetLocalPosition(const Vector3& _position);
	//!
	const Vector3& GetLocalPosition(void);
	//!
	void SetLocalRotation(const Quaternion& _rotation);
	//!
	const Quaternion& GetLocalRotation(void);
	//!
	void SetLocalScale(const Vector3& _scale);
	//!
	const Vector3& GetLocalScale(void);
	//!
	void SetPosition(const Vector3& _position);
	//!
	Vector3 GetPosition(void);
	//!
	void SetRotation(const Quaternion& _rotation);
	//!
	const Quaternion& GetRotation(void);
	//!
	void SetScale(const Vector3& _scale);
	//!
	Vector3 GetScale(void);

	//!
	Vector3 GetDirection(void);
	//!
	void SetDirection(const Vector3& _dir);

	//!
	void SetInheritPosition(bool _enabled = true);
	//!
	bool GetInheritPosition(void) { return m_inheritPosition; }
	//!
	void SetInheritRotation(bool _enabled = true);
	//!
	bool GetInheritRotation(void) { return m_inheritRotation; }
	//!
	void SetInheritScale(bool _enabled = true);
	//!
	bool GetInheritScale(void) { return m_inheritScale; }

protected:
	friend class Component;

	//!
	void _InvalidateTransform(void);
	//!
	void _UpdateTransform(void);


	//!
	Scene* m_scene = nullptr;
	//!
	Entity* m_parent = nullptr;
	//!
	Entity* m_child = nullptr;
	//!
	Entity* m_prev = nullptr;
	//!
	Entity* m_next = nullptr;

	//!
	Vector3 m_localPosition = Vector3::Zero;
	//!
	Quaternion m_localRotation = Quaternion::Identity;
	//!
	Vector3 m_localScale = Vector3::One;
	//!
	Matrix44 m_worldTransform = Matrix44::Identity;
	//!
	Quaternion m_worldRotation = Quaternion::Identity;

	//!
	bool m_enabled = true;

	//!
	bool m_transformUpdated = true;
	//!
	bool m_transformChanged = false;

	//!
	bool m_inheritPosition = true;
	//!
	bool m_inheritRotation = true;
	//!
	bool m_inheritScale = true;

	//!
	Component* m_component = nullptr;
};

//---------------------------------------------------------------------------//
// Component 
//---------------------------------------------------------------------------//

//!	
class Component : public Resource
{
public:
	RTTI(Component);

	//!
	Component(void);
	//!
	~Component(void);

	//!
	virtual bool IsSingleComponent(void) { return false; }

	//!
	Scene* GetScene(void) { return m_scene; }

	//!
	Entity* GetEntity(void) { return m_entity; }
	//!
	Component* GetNext(void) { return m_next; }
	//!
	Component* GetNext(const std::string& _class);
	//!
	template <class T> T* GetNext(void) { return static_cast<T*>(GetNext(T::StaticClass())); }

private:
	friend class Entity;

	//! 
	void _SetScene(Scene* _scene);

protected:

	//! Добавить компонент в сцену. Вызывается при создании или добавлении сущности в сцену
	virtual void _AddToScene(void) { }
	//! Удалить компонент из сцены. Вызывается при удалении компонента или при удалении сущности из сцены
	virtual void _RemoveFromScene(void) { }
	//!
	virtual void _OnAddToEntity(void) { }
	//!
	virtual void _OnRemoveFromEntity(void) { }

	//!
	Scene* m_scene = nullptr;
	//!
	Entity* m_entity = nullptr;
	//!
	Component* m_prev = nullptr;
	//!
	Component* m_next = nullptr;
};

//---------------------------------------------------------------------------//
// SingleComponent 
//---------------------------------------------------------------------------//

class SingleComponent : public Component
{
public:

protected:
};

//---------------------------------------------------------------------------//
// SceneSystem 
//---------------------------------------------------------------------------//

//! Базовый класс для подсистем сцены, таких как PhysicsWorld, GraphicsWorld, GUI и тд
class SceneSystem : public Component
{
public:
	RTTI(SceneSystem);

	//!
	virtual EventResult OnEvent(int _id, const Variant& _args = Variant::Null) { return EventResult::Pass; }

};

//---------------------------------------------------------------------------//
// Scene 
//---------------------------------------------------------------------------//

//! Сцена
class Scene : public Entity
{
public:
	RTTI(Scene);

	//!
	Scene(void);
	//!
	~Scene(void);

	//!	Переслать событие подсистемам сцены. Вызывается из Level::OnEvent для текущей сцены
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null);

protected:

};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//

