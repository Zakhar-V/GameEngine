#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "System.hpp"

//---------------------------------------------------------------------------//
// PathUtils
//---------------------------------------------------------------------------//

struct PathUtils
{
	//!
	static std::string Extension(const std::string& _path);
};

//---------------------------------------------------------------------------//
// File
//---------------------------------------------------------------------------//

//!
class File : public Object
{
public:
	RTTI(File);

	enum class Access
	{
		Read, // только чтение, файл должен существовать
		Edit, // чтение и запись, файл должен существовать
		Create, // создать или перезаписать файл
	};

	enum class SeekOrigin
	{
		Current = SEEK_CUR,
		Set = SEEK_SET,
		End = SEEK_END,
	};

	//!
	File(const std::string& _name = "", Access _access = Access::Read);
	//!
	~File(void);

	//!
	bool Open(const std::string& _name, Access _access = Access::Read);
	//!
	void Close(void);

	//!
	bool IsOpened(void) { return m_handle != 0; }
	//!
	bool IsReadOnly(void) { return m_access == Access::Read; }
	//!
	const std::string& Name(void) { return m_name; }

	//!
	uint Size(void);
	//!
	uint Tell(void);
	//!
	bool EoF(void);
	//!
	void Seek(int _pos, SeekOrigin _origin = SeekOrigin::Current);
	//!
	uint Read(void* _dst, uint _size);
	//!
	uint Write(const void* _src, uint _size);
	//!
	void Flush(void);


	//!
	std::string ReadString(char _end = 0);

protected:
	//!
	uint _GetSize(void);

	//!
	std::string m_name;
	//!
	FILE* m_handle = nullptr;
	//!
	uint m_size = 0;
	//!
	Access m_access;
};

//---------------------------------------------------------------------------//
// FileSystem 
//---------------------------------------------------------------------------//

//!
#define gFileSystem Scene::Get()

//!
class FileSystem : public System, public Singleton<FileSystem>
{
public:
	RTTI(FileSystem);

protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//

