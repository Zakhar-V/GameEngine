#pragma once

#include "System.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// File
	//---------------------------------------------------------------------------//

	//!
	class File : public Object
	{
	public:
		//!
		RTTI(File);

	protected:
	};

	//---------------------------------------------------------------------------//
	// FileSystem
	//---------------------------------------------------------------------------//

	//!
#	define gFileSystem Tiny::FileSystem::Get()

	//!
	class FileSystem : public System, public Singleton<FileSystem>
	{
	public:
		//!
		RTTI(FileSystem);

	protected:
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
