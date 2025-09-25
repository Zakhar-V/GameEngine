#include "Serialization.hpp"
#include "File.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Variant
	//----------------------------------------------------------------------------//

	const Variant Variant::Null;
	const Variant Variant::EmptyArray(Variant::Type::Array);
	const Variant Variant::EmptyObject(Variant::Type::Object);

	//----------------------------------------------------------------------------//
	Variant::Variant(const Variant& _other)
	{
		SetType(_other.m_type);
		if (IsString())
			_String() = _other._String();
		else if (IsNode())
			_Node() = _other._Node();
		else
			m_ptr = _other.m_ptr;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(Variant&& _temp) noexcept
	{
		SetType(_temp.m_type);
		if (IsString())
			_String() = Move(_temp._String());
		else if (IsNode())
			_Node() = Move(_temp._Node());
		else
			m_ptr = _temp.m_ptr;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(bool _value)
	{
		SetType(Type::Bool);
		_Bool() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(int _value)
	{
		SetType(Type::Int);
		_Int() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(uint _value)
	{
		SetType(Type::Int);
		_Int() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(float _value)
	{
		SetType(Type::Float);
		_Float() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(void* _value)
	{
		SetType(Type::Pointer);
		_Ptr() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(const char* _value)
	{
		SetType(Type::String);
		_String() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(const String& _value)
	{
		SetType(Type::String);
		_String() = _value;
	}
	//----------------------------------------------------------------------------//
	Variant::Variant(String&& _value)
	{
		SetType(Type::String);
		_String() = Move(_value);
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::operator = (const Variant& _rhs)
	{
		SetType(_rhs.m_type);
		if (IsString())
			_String() = _rhs._String();
		else if (IsNode())
			_Node() = _rhs._Node();
		else
			m_ptr = _rhs.m_ptr;

		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::operator = (Variant&& _rhs) noexcept
	{
		SetType(_rhs.m_type);
		if (IsString())
			_String() = Move(_rhs._String());
		else if (IsNode())
			_Node() = Move(_rhs._Node());
		else
			m_ptr = _rhs.m_ptr;
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::SetType(Type _type)
	{
		if (m_type != _type)
		{
			switch (m_type)
			{
			case Type::String:
				_String().~String();
				break;
			case Type::Array:
			case Type::Object:
				_Node().~Node();
				break;
			};

			m_type = _type;

			switch (m_type)
			{
			case Type::String:
				new(&_String()) String();
				break;
			case Type::Array:
			case Type::Object:
				new(&_Node()) Node();
				break;
			default:
				m_ptr = 0;
				break;
			};
		}

		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::SetArray(InitializerList<Variant> _value)
	{
		Clear();
		for (auto i : _value)
			Push(i);
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::SetObject(InitializerList<KeyValue> _value)
	{
		Clear();
		for (auto i : _value)
			Set(i.first, i.second);
		return *this;
	}
	//----------------------------------------------------------------------------//
	bool Variant::AsBool(void) const
	{
		switch (m_type)
		{
		case Type::Null:
		case Type::Bool:
		case Type::Int:
			return m_int != 0;
		case Type::Float:
			return m_flt != 0;
		case Type::Pointer:
			return m_ptr != 0;
		}
		return false;
	}
	//----------------------------------------------------------------------------//
	int Variant::AsInt(void) const
	{
		switch (m_type)
		{
		case Type::Null:
		case Type::Bool:
		case Type::Int:
			return m_int;
		case Type::Float:
			return (int)m_flt;
		}
		return 0;
	}
	//----------------------------------------------------------------------------//
	float Variant::AsFloat(void) const
	{
		switch (m_type)
		{
		case Type::Null:
		case Type::Bool:
		case Type::Int:
			return (float)m_int;
		case Type::Float:
			return m_flt;
		}
		return 0;
	}
	//----------------------------------------------------------------------------//
	void* Variant::AsPointer(void) const
	{
		switch (m_type)
		{
		case Type::Null:
		case Type::Pointer:
			return m_ptr;
		}
		return 0;
	}
	//----------------------------------------------------------------------------//
	String Variant::AsString(void) const
	{
		switch (m_type)
		{
		case Type::Null:
			return "null";
		case Type::Bool:
			return m_bool ? "true" : "false";
		case Type::Int:
			return String::Format("%d", m_int);
		case Type::Float:
			return String::Format("%f", m_flt);
		case Type::Pointer:
			return String::Format("&%p", m_ptr);
		case Type::String:
			return _String();
		}
		return "";
	}
	//----------------------------------------------------------------------------//
	uint Variant::Size(void) const
	{
		switch (m_type)
		{
		case Type::Array:
		case Type::Object:
			return (uint)_Node().Size();
		}
		return 0;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Clear(void)
	{
		if (IsNode())
			_Node().Clear();
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Resize(uint _size)
	{
		SetType(Type::Array);
		_Node().Resize(_size);
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Get(uint _index)
	{
		SetType(Type::Array);
		return _Node()[_index].second;
	}
	//----------------------------------------------------------------------------//
	const Variant& Variant::Get(uint _index) const
	{
		return IsArray() ? _Node()[_index].second : Null;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Insert(uint _pos, const Variant& _value)
	{
		SetType(Type::Array);
		_Node().Insert(_Node().Begin() + _pos, { "", _value });
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Insert(uint _pos, Variant&& _value)
	{
		SetType(Type::Array);
		_Node().Insert(_Node().Begin() + _pos, { "", Move(_value) }); TODO("Not implemented yet");
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Push(const Variant& _value)
	{
		SetType(Type::Array);
		_Node().Push({ "", _value });
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Push(Variant&& _value)
	{
		SetType(Type::Array);
		_Node().Push({ "", Move(_value) });
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Append(void)
	{
		SetType(Type::Array);
		_Node().Push({ "", Null });
		return _Node().Back().second;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Pop(void)
	{
		if (IsArray())
			_Node().Pop();
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Erase(uint _start, uint _num)
	{
		if (IsArray())
			_Node().Erase(_Node().Begin() + _start, _Node().Begin() + _start + _num);
		return *this;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::GetOrAdd(const String& _key)
	{
		SetType(Type::Object);

		Variant* _value = Find(_key);
		if (_value)
			return *_value;

		_Node().Push({ _key, Null });
		return _Node().Back().second;
	}
	//----------------------------------------------------------------------------//
	const Variant& Variant::Get(const String& _key) const
	{
		const Variant* _value = Find(_key);
		return _value ? *_value : Null;
	}
	//----------------------------------------------------------------------------//
	Variant* Variant::Find(const String& _key)
	{
		if (IsObject())
		{
			for (auto& i : _Node())
			{
				if (i.first == _key)
					return &i.second;
			}
		}
		return nullptr;
	}
	//----------------------------------------------------------------------------//
	const Variant* Variant::Find(const String& _key) const
	{
		if (IsObject())
		{
			for (auto& i : _Node())
			{
				if (i.first == _key)
					return &i.second;
			}
		}
		return nullptr;
	}
	//----------------------------------------------------------------------------//
	Variant& Variant::Set(const String& _key, const Variant& _value)
	{
		GetOrAdd(_key) = _value;
		return *this;
	}
	//----------------------------------------------------------------------------//
	bool Variant::Erase(const String& _key)
	{
		if (IsObject())
		{
			for (auto i = _Node().Begin(); i != _Node().End(); ++i)
			{
				if (i->first == _key)
				{
					_Node().Erase(i);
					return true;
				}
			}
		}
		return false;
	}
	//----------------------------------------------------------------------------//
	Variant::Node& Variant::Container(void)
	{
		SetType(Type::Object);
		return _Node();
	}
	//----------------------------------------------------------------------------//
	const Variant::Node& Variant::Container(void) const
	{
		return IsNode() ? _Node() : EmptyObject._Node();
	}
	//----------------------------------------------------------------------------//
	bool Variant::Parse(const char* _str, String* _error)
	{
		Tokenizer _stream;
		_stream.s = _str;

		if (!_Parse(_stream))
		{
			if (_error)
			{
				int _l, _c;
				Tokenizer::GetErrorPos(_str, _stream.s, _l, _c);
				*_error = String::Format("(%d:%d) : JSON error : %s", _l, _c, _stream.e);
			}

			return false;
		}

		return true;
	}
	//----------------------------------------------------------------------------//
	String Variant::Print(void) const
	{
		String _str;
		_Print(_str, 0);
		return _str;
	}
	//----------------------------------------------------------------------------//
	bool Variant::_Parse(Tokenizer& _str)
	{
		//http://www.json.org/json-ru.html

		_str.NextToken();

		if (_str.e)
			return false;

		if (_str.EoF()) // eof
		{
			SetNull();
		}
		else if (_str.IsNumber()) // int or float
		{
			Tokenizer::Number _val;
			if (!_str.ParseNumber(_val))
				return false;

			if (_val.isFloat)
				SetFloat(_val.fValue);
			else
				SetInt(_val.iValue);
		}
		else if (_str.IsPointer()) // pointer
		{
			void* _ptr = 0;
			_str.ParsePointer(_ptr);
			SetPointer(_ptr);
		}
		else if (_str.IsString()) // string
		{
			SetType(Type::String);
			if (!_str.ParseString(_String()))
				return false;
		}
		else if (_str.Cmpi("true", 4)) // bool
		{
			_str += 4;
			SetBool(true);
		}
		else if (_str.Cmpi("false", 5))	// bool
		{
			_str += 5;
			SetBool(false);
		}
		else if (_str.Cmpi("null", 4)) // null
		{
			_str += 4;
			SetNull();
		}
		else if (_str[0] == '[') // array
		{
			++_str;
			SetType(Type::Array);
			for (;;)
			{
				if (_str[0] == ']')
				{
					++_str;
					break;
				}

				if (_str.EoF())
					return _str.RaiseError("Unexpectd EoF");

				if (!Append()._Parse(_str))
					return false;

				_str.NextToken();
				if (_str[0] == ',') // divisor (not necessarily) 
					++_str;
			}
		}
		else if (_str[0] == '{') // object
		{
			++_str;
			SetType(Type::Object);
			for (;;)
			{
				_str.NextToken();

				if (_str[0] == '}')
				{
					++_str;
					break;
				}

				if (_str.EoF())
					return _str.RaiseError("Unexpectd EoF");

				_Node().Push({ "", Null });
				KeyValue& _pair = _Node().Back();

				if (!_str.ParseString(_pair.first))
					return false;

				_str.NextToken();
				if (_str[0] == ':')
					++_str;
				else
					return _str.RaiseError("Expected ':' not found");

				_str.NextToken();
				if (!_pair.second._Parse(_str))
					return false;

				_str.NextToken();
				if (_str[0] == ',') // divisor (not necessarily) 
					++_str;
			}
		}
		else
		{
			return _str.RaiseError("Unknown symbol");
		}

		return true;
	}
	//----------------------------------------------------------------------------//
	void Variant::_Print(String& _dst, int _depth) const
	{
		switch (m_type)
		{
		case Type::Null:
		case Type::Bool:
		case Type::Int:
		case Type::Float:
		case Type::Pointer:
			_dst += AsString();
			break;
		case Type::String:
		{
			_PrintString(_dst, _String(), _depth);

		} break;
		case Type::Array:
		{
			bool _oneLine = true;
			if (_Node().Size() < 5)
			{
				for (const auto& i : _Node())
				{
					if (i.second.IsNode())
					{
						_oneLine = false;
						break;
					}
				}
			}
			else
				_oneLine = false;

			_dst += "[";

			for (const auto& i : _Node())
			{
				if (!_oneLine)
				{
					_dst += '\n';
					for (int i = 0; i <= _depth; ++i)
						_dst += "\t";
				}
				i.second._Print(_dst, _depth + 1);

				if (&i != &_Node().Back())
				{
					_dst += ",";
					if (_oneLine)
						_dst += " ";
				}
			}

			if (!_oneLine)
			{
				_dst += "\n";
				for (int i = 0; i < _depth; ++i)
					_dst += "\t";
			}
			_dst += "]";

		} break;
		case Type::Object:
		{
			_dst += "{\n";

			for (const auto& i : _Node())
			{
				for (int i = 0; i <= _depth; ++i)
					_dst += "\t";

				_PrintString(_dst, i.first, _depth + 1);

				_dst += " : ";
				i.second._Print(_dst, _depth + 1);

				if (&i != &_Node().Back())
					_dst += ",\n";
			}

			_dst += "\n";
			for (int i = 0; i < _depth; ++i)
				_dst += "\t";
			_dst += "}";

		} break;
		}
	}
	//----------------------------------------------------------------------------//
	void Variant::_PrintString(String& _dst, const String& _src, int _depth)
	{
		_dst += "\"";
		for (char s : _src)
		{
			if (s == '\n')
				_dst += "\\n";
			else if (s == '\r')
				_dst += "\\r";
			else if (s == '\\')
				_dst += "\\\\";
			else
				_dst += s; // TODO:
		}
		_dst += "\"";
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	//
	//----------------------------------------------------------------------------//
}
