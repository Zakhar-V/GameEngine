#include "Resource.hpp"
#include "Factory.hpp"

//---------------------------------------------------------------------------//
// ResourceManager
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
SharedPtr<Resource> ResourceManager::Load(const std::string& _class, const std::string& _name)
{
	auto _cache = m_cache.find(_class);
	if (_cache != m_cache.end())
	{
		auto _resource = _cache->second.find(_name);

		if (_resource != _cache->second.end())
			return _resource->second;
	}

	SharedPtr<Resource> _resource = gFactory->Create<Resource>(_class);
	if (_resource)
	{
		SharedPtr<File> _file = new File(_name);
		if (_file->IsOpened())
		{
			if (_resource->Load(_file))
			{
				m_cache[_class][_name] = _resource;
				_resource->SetName(_name);
				_resource->m_nameInCache = _name;
				return _resource;
			}
		}

		LOG("ERROR: Unable to load %s from \'%s\'", _class.c_str(), _name.c_str());
	}
	else
		LOG("ERROR: Unable to create resource \'%s\'", _class.c_str());

	return nullptr;
}
//---------------------------------------------------------------------------//
EventResult ResourceManager::OnEvent(int _id, const Variant& _args)
{
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool ResourceManager::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	return true;
}
//---------------------------------------------------------------------------//
void ResourceManager::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);

	m_cache.clear();
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
