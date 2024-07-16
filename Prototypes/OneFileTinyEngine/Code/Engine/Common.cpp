#include "Common.hpp"

namespace Tiny
{
	//----------------------------------------------------------------------------//
	// List 
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	void** _LL_Field(void* _ptr, size_t _offset)
	{
		return (void**)((uint8*)_ptr + _offset);
	}
	//----------------------------------------------------------------------------//
	void _LL_Link(void* _head, void* _this, void* _node)
	{
		/*
		next = head;
		if(next) next->prev = this;
		head = this;
		*/

		size_t _offset = (uint8*)_node - (uint8*)_this;
		void** _next = 1 + (void**)_node;
		*_next = *(void**)_head;
		if (*_next)
			_LL_Field(*_next, _offset)[0] = _this;
		*(void**)_head = _this;
	}
	//----------------------------------------------------------------------------//
	void _LL_Unlink(void* _head, void* _this, void* _node)
	{
		/*
		if (m_next) m_next->m_prev = m_prev;
		if (m_prev) m_prev->m_next = m_next;
		else if(head == this) head = m_next;
		*/

		size_t _offset = (uint8*)_node - (uint8*)_this;
		void** _prev = 0 + (void**)_node;
		void** _next = 1 + (void**)_node;

		if (*_next)
			_LL_Field(*_next, _offset)[0] = *_prev;
		if (*_prev)
			_LL_Field(*_prev, _offset)[1] = *_next;
		else if (*(void**)_head == _this)
			*(void**)_head = *_next;

		*_prev = nullptr;
		*_next = nullptr;
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
