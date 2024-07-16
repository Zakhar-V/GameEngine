#pragma once

#include "Common.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// types
	//---------------------------------------------------------------------------//

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	using glm::ivec2;

	using glm::quat;

	using glm::mat4;

	//---------------------------------------------------------------------------//
	// rect
	//---------------------------------------------------------------------------//

	//!
	template <class T> struct rect_t
	{
		//!
		typedef glm::vec<2, T, glm::defaultp> vec2;
		//!
		vec2 lt, rb;

		//!
		const vec2 origin(void) const { return lt; }
		//!
		vec2 size(void) const { return rb - lt; }
		//!
		T width(void) const { return rb.x - lt.x; }
		//!
		T height(void) const { return rb.y - lt.y; }

		//!
		bool operator == (const rect_t& _rhs) const { return lt == _rhs.lt && rb == _rhs.rb; }
	};

	//!
	typedef rect_t<float> rect;
	//!
	typedef rect_t<int> irect;

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
