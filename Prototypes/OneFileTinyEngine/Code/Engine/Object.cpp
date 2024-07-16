#include "Object.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Object
	//---------------------------------------------------------------------------//

	std::unordered_map<uint, Object::FactoryFunc> Object::s_factory;

	//---------------------------------------------------------------------------//
	Object::WeakRef* Object::GetWeakRef(void)
	{
		if (!m_weakRef)
			m_weakRef = new WeakRef(this);
		return m_weakRef;
	}
	//---------------------------------------------------------------------------//
	void Object::_ResetRef(void)
	{
		if (m_weakRef)
		{
			m_weakRef->_Reset();
			m_weakRef = nullptr;
		}
	}
	//---------------------------------------------------------------------------//
	void Object::_DeleteThis(void)
	{
		_ResetRef();
		delete this;
	}
	//---------------------------------------------------------------------------//
	void Object::Register(const std::string& _type, FactoryFunc _factory)
	{
		LOG("Register class \"%s\" (ID 0x%08x)", _type.c_str(), Hash(_type));
		s_factory[ConstHash(_type.c_str())] = _factory;
	}
	//---------------------------------------------------------------------------//
	ObjectRef Object::Create(uint _type)
	{
		auto _fi = s_factory.find(_type);
		if (_fi != s_factory.end() && _fi->second)
			return _fi->second();

		LOG("ERROR: Unable to create an istance of class 0x%08x", _type);
		return nullptr;
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
