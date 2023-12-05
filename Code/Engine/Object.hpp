#pragma once

#include "String.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//

	template <class T> class SharedPtr;

	//----------------------------------------------------------------------------//
	// Reflection utils
	//----------------------------------------------------------------------------//
	
	//!
#define RTTI_FACTORY(T) static SharedPtr<Object> Factory(void) { return new T; }

	//!
#define RTTI_ABSTRACT(T) \
	static uint ClassIdStatic(void) { static const NameHash _clsid(ClassNameStatic()); return _clsid; } \
	static const String& ClassNameStatic(void) { static const String _clsname(#T); return _clsname; } \
	uint ClassId(void) override { return ClassIdStatic(); } \
	const String& ClassName(void) override  { return ClassNameStatic(); } \
	bool IsClass(const NameHash& _id) override { return _id.hash == ClassIdStatic() || __super::IsClass(_id); } \
	template <class X> bool IsClass(void) { return IsClass(X::ClassIdStatic()); } \

	//!
#define RTTI(T) RTTI_ABSTRACT(T) RTTI_FACTORY(T)

	//----------------------------------------------------------------------------//
	// Object
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Object : public NonCopyable
	{
	public:
		//!
		class ENGINE_API WeakReference final : public NonCopyable
		{
		public:
			friend class Object;

			//!
			void AddRef(void) { ++m_refCount; }
			//!
			void Release(void)
			{
				if(--m_refCount == 0)
					delete this;
			}

			//!
			Object* GetPtr(void) { return m_ptr; }

		private:
			//!
			WeakReference(Object* _ptr) : m_ptr(_ptr) { }
			//!
			~WeakReference(void)
			{
				ASSERT(m_ptr == nullptr && m_refCount == 0, "Incorrect deletion");
			}

			//!
			void _Reset(void)
			{
				m_ptr = nullptr;
				Release();
			}

			//!
			Object* m_ptr;
			//!
			int m_refCount = 1;
		};


		//!
		Object(void) = default;
		//!
		virtual ~Object(void)
		{
			ASSERT(m_weakRef == nullptr && m_refCount == 0, "Incorrect deletion");
		}

		//!
		void AddRef(void) const { ++m_refCount; }
		//!
		void Release(void)
		{
			if(--m_refCount == 0)
				_DeleteThis();
		}

		//!
		WeakReference* GetWeakRef(void) { return m_weakRef; }


		//!
		static uint ClassIdStatic(void) { static const NameHash _clsid(ClassNameStatic()); return _clsid; }
		//!
		static const String& ClassNameStatic(void) { static const String _clsname("Object"); return _clsname; }
		//!
		virtual uint ClassId(void) { return ClassIdStatic(); }
		//!
		virtual const String& ClassName(void) { return ClassNameStatic(); }
		//!
		virtual bool IsClass(const NameHash& _id) { return _id.hash == ClassIdStatic(); }
		//!
		template <class T> bool IsClass(void) { return IsClass(T::ClassIdStatic()); }

		//!
		typedef SharedPtr<Object>(*FactoryFunc)(void);
		//!
		static void Register(const String& _type, FactoryFunc _factory);
		//!
		static SharedPtr<Object> Create(const NameHash& _type);
		//!

	protected:
		//!
		void _ResetRef(void)
		{
			if (m_weakRef)
			{
				m_weakRef->_Reset();
				m_weakRef = nullptr;
			}
		}
		//!
		virtual void _DeleteThis(void)
		{
			_ResetRef();
			delete this;
		}

	private:
		//!
		WeakReference* m_weakRef = nullptr;
		//!
		mutable int m_refCount = 0;

		//!
		static HashMap<NameHash, FactoryFunc> s_factories;
	};

	//----------------------------------------------------------------------------//
	// SharedPtr
	//----------------------------------------------------------------------------//

	//!
	enum NoAddRef_t { NoAddRef };

	//!
	template <class T> class SharedPtr
	{
	public:
		//!
		SharedPtr(void) = default;
		//!
		~SharedPtr(void)
		{
			if (m_ptr)
				m_ptr->Release();
		}
		//!
		SharedPtr(const SharedPtr& _ptr) : SharedPtr(_ptr.m_ptr) { }
		//!
		SharedPtr(SharedPtr&& _ptr) noexcept : m_ptr(_ptr.m_ptr)
		{
			_ptr.m_ptr = nullptr;
		}
		//!
		SharedPtr(const T* _ptr) : m_ptr(const_cast<T*>(_ptr))
		{
			if (m_ptr)
				m_ptr->AddRef();
		}
		//!
		SharedPtr(const T* _ptr, NoAddRef_t) : m_ptr(const_cast<T*>(_ptr)) { }

		//!
		SharedPtr& operator = (const T* _ptr)
		{
			if (_ptr)
				_ptr->AddRef();
			if (m_ptr)
				m_ptr->Release();
			m_ptr = const_cast<T*>(_ptr);
			return *this;
		}
		//!
		SharedPtr& operator = (const SharedPtr& _ptr)
		{
			return *this = _ptr.m_ptr;
		}
		//!
		SharedPtr& operator = (SharedPtr&& _ptr) noexcept
		{
			Swap(_ptr.m_ptr, m_ptr);
			return *this;
		}

		//!
		operator T* (void) const { return const_cast<T*>(m_ptr); }
		//!
		T* operator -> (void) const { return const_cast<T*>(m_ptr); }
		//!
		T& operator * (void) const { return *const_cast<T*>(m_ptr); }

		//!
		T* Get(void) const { return const_cast<T*>(m_ptr); }
		//!
		template <class X> X* Cast(void) const { return static_cast<X*>(const_cast<T*>(m_ptr)); }

	protected:
		//!
		T* m_ptr = nullptr;
	};

	//----------------------------------------------------------------------------//
	// WeakRef
	//----------------------------------------------------------------------------//

	//!
	template <class T> class WeakRef
	{
	public:
		//!
		WeakRef(void) = default;
		//!
		~WeakRef(void) = default;
		//!
		WeakRef(const WeakRef& _ref) : m_ref(_ref.m_ref) { }
		//!
		WeakRef(WeakRef&& _ref) : m_ref(Move(_ref.m_ref)) { }
		//!
		WeakRef(const SharedPtr<T>& _ptr) : WeakRef(_ptr.Get()) { }
		//!
		WeakRef(const T* _ptr) : m_ref(_ptr ? const_cast<T*>(_ptr)->GetweakRef() : nullptr) { }

		//!
		WeakRef& operator = (const WeakRef& _ref)
		{
			m_ref = _ref.m_ref;
			return *this;
		}
		//!
		WeakRef& operator = (WeakRef&& _ref)
		{
			Swap(m_ref, _ref.m_ref);
			return *this;
		}
		//!
		WeakRef& operator = (const SharedPtr<T>& _ptr) { return *this = _ptr.Get(); }
		//!
		WeakRef& operator = (const T* _ptr)
		{
			m_ref = _ptr ? const_cast<T*>(_ptr)->GetweakRef() : nullptr;
			return *this;
		}

		//!
		operator SharedPtr<T>(void) const { return Get(); }

		//!
		T* Get(void) const { return m_ref ? m_ref->GetPtr() : nullptr; }

	protected:
		//!
		SharedPtr<Object::WeakReference> m_ref;
	};

	//----------------------------------------------------------------------------//
	// Factory utils
	//----------------------------------------------------------------------------//

	//!
	template <class T> void RegisterObject()
	{
		Object::Register(T::ClassNameStatic(), T::Factory);
	}
	//!
	template <class T> SharedPtr<T> CreateObject(const NameHash& _type)
	{
		SharedPtr<Object> _obj = Object::Create(_type);
		if (_obj)
		{
			if (_obj->IsClass(T::ClassIdStatic()))
				return _obj.Cast<T>();
			LOG_ERROR("Unable to create object of type(0x%x) as %s", _type.hash, T::ClassNameStatic().CStr());
		}
		
		return nullptr;
	}
	//!
	template <class T> SharedPtr<T> CreateObject(void)
	{
		return Object::Create(T::ClassIdStatic()).Cast<T>();
	}

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}

