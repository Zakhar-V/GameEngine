#include "Object.hpp"

namespace Engine
{

	//----------------------------------------------------------------------------//
	// Object
	//----------------------------------------------------------------------------//

	HashMap<NameHash, Object::FactoryFunc> Object::s_factories;

	//----------------------------------------------------------------------------//
	void Object::Register(const String& _type, FactoryFunc _factory)
	{
		s_factories[_type] = _factory;
	}
	//----------------------------------------------------------------------------//
	SharedPtr<Object> Object::Create(const NameHash& _type)
	{
		auto _f = s_factories.Find(_type);
		if (_f != s_factories.End())
			return _f->second();
		LOG_ERROR("Unable to create object of type(0x%x)", _type);
		return nullptr;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
