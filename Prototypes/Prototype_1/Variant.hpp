#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "String.hpp"
#include "Container.hpp"

//---------------------------------------------------------------------------//
// Variant
//---------------------------------------------------------------------------//

//!
class Variant
{
public:
	//!
	enum class Type
	{
		Null,
		Pointer,
		Bool,
		Int,
		Float,
		String,
		Array,
		Object,
	};

	Variant() = default;

	static const Variant Null;

protected:

	Type m_type;
	std::string m_name;
	std::string m_str;
	union
	{
		void* m_ptr = nullptr;
		int m_int;
		float m_flt;
	};
	Variant* m_firstChild = nullptr;
	Variant* m_lastChild = nullptr;
	Variant* m_prev = nullptr;
	Variant* m_next = nullptr;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
