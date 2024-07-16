#pragma once

#include "Common.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// NonCopyable
	//---------------------------------------------------------------------------//

	//!
	class NonCopyable
	{
	public:
		//!
		NonCopyable(void) = default;
		//!
		~NonCopyable(void) = default;

	private:

		//!
		NonCopyable(const NonCopyable&) = delete;
		//!
		NonCopyable& operator = (const NonCopyable&) = delete;
	};

	//---------------------------------------------------------------------------//
	// Singleton
	//---------------------------------------------------------------------------//

	//!
	template <class T> class Singleton : public NonCopyable
	{
	public:
		//!
		static T* Get(void) { return s_instance; }
		//!
		Singleton(void) { ASSERT(s_instance == nullptr); s_instance = static_cast<T*>(this); }
		//!
		~Singleton(void) { s_instance = nullptr; }

	protected:
		//!
		static T* s_instance;
	};

	template <class T> T* Singleton<T>::s_instance = nullptr;

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
