#pragma once

#include "Common.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Logger
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Logger : public NonCopyable
	{
	public:
		enum class Mask	: uint
		{
			Level = 0x1,
			Timestamp = 0x2,
			FileLine = 0x4,
			Function = 0x8,
			Message = 0x10,
			All = Level | Timestamp | Function | FileLine | Message,
		};

		//!
		static void SetMask(uint _mask);
		//!
		static uint GetMask(void);

		//!
		static void Message(LogLevel _level, const char* _func, const char* _file, int _line, const char* _msg, ...);
		//!
		static void MessageV(LogLevel _level, const char* _func, const char* _file, int _line, const char* _msg, va_list _args);

	//protected:
		//!
		Logger();
		//!
		~Logger();

		//!
		static uint s_mask;

		//!
		static Logger s_instance;
	};
	 
	//!
	inline uint operator | (Logger::Mask _lhs, Logger::Mask _rhs) { return (uint)_lhs | (uint)_rhs; }
	//!
	inline uint operator & (uint _lhs, Logger::Mask _rhs) { return (uint)_lhs & (uint)_rhs; }

	//----------------------------------------------------------------------------//
	// Profiler
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Profiler
	{
	public:

		//!
		class Section
		{

		};

	protected:
	};

	//----------------------------------------------------------------------------//
	// Console
	//----------------------------------------------------------------------------//

	class ENGINE_API Console
	{

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}

