#pragma once

//----------------------------------------------------------------------------//
// Compiler and Platform
//----------------------------------------------------------------------------//

#define _QUOTE( x ) #x
#define _QUOTE_IN_PLACE( x ) _QUOTE( x )
#define __FILELINE__ __FILE__"(" _QUOTE_IN_PLACE( __LINE__ ) ")"

#ifdef _MSC_VER
#   define COMPILER_MESSAGE(_prefix, _message) __pragma( message( __FILELINE__ " : "_prefix ": " _message ) )
#   define DEPRECATED __declspec( deprecated( "It will be removed or changed in closest time" ) )
#   ifndef _CRT_SECURE_NO_WARNINGS
#       define _CRT_SECURE_NO_WARNINGS
#   endif
#   define PACK __pragma( pack( push, 1 ) )
#   define PACKED
#   define UNPACK ;__pragma( pack( pop ) )
#	define THREAD_LOCAL __declspec(thread)
#	define NOINLINE __declspec(noinline)
#	pragma warning( disable : 4251 ) // dll interface
#	pragma warning( disable : 4275 ) // dll interface
#	pragma warning( disable : 4201 ) // unnamed union
#	pragma warning( disable : 4100 ) // unused arg
#	pragma warning(disable : 4996)	// The POSIX name
#elif defined(__GNUC__)
#   define COMPILER_MESSAGE(_prefix, _message) __pragma( message( __FILELINE__ " : "_prefix ": " _message ) )
#   define DEPRECATED __declspec( deprecated( "It will be removed or changed in closest time" ) )
#   define PACK
#   define PACKED __attribute__((packed))
#   define UNPACK
#   define THREAD_LOCAL __thread
#	define NOINLINE __attribute__((noinline))
#	define abstract =0
#else
#   define COMPILER_MESSAGE(_prefix, _message)
#   define DEPRECATED
#   define PACK
#   define PACKED
#   define UNPACK
#   define THREAD_LOCAL
#	define NOINLINE
#endif

#define COMPILER_MESSAGE_EX(_prefix, _source, _message) COMPILER_MESSAGE(_prefix, _source " : " _message)
#define WARNING_EX(_source, _message) COMPILER_MESSAGE_EX("Warning", _source, _message)
#define WARNING(_message) WARNING_EX(__FUNCTION__, _message)
#define FIXME_EX(_source, _message) COMPILER_MESSAGE_EX("FixMe", _source, _message)
#define FIXME(_message) FIXME_EX(__FUNCTION__, _message)
#define TODO_EX(_source, _message) COMPILER_MESSAGE_EX("ToDo", _source, _message)
#define TODO(_message) TODO_EX(__FUNCTION__, _message)
#define NOT_IMPLEMENTED_YET() FIXME("Not implemented yet")
#define NOT_IMPLEMENTED_YET_EX(_source) FIXME_EX(_source, "Not implemented yet")

//----------------------------------------------------------------------------//
// Settings
//----------------------------------------------------------------------------//

#define ENGINE_STATIC_LIB // define it if you use the engine as static library.

//----------------------------------------------------------------------------//
// Import/Export
//----------------------------------------------------------------------------//

#ifdef ENGINE_EXPORT_SYMBOLS
#	define RX_API __declspec(dllexport)
#elif !defined(ENGINE_STATIC_LIB)
#	define ENGINE_API __declspec(dllimport)
#else
#	define ENGINE_API
#endif

//----------------------------------------------------------------------------//
// Assert
//----------------------------------------------------------------------------//

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
#endif
#if !defined(DEBUG) && !defined(NDEBUG)
#	define NDEBUG
#endif

#ifdef DEBUG
#	define ASSERT(x, ...) ((x) ? ((void)0) : (Engine::Logger::Message(Engine::Logger::Level::Assert, __FUNCTION__, __FILE__, __LINE__, #x " (" ##__VA_ARGS__ ")")))
#else
#	define ASSERT(x, ...)
#endif

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Types
	//----------------------------------------------------------------------------//

	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
	typedef unsigned int uint;

	//!
	enum class AccessMode : uint
	{
		None = 0,
		Read = 0x1,
		Write = 0x2,
		ReadWrite = Read | Write,
	};

	//!
	inline bool operator & (AccessMode _lhs, AccessMode _rhs) { return (uint)_lhs & (uint)_rhs; }

	//----------------------------------------------------------------------------//
	// Debug
	//----------------------------------------------------------------------------//

#	define LOG_MSG(level, msg, ...) Engine::Logger::Message(level, __FUNCTION__, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#	define LOG_FATAL(msg, ...) LOG_MSG(Engine::Logger::Level::Fatal, msg, ##__VA_ARGS__)
#	define LOG_ERROR(msg, ...) LOG_MSG(Engine::Logger::Level::Error, msg, ##__VA_ARGS__)
#	define LOG_WARNING(msg, ...) LOG_MSG(Engine::Logger::Level::Warning, msg, ##__VA_ARGS__)
#	define LOG_EVENT(msg, ...) LOG_MSG(Engine::Logger::Level::Event, msg, ##__VA_ARGS__)
#	define LOG_INFO(msg, ...) LOG_MSG(Engine::Logger::Level::Info, msg, ##__VA_ARGS__)
#	define LOG_DEBUG(msg, ...) LOG_MSG(Engine::Logger::Level::Debug, msg, ##__VA_ARGS__)

	class ENGINE_API Logger final
	{
	public:

		//!
		struct Level
		{
			enum Enum : int
			{
				Assert = 0x1,
				Fatal = 0x2,
				Error = 0x4,
				Warning = 0x8,
				Event = 0x10,
				Info = 0x20,
				Debug = 0x40,
			};
		};

		//!
		struct Mask
		{
			enum Enum : int
			{
				Level = 0x1,
				Timestamp = 0x2,
				FileLine = 0x4,
				Function = 0x8,
				Message = 0x10,
				All = Level | Timestamp | Function | FileLine | Message,
			};
		};

		//!
		struct MessageInfo
		{
			int level;
			int mask;
			uint64 time; // ?
			const char* func;
			const char* file;
			int line;
			const char* msg;
		};
		//!
		typedef void(*Callback)(const MessageInfo&);

		//!
		static void SetMask(uint _mask);
		//!
		static int GetMask(void);
		//!
		static void SetCallback(Callback _cb);
		//!
		static Callback GetCallback(void);
		//!
		static void Message(Level::Enum _level, const char* _func, const char* _file, int _line, const char* _msg, ...);
		//!
		static void MessageV(Level::Enum _level, const char* _func, const char* _file, int _line, const char* _msg, va_list _args);

		//!
		static void DefaultCallback(const MessageInfo& _msg);

	private:
		//!
		Logger(void);
		//!
		~Logger(void);
		//!
		Logger(const Logger&) = delete;
		//!
		Logger& operator = (const Logger&) = delete;

		//!
		static int s_mask;
		//!
		static Callback s_callback;
		//!
		static Logger s_instance;
	};

	//----------------------------------------------------------------------------//
	// Move semantics
	//----------------------------------------------------------------------------//

	//!\addtogroup Container 
	//!\{

	template <class T> struct BaseType { typedef T Type; };
	template <class T> struct BaseType<T&> { typedef T Type; };
	template <class T> struct BaseType<T&&> { typedef T Type; };

	//! Get a rvalue reference.
	template <class T> inline constexpr typename T&& Forward(typename BaseType<T>::Type& _ref) { return static_cast<T&&>(_ref); }
	//! Get a rvalue reference.
	template <class T> inline constexpr typename T&& Forward(typename BaseType<T>::Type&& _ref) { return static_cast<T&&>(_ref); }
	//! Get a rvalue reference.
	template <class T> inline typename BaseType<T>::Type&& Move(T&& _ref) { return static_cast<BaseType<T>::Type&&>(_ref); }

	//! Swap elements.
	template <class T> void Swap(T& _a, T& _b)
	{
		T _c = Move(_a);
		_a = Move(_b);
		_b = Move(_c);
	}

	//!\} Container

	//----------------------------------------------------------------------------//
	// Function
	//----------------------------------------------------------------------------//

	//!
	template <class F> void* FuncPtr(F _func) { union { F f; void* p; }_fp = { _func }; return _fp.p; }
	//!
	template <class F> F FuncCast(void* _func) { union { void* p; F f; }_fp = { _func }; return _fp.f; }

	// example:
	// auto func = Function<void(int)>(Func); // c-func
	// func = Function<void(int)>(&var, &MyClass::Func); // method
	// func(0);	// call

	//!
	template <class F> struct Function;
	//!
	template <class R, class... A> struct Function<R(A...)>
	{
		// TODO: calling convention
		//!
		typedef R(*Invoke)(void*, void*, A&&...);

		//!
		typedef R(*Ptr)(A...);
		//!
		typedef R(Type)(A...);

		//!
		Invoke invoke;
		//!
		void* func;
		//!
		void* self;

		//!
		Function(void) : invoke(nullptr), func(nullptr), self(nullptr) { }
		//!
		Function(R(*_func)(A...)) : invoke(InvokeFunc), func(FuncPtr(_func)), self(nullptr) { }
		//!
		template <class C> Function(C* _self, R(C::* _func)(A...)) : invoke(InvokeMethod<C>), func(FuncPtr(_func)), self(_self) { ASSERT(_self != nullptr); }
		//!
		template <class C> Function(const C* _self, R(C::* _func)(A...) const) : invoke(InvokeConstMethod<C>), func(FuncPtr(_func)), self(const_cast<C*>(_self)) { ASSERT(_self != nullptr); }
		//!
		operator bool(void) const { return func != nullptr; }

		//!
		R operator () (A... _args) const
		{
			ASSERT(func != nullptr);
			return invoke(self, func, Forward<A>(_args)...);
		}

		//!
		static R InvokeFunc(void* _self, void* _func, A&&... _args)
		{
			typedef R(*Func)(A...);
			return FuncCast<Func>(_func)(Forward<A>(_args)...);
		}

		//!
		template <class C> static R InvokeMethod(void* _self, void* _func, A&&... _args)
		{
			ASSERT(_self != nullptr);
			typedef R(C::* Func)(A...);
			return (*((C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
		}

		//!
		template <class C> static R InvokeConstMethod(void* _self, void* _func, A&&... _args)
		{
			ASSERT(_self != nullptr);
			typedef R(C::* Func)(A...) const;
			return (*((const C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
		}
	};

	//!
	template <class R, class... A> Function<R(A...)> MakeFunction(R(*_func)(A...))
	{
		return Function<R(A...)>(_func);
	}
	//!
	template <class C, class R, class... A> Function<R(A...)> MakeFunction(C* _self, R(C::* _func)(A...))
	{
		return Function<R(A...)>(_self, _func);
	}
	//!
	template <class C, class R, class... A> Function<R(A...)> MakeFunction(const C* _self, R(C::* _func)(A...) const)
	{
		return Function<R(A...)>(_self, _func);
	}

	//----------------------------------------------------------------------------//
	// NonCopyable
	//----------------------------------------------------------------------------//

	//!
	class NonCopyable
	{
	public:
		//!
		NonCopyable(void) { }
		//!
		~NonCopyable(void) { }

	private:
		//!
		NonCopyable(const NonCopyable&) = delete;
		//!
		NonCopyable& operator = (const NonCopyable&) = delete;
	};

	//----------------------------------------------------------------------------//
	// Singleton
	//----------------------------------------------------------------------------//

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

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
