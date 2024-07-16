#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "Base.hpp"
#include "String.hpp"

//----------------------------------------------------------------------------//
// Types
//----------------------------------------------------------------------------//

//!
template <class T> class SharedPtr;

//----------------------------------------------------------------------------//
// RTTI
//----------------------------------------------------------------------------//

//!
#define RTTI(C) \
	static const std::string& StaticClass(void) { static const std::string _cls(#C); return _cls; } \
	virtual bool IsClass(const std::string& _cls) const { return _cls == StaticClass() || __super::IsClass(_cls); } \
	virtual const std::string& Class(void) const { return StaticClass(); } \
	template <class T> bool IsClass(void) const { return IsClass(T::StaticClass()); } \
	static SharedPtr<Object> Create(void) { return new C; }

//----------------------------------------------------------------------------//
// Object
//----------------------------------------------------------------------------//

//!
class Object : public NonCopyable
{
public:
	//!
	typedef SharedPtr<Object>(*FactoryFunc)(void);
	//!
	static const std::string& StaticClass(void) { static const std::string _cls("Object"); return _cls; }
	//!
	virtual bool IsClass(const std::string& _cls) const { return _cls == StaticClass(); }
	//!
	virtual const std::string& Class(void) const { return StaticClass(); }
	//!
	template <class T> bool IsClass(void) const { return IsClass(T::StaticClass()); }


	//!
	class WeakReference final : public NonCopyable
	{
	public:
		friend class Object;

		//!
		void AddRef(void) const { ++m_refCount; }
		//!
		void Release(void) { if (--m_refCount == 0) delete this; }

		//!
		Object* GetPtr(void) { return m_ptr; }

	private:
		//!
		WeakReference(Object* _ptr) : m_ptr(_ptr) { }
		//!
		~WeakReference(void) { ASSERT(m_ptr == nullptr && m_refCount == 0); }

		//!
		void _Reset(void) { m_ptr = nullptr; Release(); }

		//!
		Object* m_ptr;
		//!
		mutable int m_refCount = 1;
	};


	//!
	Object(void) = default;
	//!
	virtual ~Object(void) { ASSERT(m_weakRef == nullptr && m_refCount == 0); }

	//!
	void AddRef(void) const { ++m_refCount; }
	//!
	void Release(void) { if (--m_refCount == 0) _DeleteThis(); }

	//!
	WeakReference* GetWeakRef(void) { return m_weakRef; }

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
	mutable int m_refCount = 0;
	//!
	WeakReference* m_weakRef = nullptr;
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
		std::swap(_ptr.m_ptr, m_ptr);
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
		m_ref = _ptr ? const_cast<T*>(_ptr)->GetWeakRef() : nullptr;
		return *this;
	}

	//!
	operator SharedPtr<T>(void) const { return Get(); }

	//!
	T* Get(void) const { return m_ref ? static_cast<T*>(m_ref->GetPtr()) : nullptr; }

protected:
	//!
	SharedPtr<Object::WeakReference> m_ref;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
