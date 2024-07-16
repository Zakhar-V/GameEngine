#include "File.hpp"



//---------------------------------------------------------------------------//
// PathUtils
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
std::string PathUtils::Extension(const std::string& _path)
{
	if (!_path.empty())
	{
		const char* i = _path.c_str() + _path.length();
		while (i-- > _path.c_str() && *i != '.' && *i != '/' && *i != '\\');

		if (*i == '.')
			return std::string(i + 1);
	}

	return "";
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// File
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
File::File(const std::string& _name, Access _access)
{
	Open(_name, _access);
}
//---------------------------------------------------------------------------//
File::~File(void)
{
	Close();
}
//---------------------------------------------------------------------------//
bool File::Open(const std::string& _name, Access _access)
{
	Close();

	m_name = _name;
	m_access = _access;
	if (!m_name.empty())
	{
		m_handle = fopen(m_name.c_str(), _access == Access::Read ? "rb" : (_access == Access::Edit ? "r+b" : "w+b"));

		if (IsReadOnly())
			m_size = _GetSize();
	}

	return IsOpened();
}
//---------------------------------------------------------------------------//
void File::Close(void)
{
	if (m_handle)
	{
		fflush(m_handle);
		fclose(m_handle);
		m_handle = nullptr;
	}
}
uint File::Size(void)
{
	return IsReadOnly() ? m_size : _GetSize();
}
//---------------------------------------------------------------------------//
uint File::Tell(void)
{
	if (m_handle)
		return ftell(m_handle);
	return 0;
}
//---------------------------------------------------------------------------//
bool File::EoF(void)
{
	if (m_handle)
		return feof(m_handle);
	return true;
}
//---------------------------------------------------------------------------//
void File::Seek(int _pos, SeekOrigin _origin)
{
	if (m_handle)
		fseek(m_handle, _pos, (int)_origin);
}
//---------------------------------------------------------------------------//
uint File::Read(void* _dst, uint _size)
{
	if (m_handle)
		return (uint)fread(_dst, 1, _size, m_handle);
	return 0;
}
//---------------------------------------------------------------------------//
uint File::Write(const void* _src, uint _size)
{
	if (m_handle)
		return (uint)fwrite(_src, 1, _size, m_handle);
	return 0;
}
//---------------------------------------------------------------------------//
void File::Flush(void)
{
	if (m_handle)
		fflush(m_handle);
}
//---------------------------------------------------------------------------//
std::string File::ReadString(char _end)
{
	std::string _str;
	if(m_handle)
	{
		for (char i = fgetc(m_handle); i != _end && !feof(m_handle); i = fgetc(m_handle))
			_str.push_back(i);
	}
	return _str;
}
//---------------------------------------------------------------------------//
uint File::_GetSize(void)
{
	if (m_handle)
	{
		uint _pos = ftell(m_handle);
		fseek(m_handle, 0, SEEK_END);
		uint _size = ftell(m_handle);
		fseek(m_handle, _pos, SEEK_SET);
		return _size;
	}
	return 0;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// FileSystem
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
bool FileSystem::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	return true;
}
//---------------------------------------------------------------------------//
void FileSystem::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
