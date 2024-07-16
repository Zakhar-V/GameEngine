#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "Common.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------//
// List 
//----------------------------------------------------------------------------//

//! the simple linked list
struct List
{
	static void** _Field(void* _ptr, size_t _offset)
	{
		return (void**)((uint8*)_ptr + _offset);
	}

	static void _Link(void* _head, void* _this, void* _node)
	{

		/*next = head;
		if(next) next->prev = this;
		head = this;*/

		size_t _offset = (uint8*)_node - (uint8*)_this;
		void** _next = 1 + (void**)_node;
		*_next = *(void**)_head;
		if (*_next)
			_Field(*_next, _offset)[0] = _this;
		*(void**)_head = _this;
	}

	static void _Unlink(void* _head, void* _this, void* _node)
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
			_Field(*_next, _offset)[0] = *_prev;
		if (*_prev)
			_Field(*_prev, _offset)[1] = *_next;
		else if (*(void**)_head == _this)
			*(void**)_head = *_next;

		*_prev = nullptr;
		*_next = nullptr;
	}

	template <class T> static void Link(T*& _head, T* _this, T*& _node)
	{
		_Link(&_head, _this, &_node);
	}

	template <class T> static void Unlink(T*& _head, T* _this, T*& _node)
	{
		_Unlink(&_head, _this, &_node);
	}

	template <class T> struct Node
	{
		T* prev = nullptr;
		T* next = nullptr;
	};

	template <class T> static void Link(T*& _head, T* _this, Node<T>& _node)
	{
		_Link(&_head, _this, &_node.prev);
	}

	template <class T> static void Unlink(T*& _head, T* _this, Node<T>& _node)
	{
		_Unlink(&_head, _this, &_node.prev);
	}
};

//----------------------------------------------------------------------------//
//  
//----------------------------------------------------------------------------//
