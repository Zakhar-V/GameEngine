#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "System.hpp"

//---------------------------------------------------------------------------//
// Factory
//---------------------------------------------------------------------------//

//!
#define gFactory Factory::Get()

//!
class Factory : public System, public Singleton<Factory>
{
public:
	RTTI(Factory);

	//!
	void Register(const std::string _class, Object::FactoryFunc _f);
	//!
	template <class T> void Register(void) { Register(T::StaticClass(), T::Create); }
	//!
	SharedPtr<Object> Create(const std::string _class);
	//!
	template <class T> SharedPtr<T> Create(void) { return Create(T::StaticClass()).Cast<T>(); }
	//!
	template <class T> SharedPtr<T> Create(const std::string& _class)
	{
		auto _obj = Create(_class);
		return (_obj && _obj->IsClass<T>()) ? _obj.Cast<T>() : nullptr;
	}

protected:
	//!
	bool _Startup(void) override;
	//!
	void _Shutdown(void) override;

	//!
	std::unordered_map<std::string, Object::FactoryFunc> m_factory;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
