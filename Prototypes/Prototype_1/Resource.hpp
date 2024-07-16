#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "System.hpp"
#include "File.hpp"

//---------------------------------------------------------------------------//
// Resource
//---------------------------------------------------------------------------//

//!
class Resource : public Object
{
public:
	RTTI(Resource);

	//!
	void SetName(const std::string& _name) { m_name = _name; }
	//!
	const std::string& GetName(void) { return m_name; }
	//!
	const std::string& GetNameInCache(void) { return m_nameInCache; }

	//!
	virtual bool Load(File* _src) { return false; }
	//!
	virtual void Save(File* _dst) { }
	//!
	virtual bool Deserialize(const Variant& _src) { return false; }
	//!
	virtual void Serialize(Variant& _dst) { }

protected:
	friend class ResourceManager;

	std::string m_name, m_nameInCache;

};

//---------------------------------------------------------------------------//
// ResourceManager
//---------------------------------------------------------------------------//

#define gResourceManager ResourceManager::Get()

class ResourceManager : public System, public Singleton<ResourceManager>
{
public:
	RTTI(ResourceManager);

	//!
	typedef	std::unordered_map<std::string, SharedPtr<Resource>> Cache;

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;

	//!
	SharedPtr<Resource> Load(const std::string& _class, const std::string& _name);
	//!
	template <class T> SharedPtr<T> Load(const std::string& _name) { return Load(T::StaticClass(), _name).Cast<T>(); }

protected:
	//!
	bool _Startup(void) override;
	//!
	void _Shutdown(void) override;

	//!
	std::unordered_map<std::string, Cache> m_cache;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//

