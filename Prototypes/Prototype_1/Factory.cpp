#include "Factory.hpp"

//---------------------------------------------------------------------------//
// Factory
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
void Factory::Register(const std::string _class, Object::FactoryFunc _f)
{
	if (_f)
	{
		LOG("Register class \'%s\'", _class.c_str());
		m_factory[_class] = _f;
	}
}
//---------------------------------------------------------------------------//
SharedPtr<Object> Factory::Create(const std::string _class)
{
	auto _factory = m_factory.find(_class);
	if (_factory != m_factory.end())
		return _factory->second();
	return nullptr;
}
//---------------------------------------------------------------------------//
bool Factory::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	return true;
}
//---------------------------------------------------------------------------//
void Factory::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);

	m_factory.clear();
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//