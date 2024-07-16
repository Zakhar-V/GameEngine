#pragma once

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// types
	//---------------------------------------------------------------------------//

	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
	typedef unsigned int uint;

	//---------------------------------------------------------------------------//
	// Hash
	//---------------------------------------------------------------------------//

	//!
	inline constexpr uint ConstHash(const char* _str, uint _hash = 0) { return *_str ? ConstHash(_str + 1, *_str + (_hash << 6) + (_hash << 16) - _hash) : _hash; }
	//!
	inline uint Hash(const std::string& _str) { return ConstHash(_str.c_str()); }
	//! SDMB hash
	inline uint Hash(const void* _data, uint _size, uint _hash = 0)
	{
		const uint8* _start = reinterpret_cast<const uint8*>(_data);
		const uint8* _end = _start + _size;
		while (_start < _end)
			_hash = *_start++ + +(_hash << 6) + (_hash << 16) - _hash;
		return _hash;
	}
	//!
	template <class T> uint Hash(const T& _e) { return Hash(&_e, sizeof(T)); }

	//---------------------------------------------------------------------------//
	// DEBUG
	//---------------------------------------------------------------------------//

	//!
#	define ASSERT(x, ...)

	//---------------------------------------------------------------------------//
	// LOG
	//---------------------------------------------------------------------------//

	//!
#	define LOG(x, ...) { printf(x, ##__VA_ARGS__); printf("\n"); }

	//---------------------------------------------------------------------------//
	// Utilities
	//---------------------------------------------------------------------------//
	/*
	//!
#	define _LL_Link(head, node, prev, next) \
	{  \
		if (head) head->prev = node; \
		node->next = head; \
		head = node; \
	}
	//!
#	define LL_Link(head, node, ...) _LL_Link(head, node, m_prev##__VA_ARGS__, m_next##__VA_ARGS__)
	//!
#	define _LL_Unlink(head, node, prev, next) \
	{ \
		if (node->next) node->next->prev = node->prev; \
		if (node->prev) node->prev->next = node->next; \
		else /*if (head == this)* / head = node->next; \
		node->next = node->prev = nullptr; \
	}
	//!
#	define LL_Unlink(head, node, ...) _LL_Unlink(head, node, m_prev##__VA_ARGS__, m_next##__VA_ARGS__)
	*/

	//----------------------------------------------------------------------------//
	// Linked List utilities 
	//----------------------------------------------------------------------------//

	// nullptr<-head{prev, next}<->node{prev, next}<->node{prev, next}->nullptr
	// Link<Type>(m_head, node, node->m_prev)

	//!
	void _LL_Link(void* _head, void* _this, void* _node);
	//!
	void _LL_Unlink(void* _head, void* _this, void* _node);

	//!
	template <class T> void Link(T*& _head, T* _this, T*& _node) { _LL_Link(&_head, _this, &_node); }
	//!
	template <class T> void Unlink(T*& _head, T* _this, T*& _node) { _LL_Unlink(&_head, _this, &_node); }

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
