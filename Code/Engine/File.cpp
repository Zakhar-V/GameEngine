#include "File.hpp"
#include <io.h>
#include <sys/stat.h>
#ifdef _MSC_VER
#	include <direct.h>
#else
#	include <dirent.h>
#endif
#ifdef _WIN32
#	include <Windows.h>
#	undef SearchPath
#	undef CreateDirectory
#endif

namespace Engine
{
	//----------------------------------------------------------------------------//
	// PathUtils
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	bool PathUtils::IsFull(const char* _path)
	{
#	ifdef _WIN32
		return _path && _path[1] == ':';
#	else // linux
		return _path && _path[0] == '/';
#	endif
	}
	//----------------------------------------------------------------------------//
	void PathUtils::SplitFilename(const String& _filename, String* _device, String* _dir, String* _name, String* _shortname, String* _ext)
	{
		uint _l = _filename.Length();
		if (_device)
		{
#	ifdef _WIN32
			const char* c = strchr(_filename, ':');
			if (c)
				*_device = String(_filename, c);
#	else
			const char* c = _filename.c_str();
			if (*c && strchr("/\\", *c))
			{
				while (*++c && !strchr("/\\", *c));
				*_device = String(_filename.Ptr(1), c);
			}
#endif
		}
		if (_dir)
		{
			uint c = _l;
			for (; c > 0 && strchr("/\\", _filename[c - 1]); --c);
			for (; c > 0 && !strchr("/\\", _filename[c - 1]); --c);
			*_dir = _filename.SubStr(0, c);
		}
		if (_name || _ext || _shortname)
		{
			uint _np = _l;
			for (; _np > 0 && !strchr("/\\", _filename[_np - 1]); --_np);
			String _n = _filename.SubStr(_np);
			if (_name)
				*_name = _n;
			if (_ext || _shortname)
			{
				uint _pp = _l - _np;
				for (; _pp > 0 && _n[--_pp] != '.';);
				if (_shortname)
					*_shortname = _pp > 0 ? _n.SubStr(0, _pp) : _n;
				if (_ext)
					*_ext = _pp > 0 ? _n.SubStr(_pp + 1) : String::Empty;
			}
		}
	}
	//----------------------------------------------------------------------------//
	void PathUtils::SplitPath(const char* _path, String* _device, Array<String>* _items)
	{
		static const String _P1 = ".";
		static const String _P2 = "..";

		if (_path && *_path)
		{
			const char* p = _path;

#	ifdef _WIN32
			const char* c = strchr(_path, ':');
			if (c)
			{
				if (_device)
					*_device = String(p, c);
				p = ++c;
			}
			else c = _path;
#	else
			const char* c = _path;
			if (strchr("/\\", *c))
			{
				while (*++c && !strchr("/\\", *c));
				if (_device)
					*_device = String(p + 1, c);
				p = ++c;
			}
#endif
			if (_items)
			{
				_items->Clear();
				_items->Reserve(10);
				String _item;
				for (;;)
				{
					if (!*c || strchr("\\/", *c))
					{
						if (c != p)
						{
							_item = String(p, (uint)(c - p));
							if (_item == _P2)
							{
								if (_items->IsEmpty() || _items->Back() == _P2)
									_items->Push(_P2);
								else
									_items->Pop();
							}
							else if (_item != _P1)
								_items->Push(_item);
						}
						p = c + 1;
					}
					if (!*c++)
						break;
				}
			}
		}
	}
	//----------------------------------------------------------------------------//
	String PathUtils::Make(const String& _device, const Array<String>& _items)
	{
		String _path;
#	ifdef _WIN32
		if (_device.NonEmpty())
			_path += _device + ":/";
#	else
		if (_device.NonEmpty())
			_path += "/" + _device + "/";
#	endif
		//if (_items.IsEmpty()) _path += ".";
		for (uint i = 0; i < _items.Size(); ++i)
		{
			if (i > 0)
				_path += "/";
			_path += _items[i];
		}
		return _path;
	}
	//----------------------------------------------------------------------------//
	String PathUtils::Make(const String& _device, const String& _item)
	{
		String _path;
#	ifdef _WIN32
		if (_device.NonEmpty())
			_path += _device + ":/";
#	else
		if (_device.NonEmpty())
			_path += "/" + _device + "/";
#	endif
		return _path + _item;
	}
	//----------------------------------------------------------------------------//
	String PathUtils::Full(const char* _path, const char* _root, bool _addSlash)
	{
		static const String _P1 = ".";
		static const String _P2 = "..";

		if (_path)
		{
			String _d, _pd;
			Array<String> _p, _f;
			if (IsFull(_path) || !(_root && *_root))
			{
				char _last = _path[strlen(_path) - 1];
				if (!strchr(_path, '\\') &&
					!strstr(_path, "..") &&
					!strstr(_path, "./") &&
					!strstr(_path, "/.") &&
					!strstr(_path, "//") &&
					_last != '/')
					return _addSlash ? MakeDirectory(_path) : String(_path);

				SplitPath(_path, &_pd, &_p);
				String _r = Make(_pd, _p);
				if (_addSlash)
					_r = MakeDirectory(_r);
				return _r;
			}

			SplitPath(_path, &_pd, &_p);
			SplitPath(_root, &_d, &_f);
			for (uint i = 0; i < _p.Size(); ++i)
			{
				if (_p[i] == _P2)
				{
					if (_f.IsEmpty() || _f.Back() == _P2)
						_f.Push(_P2);
					else
						_f.Pop();
				}
				else if (_p[i] != _P1)
					_f.Push(_p[i]);
			}
			String _r = Make(_d, _f);
			if (_addSlash)
				_r = MakeDirectory(_r);
			return _r;

		}
		return _addSlash ? String("./") : String::Empty;
	}
	//----------------------------------------------------------------------------//
	String PathUtils::Short(const char* _path, const char* _root, bool _ignoreCase)
	{
		String _pd, _rd, _sp;
		Array<String> _p, _r, _s;

		SplitPath(_path, &_pd, &_p);
		SplitPath(_root, &_rd, &_r);

		if (!_pd.Equals(_rd, _ignoreCase))
			return _path;

		uint _ps = (uint)_p.Size();
		uint _rs = (uint)_r.Size();
		uint _pos = 0;
		for (uint i = 0; i < _ps && i < _rs; ++i)
		{
			if (_p[_pos].Equals(_r[_pos], _ignoreCase))
				++_pos;
			else
				break;
		}

		for (uint i = _pos; i < _rs; ++i)
			_s.Push("..");

		for (; _pos < _ps; ++_pos)
			_s.Push(_p[_pos]);

		if (_s.IsEmpty())
			return ".";

		for (uint i = 0, _ss = _s.Size(); i < _ss; ++i)
		{
			if (i > 0)
				_sp += "/";
			_sp += _s[i];
		}

		return _sp;
	}
	//----------------------------------------------------------------------------//
	String PathUtils::MakeDirectory(const String& _path)
	{
		String _dir = _path;
		if (_dir.IsEmpty())
			_dir = ".";
		if (_dir.Last() != '\\' && _dir.Last() != '/')
			_dir += "/";
		return _dir;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// File
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	String File::ReadString(int _maxLength)
	{
		String _r;
		uint64 _fsize = Size();
		uint64 _readed, _length, _pos = Tell(), _size = _maxLength < 0 ? _fsize : _maxLength;
		if (_pos > _fsize)
			_pos = _fsize;
		if (_size + _pos > _fsize)
			_size = _fsize - _pos;

		Array<char> _buf(_size + 1);
		_readed = Read(&_buf[0], _size);
		_buf[_readed] = 0;
		_length = (uint)strlen(&_buf[0]);
		_r = &_buf[0];

		if (_length + 1 < _readed)
			Seek(_pos + _length + 1); // null-terminated string

		return _r;
	}
	//----------------------------------------------------------------------------//
	uint File::WriteString(const String& _str, bool _binary)
	{
		return Write(_str.IsEmpty() ? "" : _str, _str.Length() + (_binary ? 1 : 0));
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// DiskFile
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	DiskFile::DiskFile(FILE* _handle, const String& _path, const String& _root, AccessMode _access) :
		m_handle(_handle), m_access(_access)
	{
		ASSERT(m_handle != nullptr);

		m_name = _path;
		m_root = _root;
		if (m_access == AccessMode::Read)
			m_size = _GetSize();
	}
	//----------------------------------------------------------------------------//
	DiskFile::~DiskFile(void)
	{
		fclose(m_handle);
	}
	//----------------------------------------------------------------------------//
	uint64 DiskFile::Size(void)
	{
		return (m_access & AccessMode::Write) ? _GetSize() : m_size;
	}
	//----------------------------------------------------------------------------//
	uint64 DiskFile::Tell(void)
	{
		return _ftelli64(m_handle);
	}
	//----------------------------------------------------------------------------//
	void DiskFile::Seek(int64 _pos, SeekOrigin _origin)
	{
		_fseeki64(m_handle, _pos, (int)_origin);
	}
	//----------------------------------------------------------------------------//
	bool DiskFile::Eof(void)
	{
		return feof(m_handle) != 0;
	}
	//----------------------------------------------------------------------------//
	uint DiskFile::Read(void* _dst, uint _size)
	{
		ASSERT(_dst || !_size);
		return (m_access & AccessMode::Read) ? (uint)fread(_dst, 1, _size, m_handle) : 0;
	}
	//----------------------------------------------------------------------------//
	uint DiskFile::Write(const void* _src, uint _size)
	{
		ASSERT(_src || !_size);
		return (m_access & AccessMode::Write) ? (uint)fwrite(_src, 1, _size, m_handle) : 0;
	}
	//----------------------------------------------------------------------------//
	void DiskFile::Flush(void)
	{
		if (m_access & AccessMode::Write)
			fflush(m_handle);
	}
	//----------------------------------------------------------------------------//
	uint64 DiskFile::_GetSize(void)
	{
		int64 _size, _pos = _ftelli64(m_handle);
		fseek(m_handle, 0, SEEK_END);
		_size = _ftelli64(m_handle);
		_fseeki64(m_handle, _pos, SEEK_SET);
		return _size;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// DiskFileSystem
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	bool DiskFileSystem::Open(const String& _path)
	{
		String _fpath = PathUtils::Full(_path, gFileSystem->RootDir(), true);

		if (m_path.Equals(_fpath, true))
			return m_opened; // no changes

		Close();

		m_path = _fpath;

		struct stat _st;
		if (stat(_fpath, &_st) || !(_st.st_mode & S_IFDIR))
		{
			LOG_ERROR("Couldn't open DiskFileSystem '%s' : No directory", m_path.CStr());
			return m_opened = false;
		}

		return m_opened = true;
	}
	//----------------------------------------------------------------------------//
	void DiskFileSystem::Close(void)
	{
		m_opened = false;
	}
	//----------------------------------------------------------------------------//
	bool DiskFileSystem::FileExists(const String& _name)
	{
		struct stat _st;
		return !stat(_name, &_st) && (_st.st_mode & S_IFREG);
	}
	//----------------------------------------------------------------------------//
	SharedPtr<File> DiskFileSystem::OpenFile(const String& _name)
	{
		if (_name.IsEmpty())
			return nullptr;

		String _fp = m_path + _name;
		String _rp = PathUtils::Short(_fp, m_path);

		AccessMode _access = AccessMode::Read;
		FILE* _f = fopen(_fp, "rb");
		return _f ? new DiskFile(_f, _rp, m_path, _access) : nullptr;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// SearchPath
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	SearchPath::SearchPath(const String& _name, VirtualFileSystem* _fs, int _priority) :
		name(_name), fs(_fs), priority(_priority)
	{
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// FileSystem
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	FileSystem::FileSystem(void)
	{
#	ifdef _WIN32
		char _buf[4096];
		uint _length = GetModuleFileNameA(0, _buf, sizeof(_buf));
		_buf[_length] = 0;
		PathUtils::SplitFilename(PathUtils::Full(_buf), nullptr, &m_appDir, &m_appName, &m_appShortName);
#	else
		NOT_IMPLEMENTED_YET("Get app name");
#	endif
		m_appDir = PathUtils::MakeDirectory(m_appDir);

		if (chdir(m_appDir))
		{
			LOG_WARNING("Unable to set current directory as '%s'", m_appDir.CStr());
		}

		m_rootDir = m_appDir;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::_Startup(void)
	{
		const Variant& _cfg = gSystem->GetConfig("FileSystem");

		if (_cfg.IsNull())
		{
			// use default settings
		}
		else
		{
			String _root = _cfg["RootDir"];

			if (_root.NonEmpty())
				m_rootDir = PathUtils::MakeDirectory(PathUtils::Full(_root, gSystem->GetConfigPath()));

			const Variant& _paths = _cfg["SearchPaths"];
			for(auto i : _paths)
				AddSearchPath(i.second["Path"], i.second["Priority"]);
		}
		
		return true;
	}
	//----------------------------------------------------------------------------//
	void FileSystem::_Shutdown(void)
	{
		Variant& _cfg = gSystem->GetConfig("FileSystem");
		_cfg["RootDir"] = gSystem->GetConfigPath(); // PathUtils::MakeDirectory(PathUtils::Short(gSystem->GetConfigPath(), m_appDir));

		Variant& _paths = _cfg["SearchPaths"];
		_paths.Clear();
		_paths.SetType(Variant::Type::Array);
		for (auto i : m_paths)
		{
			Variant _v;
			_v["Path"] = i.name;
			_v["Priority"] = i.priority;
			_paths.Push(_v);
		}

		m_paths.Clear();
	}
	//----------------------------------------------------------------------------//
	void FileSystem::AddSearchPath(const String& _path, int _priority)
	{
		String _nname = PathUtils::Full(_path, nullptr, true);
		String _fpath = PathUtils::Full(_nname, m_rootDir, true);
		if (_nname.IsEmpty())
			return;

		int i = 0;
		for (SearchPath& _sp : m_paths)
		{
			if (_sp.name.Equals(_nname, true) || _sp.fs->GetPath().Equals(_fpath, true))
			{
				if (_sp.priority != _priority)
				{
					m_paths.Erase(i);
					break;
				}
				else
				{
					LOG_INFO("Search path '%s' is already exists as '%s' or '%s'", _nname.CStr(), _sp.name.CStr(), _sp.fs->GetPath().CStr());
					return;
				}
			}
			++i;
		}

		SearchPath _sp(_nname, nullptr, _priority);
		_InitSearchPath(_sp, _fpath); // open or create

		for (auto i = m_paths.Begin();; ++i)
		{
			if (i == m_paths.End())
			{
				m_paths.Push(_sp);
				break;
			}
			if (i->priority <= _priority)
			{
				m_paths.Insert(i, _sp);
				break;
			}
		}

		LOG_EVENT("Add search path '%s' as '%s' with priority %d", _nname.CStr(), _fpath.CStr(), _priority);
	}
	//----------------------------------------------------------------------------//
	void FileSystem::RemoveSearchPath(uint _idx)
	{
		m_paths.Erase(_idx);
	}
	//----------------------------------------------------------------------------//
	void FileSystem::RemoveAllSearchPaths(void)
	{
		m_paths.Clear();
	}
	//----------------------------------------------------------------------------//
	SharedPtr<File> FileSystem::OpenFile(const String& _path, AccessMode _access)
	{
		if (_path.IsEmpty() || _access == AccessMode::None)
			return nullptr;

		String _root = (_access & AccessMode::Write) ? m_rootDir : (m_paths.IsEmpty() ? m_rootDir : String::Empty);
		String _npath = PathUtils::Full(_path, _root);
		String _rpath = PathUtils::Short(_path, _root);

		// open file with full name or create file in write directory
		if (PathUtils::IsFull(_npath))
		{
			FILE* _f;
			if (_access & AccessMode::Write)
			{
				CreateDirectory(PathUtils::FilePath(_npath));

				_f = fopen(_npath, (_access & AccessMode::Read) ? "w+b" : "wb");
				if (!_f)
				{
					LOG_ERROR("Couldn't create file '%s'", _rpath.CStr());
					return nullptr;
				}
			}
			else
			{
				_f = fopen(_npath, "rb");
				if (!_f)
				{
					LOG_ERROR("Couldn't open file '%s': File not was found", _rpath.CStr());
					return nullptr;
				}
			}

			return new DiskFile(_f, PathUtils::IsFull(_path) ? _npath : _rpath, PathUtils::IsFull(_path) ? String::Empty : _root,  _access);
		}

		// open read only file

		SharedPtr<File> _f = nullptr;
		for (auto& _sp : m_paths)
		{
			_f = _sp.fs->OpenFile(_npath);
			if (_f)
				break;
		}

		if (!_f)
		{
			LOG_ERROR("Couldn't open file '%s': File not was found", _npath.CStr());
		}

		return _f;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::CreateDirectory(const String& _path)
	{
		String _dir = PathUtils::Full(_path, m_rootDir);
		struct stat _st;
		if (stat(_dir, &_st))
		{
			Array<String> _items;
			String _dev;
			PathUtils::SplitPath(_dir, &_dev, &_items);
			String _cd;
			for (auto i : _items)
			{
				if (_cd.IsEmpty())
					_cd = PathUtils::Make(_dev, i);
				else
					_cd.Append('/').Append(i);

				if (stat(_cd, &_st))
				{
					if (mkdir(_cd))
					{
						LOG_ERROR("Couldn't create directory '%s'", _cd.CStr());
						return false;
					}
				}
			}
		}
		else if (!(_st.st_mode & _S_IFDIR))
		{
			LOG_ERROR("Couldn't create directory '%s'", _dir.CStr());
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::GetDiskFileInfo(FileInfo& _fi, const String& _path)
	{
		_fi.path = PathUtils::Full(_path, m_rootDir);
		if (_fi.path.IsEmpty())
			return false;

		__finddata64_t _fd;
		intptr_t _fh = _findfirst64(_fi.path, &_fd);
		if (_fh == -1)
			return false;

		_fi.attribs = (_fd.attrib & _A_HIDDEN) ? (uint)FileAttribs::Hidden : 0;
		_fi.attribs |= (uint)((_fd.attrib & _A_SUBDIR) ? FileAttribs::Directory : FileAttribs::File);
		_fi.attribs |= (_fd.attrib & _A_RDONLY) ? (uint)FileAttribs::ReadOnly : 0;
		_fi.name = _fd.name;
		_fi.time = _fd.time_write;
		_fi.size = _fd.size;

		_fi.dir = PathUtils::FilePath(_fi.path);
		_fi.path = PathUtils::FilePath(_fi.path) + _fi.name;

		_findclose(_fh);
		return true;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::EnumDiskFiles(Array<FileInfo>& _dst, const String& _dir, uint _attribMask, const String& _nameMask)
	{
		FileInfo _fi;
		__finddata64_t _fd;
		String _fpath = PathUtils::Full(_dir, m_rootDir, true);
		intptr_t _fh = _findfirst64(_fpath + "*", &_fd);
		if (_fh == -1)
			return false;

		bool _isDir, _isHidden, _isReadOnly;
		bool _enumFiles = (_attribMask & FileAttribs::File) != 0;
		bool _enumDirs = (_attribMask & FileAttribs::Directory) != 0;
		bool _enumHidden = (_attribMask & FileAttribs::Hidden) != 0;
		//bool _enumReadOnly = (_attribMask & FA_Hidden) != 0;

		_fi.dir = _fpath;

		while (!_findnext64(_fh, &_fd))
		{
			_isDir = (_fd.attrib & _A_SUBDIR) != 0;
			_isHidden = (_fd.attrib & _A_HIDDEN) != 0;

			if (_isDir ? !_enumDirs : !_enumFiles)
				continue;
			if (_isHidden && !_enumHidden)
				continue;
			if (!strcmp(_fd.name, "."))
				continue;
			if (!strcmp(_fd.name, ".."))
				continue;


			_fi.attribs = _isHidden ? (uint)FileAttribs::Hidden : 0;
			_fi.attribs |= _isDir ? (uint)FileAttribs::Directory : (uint)FileAttribs::File;
			_fi.time = _fd.time_write;
			_fi.name = _fd.name;
			_fi.path = _fpath + _fi.name;
			_fi.size = _fd.size;

			if (_nameMask.NonEmpty() && !_fi.path.Match(_nameMask, true))
				continue;

			_dst.Push(_fi);

		}

		_findclose(_fh);
		return true;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::LoadJson(const String& _path, Variant& _dst)
	{
		SharedPtr<File> _file = OpenFile(_path);
		return _file && _dst.Load(_file);
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::SaveJson(const String& _path, const Variant& _src)
	{
		SharedPtr<File> _file = OpenFile(_path, AccessMode::Write);
		if (_file)
		{
			_src.Save(_file);
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------//
	bool FileSystem::_InitSearchPath(SearchPath& _path, const String& _fpath)
	{
		TODO("Поддержка архивов");

		struct stat _st;
		if (stat(_fpath, &_st))
		{
			if (!_path.fs)
			{
				_path.fs = new DiskFileSystem();
			}

			if (_path.fs)
			{
				_path.fs->Close();
			}

			LOG_ERROR("Couldn't open virtual file system '%s' : No file or directory", _fpath.CStr());
			return false;
		}

		if (_st.st_mode & _S_IFDIR)
		{
			if (!_path.fs || !_path.fs->IsClass<DiskFileSystem>())
			{
				_path.fs = new DiskFileSystem();
			}
		}
		else
		{
			if (!_path.fs)
			{
				_path.fs = new DiskFileSystem();
			}

			//TODO: classify type of archive
			LOG_ERROR("Couldn't open virtual file system '%s' : Archive is not supported", _fpath.CStr());
			return false;
		}

		return _path.fs->Open(_fpath);
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}