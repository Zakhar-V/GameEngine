#pragma once

#include "System.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//

	//!
//#	undef SearchPath
//#	undef CreateDirectory

	//!
	enum class SeekOrigin : int
	{
		Set = SEEK_SET,
		Current = SEEK_CUR,
		End = SEEK_END,
	};

	//!
#	define gFileSystem Engine::FileSystem::Get()

	//----------------------------------------------------------------------------//
	// PathUtils
	//----------------------------------------------------------------------------//

	struct PathUtils
	{
		//!
		static bool IsFull(const char* _path);
		//!
		static void SplitFilename(const String& _filename, String* _device = nullptr, String* _dir = nullptr, String* _name = nullptr, String* _shortname = nullptr, String* _ext = nullptr);
		//!
		static void SplitPath(const char* _path, String* _device, Array<String>* _items);
		//!
		static String Make(const String& _device, const Array<String>& _items);
		//!
		static String Make(const String& _device, const String& _item);
		//!
		static String Full(const char* _path, const char* _root = nullptr, bool _addSlash = false);
		//!
		static String Short(const char* _path, const char* _root, bool _ignoreCase = true);
		//!
		inline static String FilePath(const String& _filename) { String _r; SplitFilename(_filename, nullptr, &_r); return _r; }
		//!
		inline static String FileName(const String& _filename) { String _r; SplitFilename(_filename, nullptr, nullptr, &_r); return _r; }
		//!
		inline static String FileNameOnly(const String& _filename) { String _r; SplitFilename(_filename, nullptr, nullptr, nullptr, &_r); return _r; }
		//!
		inline static String FileExt(const String& _filename) { String _r; SplitFilename(_filename, nullptr, nullptr, nullptr, nullptr, &_r); return _r; }
		//!
		static String MakeDirectory(const String& _path);
	};

	//----------------------------------------------------------------------------//
	// FileInfo
	//----------------------------------------------------------------------------//

	//!
	enum class FileAttribs : uint
	{
		File = 0x1,
		Directory = 0x2,
		Hidden = 0x4,
		ReadOnly = 0x8,
		All = Hidden | File | Directory | ReadOnly,
	};

	//!
	inline bool operator & (FileAttribs _lhs, FileAttribs _rhs) { return (uint)_lhs & (uint)_rhs; }
	//!
	inline bool operator & (uint _lhs, FileAttribs _rhs) { return (uint)_lhs & (uint)_rhs; }
	//!
	inline uint operator | (FileAttribs _lhs, FileAttribs _rhs) { return (uint)_lhs | (uint)_rhs; }
	//!
	inline uint operator | (uint _lhs, FileAttribs _rhs) { return (uint)_lhs | (uint)_rhs; }

	//!
	struct ENGINE_API FileInfo
	{
		//!
		uint attribs = 0;
		//!
		uint64 size = 0;
		//!
		time_t time = 0; // global
		//!
		String name; // short name
		//!
		String path; // full path
		//!
		String dir; // directory
	};

	//----------------------------------------------------------------------------//
	// File
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API File : public Object
	{
	public:
		RTTI_ABSTRACT(File);

		//!
		const String& GetName(void) { return m_name; }
		//!
		const String& GetRoot(void) { return m_root; }
		//!
		virtual void* GetHandle(void) abstract;

		//!
		virtual bool IsInMemory(void) { return false; }
		//!
		virtual AccessMode Access(void) abstract;
		//!
		virtual uint64 Size(void) abstract;
		//!
		virtual uint64 Tell(void) abstract;
		//!
		virtual void Seek(int64 _pos, SeekOrigin _origin = SeekOrigin::Set) abstract;
		//!
		virtual bool Eof(void) abstract;
		//!
		virtual uint Read(void* _dst, uint _size) abstract;
		//!
		virtual uint Write(const void* _src, uint _size) abstract;
		//!
		virtual void Flush(void) abstract;

		//!
		String ReadString(int _maxLength = -1);
		//!
		uint WriteString(const String& _str, bool _binary = true);

	protected:

		//!
		String m_name;
		//!
		String m_root;
	};

	//----------------------------------------------------------------------------//
	// 	DiskFile
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API DiskFile : public File
	{
	public:
		RTTI_ABSTRACT(DiskFile);

		//!
		DiskFile(FILE* _handle, const String& _path, const String& _root, AccessMode _access);
		//!
		~DiskFile(void);

		//!
		void* GetHandle(void) { return m_handle; }
		//!
		AccessMode Access(void) { return m_access; }
		//!
		uint64 Size(void) override;
		//!
		uint64 Tell(void) override;
		//!
		void Seek(int64 _pos, SeekOrigin _origin = SeekOrigin::Set) override;
		//!
		bool Eof(void) override;
		//!
		uint Read(void* _dst, uint _size) override;
		//!
		uint Write(const void* _src, uint _size) override;
		//!
		void Flush(void);

	protected:
		//!
		uint64 _GetSize(void);

		//!
		AccessMode m_access = AccessMode::None;
		//!
		uint64 m_size = 0;
		//!
		FILE* m_handle = nullptr;
	};

	//----------------------------------------------------------------------------//
	// 	MemoryFile
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API MemoryFile : public File
	{
	public:
		//RTTI(MemoryFile);

	protected:

	};

	//----------------------------------------------------------------------------//
	// VirtualFileSystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API VirtualFileSystem : public Object
	{
	public:
		//RTTI(VirtualFileSystem);

		//!
		const String& GetPath(void) const { return m_path; }
		//!
		virtual bool Open(const String& _path) abstract;
		//!
		virtual void Close(void) abstract;
		//!
		bool IsOpened(void) const { return m_opened; }

		///\return true if file exists \param[in] _name is normalized relative path.
		virtual bool FileExists(const String& _name) abstract;
		///open the file in read only mode \param[in] _name is normalized relative path.
		virtual SharedPtr<File> OpenFile(const String& _name) abstract;

	protected:
		//!
		String m_path;
		//!
		bool m_opened = false;
	};

	//----------------------------------------------------------------------------//
	// DiskFileSystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API DiskFileSystem : public VirtualFileSystem
	{
	public:
		RTTI(DiskFileSystem);

		//!
		bool Open(const String& _path) override;
		//!
		void Close(void) override;
		///\see VirtualFileSystem::FileExists
		bool FileExists(const String& _name) override;
		///\see VirtualFileSystem::OpenFile
		SharedPtr<File> OpenFile(const String& _name) override;
	};

	//----------------------------------------------------------------------------//
	// PackageFileSystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API PackageFileSystem : public VirtualFileSystem
	{
	public:
		/*RTTI(PackageFileSystem);

		//!
		bool Open(const String& _path) override;
		//!
		void Close(void) override;
		///\see VirtualFileSystem::FileExists
		bool FileExists(const String& _name) override;
		///\see VirtualFileSystem::OpenFile
		SharedPtr<File> OpenFile(const String& _name, uint _flags = 0) override;*/
	};

	//----------------------------------------------------------------------------//
	// SearchPath
	//----------------------------------------------------------------------------//

	//!
	struct SearchPath
	{
		//!
		SearchPath(const String& _name = String::Empty, VirtualFileSystem* _fs = nullptr, int _priority = 0);
		//!
		bool operator < (const SearchPath& _rhs) const { return priority >= _rhs.priority; }

		//!
		String name;
		//!
		SharedPtr<VirtualFileSystem> fs;
		//!
		int priority = 0;
	};

	//----------------------------------------------------------------------------//
	// FileSystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API FileSystem : public System, public Singleton<FileSystem>
	{
	public:
		RTTI(FileSystem);

		//!
		FileSystem(void);

		//!
		const String& AppDir(void) const { return m_appDir; }
		//!
		const String& AppName(void) const { return m_appName; }
		//!
		const String& AppShortName(void) const { return m_appShortName; }


		//!
		const String& RootDir(void) const { return m_rootDir; }

		//!
		void AddSearchPath(const String& _path, int _priority = 0);
		//!
		uint GetNumSearchPaths(void) const { return m_paths.Size(); }
		//!
		const SearchPath& GetSearchPath(uint _idx) { return m_paths[_idx]; }
		//!
		void RemoveSearchPath(uint _idx);
		//!
		void RemoveAllSearchPaths(void);

		//bool CreatePackageFromDirectory(const String& _src, const String& _dst);

		//bool FileExists(const String& _path);
		//!
		SharedPtr<File> OpenFile(const String& _path, AccessMode _access = AccessMode::Read);

		//!
		bool CreateDirectory(const String& _path);

		//!
		bool GetDiskFileInfo(FileInfo& _fi, const String& _path);
		//!
		bool EnumDiskFiles(Array<FileInfo>& _dst, const String& _dir, uint _attribMask = (uint)FileAttribs::All, const String& _nameMask = String::Empty);


		//!
		bool LoadJson(const String& _path, Variant& _dst, String* _err = nullptr);
		//!
		bool SaveJson(const String& _path, const Variant& _src);

	protected:

		//!
		bool _Startup(void);
		//!
		void _Shutdown(void);

		//!
		bool _InitSearchPath(SearchPath& _path, const String& _fpath);

		//!
		String m_appDir;
		//!
		String m_appName;
		//!
		String m_appShortName;

		//!
		String m_rootDir;

		//!
		Array<SearchPath> m_paths;
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
