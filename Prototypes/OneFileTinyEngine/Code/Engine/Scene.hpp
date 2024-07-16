#pragma once

#include "System.hpp"
#include "Math.hpp"

/*
*SceneObject	- ������� ����� ��� �������� ����� (Component, Entity)
*Component - ������� ����� ��� �����������. ���������� �������������� � ��������������� ������� � ����������� ��.
*Entity - �������� (����� ������) - ��������� ��� �����������. ��������� ������� �������� ��������.
*SceneSubsystem - ������� ����� ��� ��������� ����� (�������, ������ � ��). ���������� ��������� ������������
*SceneManager - �������� �����, ��������� ������������ � ���������.
�������� ����� �������� ����������, ��� �� ��� � ��� ����������. 
�������� ����� ����� �������� ������, ������� ������ �������.
���������� �������� � ����� � �������� �� ��� �������������� ����� �������������/������������ �������� �� ��������� �������.
����� ������ ��������� ��������� �������(��������), ���������� � ������ ������������� ������� (��������� ���� � ��),
��������� �������� ����� ������ (��� ����� ������) � ��������� �������, � ��� ������������� ��������� �� � �����,
������������ �������� ������ � ��.


	ROOT - ��������� ������� �����
	{
		PERMANENT - ��������� ���������� �����, ������ ����������� ��� ����������� ��������
		{
			// permanent objects
		}

		SCENE - ��������� ���������� �����, ������ ����������� ��� �������� ������� �����
		{
			// current scene objects
		}
	}

*/

namespace Tiny
{
	//----------------------------------------------------------------------------//
	// SceneObject
	//----------------------------------------------------------------------------//

	//!
	class SceneObject : public Object
	{
	public:
		//!
		RTTI(SceneObject);


	protected:
		friend class Entity;

		//!
		virtual void _AddToScene(void) { }
		//!
		virtual void _RemoveFromScene(void) { }

		//!
		bool m_inScene = false;
	};

	//----------------------------------------------------------------------------//
	// Component 
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class Component> ComponentRef;

	//!
	class Component : public SceneObject
	{
	public:
		//!
		RTTI(Component);

		//!
		virtual bool IsPermanent(void) { return false; }
		//!
		virtual bool IsUnique(void) { return false; }

	protected:
		friend class Entity;

		//!
		class Entity* m_entity = nullptr;
		//!
		Component* m_prev = nullptr;
		//!
		Component* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// Entity
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class Entity> EntityRef;

	//!
	class Entity : public SceneObject
	{
	public:
		//!
		RTTI(Entity);

		//!
		Entity(void);
		//!
		~Entity(void);

		//!
		Component* GetComponent(uint _type);
		//!
		Component* AddComponent(uint _type);
		//!
		void RemoveComponent(Component* _component);


		//!
		void SetName(const std::string& _name);
		//!
		const std::string& GetName(void) { return m_name; }



		//!
		void SetParent(Entity* _parent, bool _keepWorldTransform = true);
		//!
		void Attach(Entity* _entity, bool _keepWorldTransform = true);
		//!
		void Detach(Entity* _entity, bool _keepWorldTransform = true);

		Entity* CreateChild(const std::string& _name = "");

	protected:
		friend class SceneManager;

		//!
		void _Link(Component* _component);
		//!
		void _Unlink(Component* _component);
		//!
		void _AddToScene(void) override;
		//!
		void _RemoveFromScene(void) override;

		//!
		std::string m_name;

		//!
		Entity* m_parent = nullptr;
		//!
		Entity* m_child = nullptr;
		//!
		Entity* m_prev = nullptr;
		//!
		Entity* m_next = nullptr;

		//!
		Component* m_component = nullptr;
	};

	//----------------------------------------------------------------------------//
	// SceneSubsystem
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class SceneSubsystem> SceneSubsystemRef;

	//!
	class SceneSubsystem : public System
	{
	public:
		//!
		RTTI(SceneSubsystem);

		//!
		virtual void AddComponent(Component* _component) { }
		//!
		virtual void RemoveComponent(Component* _component) { }

	protected:
	};

	//----------------------------------------------------------------------------//
	// SceneManager
	//----------------------------------------------------------------------------//

	//!
#	define gSceneManager Tiny::SceneManager::Get()
	//!
#	define gRoot gSceneManager->Root()
	//!
#	define gPermanent gSceneManager->Permanent()
	//!
#	define gScene gSceneManager->Scene()

	//!
	class SceneManager : public System, public Singleton<SceneManager>
	{
	public:
		//!
		RTTI(SceneManager);

		//!
		SceneManager(void);
		//!
		~SceneManager(void);


		//!
		bool Startup(void) override;
		//!
		void Shutdown(void) override;
		//!
		bool OnEvent(int _id, void* _arg = nullptr) override;


		//!
		void AddSubsystem(SceneSubsystemRef _system);

		//!
		Entity* Root(void) { return m_root; }
		//!
		Entity* Permanent(void) { return m_permanent; }
		//!
		Entity* Scene(void) { return m_scene; }

	protected:

		//!
		std::list<SceneSubsystemRef> m_subsystems;

		//!
		Entity* m_root = nullptr; 
		//!
		Entity* m_permanent = nullptr;
		//!
		Entity* m_scene = nullptr;
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
