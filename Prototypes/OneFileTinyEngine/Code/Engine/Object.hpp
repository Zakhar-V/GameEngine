#pragma once

#include "Base.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Reference
	//---------------------------------------------------------------------------//

	//!
	template <class T> class Reference
	{
	public:

		//!
		Reference(void) = default;
		//!
		~Reference(void) { if (m_ptr) m_ptr->Release(); }
		//!
		Reference(const Reference& _ptr) : Reference(_ptr.m_ptr) { }
		//!
		Reference(Reference&& _ptr) noexcept : m_ptr(_ptr.m_ptr) { _ptr.m_ptr = nullptr; }
		//!
		Reference(const T* _ptr) : m_ptr(const_cast<T*>(_ptr)) { if (m_ptr) m_ptr->AddRef(); }

		//!
		Reference& operator = (const T* _ptr)
		{
			if (_ptr)
				_ptr->AddRef();
			if (m_ptr)
				m_ptr->Release();
			m_ptr = const_cast<T*>(_ptr);
			return *this;
		}
		//!
		Reference& operator = (const Reference& _ptr) { return *this = _ptr.m_ptr; }
		//!
		Reference& operator = (Reference&& _ptr) noexcept { std::swap(_ptr.m_ptr, m_ptr); return *this; }

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

	//---------------------------------------------------------------------------//
	// RTTI
	//---------------------------------------------------------------------------//

	//!
#	define RTTI_FACTORY(T) static ObjectRef Factory(void) { return new T; }

	//!
#	define RTTI_ABSTRACT(T) \
		static const std::string& ClassNameStatic(void) { static const std::string _clsname(#T); return _clsname; }	\
		static uint ClassIdStatic(void) { static const uint _clsid(Hash(ClassNameStatic())); return _clsid; } \
		uint ClassId(void) override { return ClassIdStatic(); } \
		const std::string& ClassName(void) override  { return ClassNameStatic(); } \
		bool IsClass(const char* _name) override { return _name == ClassNameStatic() || __super::IsClass(_name); } \
		bool IsClass(uint _id) override { return _id == ClassIdStatic() || __super::IsClass(_id); } \
		template <class X> bool IsClass(void) { return IsClass(X::ClassIdStatic()); } \

	//!
#	define RTTI(T) RTTI_ABSTRACT(T) RTTI_FACTORY(T)

	//---------------------------------------------------------------------------//
	// Object
	//---------------------------------------------------------------------------//

	//!
	typedef Reference<class Object> ObjectRef;

	//!
	class Object
	{
	public:

		//!
		class WeakRef final : public NonCopyable
		{
		public:
			friend class Object;

			//!
			void AddRef(void) { ++m_refCount; }
			//!
			void Release(void) { if (!--m_refCount) delete this; }

			//!
			Object* GetPtr(void) { return m_ptr; }

		private:
			//!
			WeakRef(Object* _ptr) : m_ptr(_ptr) { }
			//!
			~WeakRef(void) { ASSERT(m_ptr == nullptr && m_refCount == 0, "Incorrect deletion"); }
			//!
			void _Reset(void) { m_ptr = nullptr; Release(); }

			//!
			Object* m_ptr;
			//!
			int m_refCount = 1;
		};


		// [OBJECT]

		//!
		Object(void) = default;
		//!
		virtual ~Object(void) { ASSERT(m_weakRef == nullptr && m_refCount == 0, "Incorrect deletion"); }

		//!
		void AddRef(void) const { ++m_refCount; }
		//!
		void Release(void) { if (!--m_refCount) _DeleteThis(); }

		//!
		WeakRef* GetWeakRef(void);

		// [EVENT]

		//!
		virtual bool OnEvent(int _type, void* _arg = nullptr) { return false; }


		// [RTTI]

		//!
		static const std::string& ClassNameStatic(void) { static const std::string _clsname("Object"); return _clsname; }
		//!
		static uint ClassIdStatic(void) { static const uint _clsid(Hash(ClassNameStatic())); return _clsid; }

		//!
		virtual const std::string& ClassName(void) { return ClassNameStatic(); }
		//!
		virtual uint ClassId(void) { return ClassIdStatic(); }

		//
		bool IsClass(const std::string& _name) { return IsClass(_name.c_str()); }
		//!
		virtual bool IsClass(const char* _name) { return _name == ClassNameStatic(); }
		//!
		virtual bool IsClass(uint _id) { return _id == ClassIdStatic(); }
		//!
		template <class T> bool IsClass(void) { return IsClass(T::ClassIdStatic()); }

		// [FACTORY]

		//!
		typedef Reference<Object>(*FactoryFunc)(void);
		//!
		static void Register(const std::string& _type, FactoryFunc _factory);
		//!
		static ObjectRef Create(const std::string& _type) { return Create(Hash(_type)); }
		//!
		static ObjectRef Create(uint _type);

	protected:

		//!
		void _ResetRef(void);
		//!
		virtual void _DeleteThis(void);

		//!
		mutable int m_refCount = 0;
		//!
		WeakRef* m_weakRef = nullptr;

		//!
		static std::unordered_map<uint, FactoryFunc> s_factory;
	};

	//----------------------------------------------------------------------------//
	// WeakReference
	//----------------------------------------------------------------------------//

	//!
	template <class T> class WeakReference
	{
	public:
		//!
		WeakReference(void) = default;
		//!
		~WeakReference(void) = default;
		//!
		WeakReference(const WeakReference& _ref) : m_ref(_ref.m_ref) { }
		//!
		WeakReference(WeakReference&& _ref) : m_ref(std::move(_ref.m_ref)) { }
		//!
		WeakReference(const Reference<T>& _ptr) : WeakReference(_ptr.Get()) { }
		//!
		WeakReference(const T* _ptr) : m_ref(_ptr ? const_cast<T*>(_ptr)->GetWeakRef() : nullptr) { }

		//!
		WeakReference& operator = (const WeakReference& _ref) { m_ref = _ref.m_ref; return *this; }
		//!
		WeakReference& operator = (WeakReference&& _ref) { std::swap(m_ref, _ref.m_ref); return *this; }
		//!
		WeakReference& operator = (const Reference<T>& _ptr) { return *this = _ptr.Get(); }
		//!
		WeakReference& operator = (const T* _ptr) { m_ref = _ptr ? const_cast<T*>(_ptr)->GetweakRef() : nullptr; return *this; }

		//!
		operator Reference<T>(void) const { return Get(); }

		//!
		T* Get(void) const { return m_ref ? m_ref->GetPtr() : nullptr; }

	protected:
		//!
		Reference<Object::WeakRef> m_ref;
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
