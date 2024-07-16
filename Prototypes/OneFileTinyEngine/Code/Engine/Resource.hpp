#pragma once

#include "System.hpp"
#include "File.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Resource
	//---------------------------------------------------------------------------//

	//!
	typedef Reference<class Resource> ResourceRef;

	//!
	class Resource : public Object
	{
	public:
		//!
		RTTI(Resource);

	protected:
	};

	//---------------------------------------------------------------------------//
	// ResourceManager
	//---------------------------------------------------------------------------//

	//!
#	define gResourceManager Tiny::ResourceManager::Get()

	//!
	class ResourceManager : public System, public Singleton<ResourceManager>
	{
	public:
		//!
		RTTI(ResourceManager);

	protected:

		std::unordered_map<std::string, ResourceRef> m_resources;
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
