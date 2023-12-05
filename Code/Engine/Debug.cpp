#include "Debug.hpp"
#include "String.hpp"
#include <Windows.h>

namespace Engine
{
	//----------------------------------------------------------------------------//
	//
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	void LogMsg(LogLevel _level, const char* _func, const char* _file, int _line, const char* _msg, ...)
	{
		va_list _args;
		va_start(_args, _msg);
		Logger::MessageV(_level, _func, _file, _line, _msg, _args);
		va_end(_args);
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// Logger
	//----------------------------------------------------------------------------//

	//!
	Logger Logger::s_instance;
	//!
	uint Logger::s_mask = (uint)Logger::Mask::All;

	//----------------------------------------------------------------------------//
	Logger::Logger()
	{
		LOG_EVENT("Logger startup");
	}
	//----------------------------------------------------------------------------//
	Logger::~Logger()
	{
		LOG_EVENT("Logger shutdown");
		system("pause");
	}
	//----------------------------------------------------------------------------//
	void Logger::SetMask(uint _mask)
	{
		s_mask = _mask;
	}
	//----------------------------------------------------------------------------//
	uint Logger::GetMask(void)
	{
		return s_mask;
	}
	//----------------------------------------------------------------------------//
	void Logger::Message(LogLevel _level, const char* _func, const char* _file, int _line, const char* _msg, ...)
	{
		va_list _args;
		va_start(_args, _msg);
		MessageV(_level, _func, _file, _line, _msg, _args);
		va_end(_args);
	}
	//----------------------------------------------------------------------------//
	void Logger::MessageV(LogLevel _level, const char* _func, const char* _file, int _line, const char* _msg, va_list _args)
	{
		HANDLE _con = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO _ci;
		GetConsoleScreenBufferInfo(_con, &_ci);
		

		const char* _type = "";
		switch (_level)
		{
		case LogLevel::Assert:
			_type = "ASSERTION FAILED!";
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case LogLevel::Fatal:
			_type = "FATAL ERROR!";
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case LogLevel::Error:
			_type = "ERROR";
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case LogLevel::Warning:
			_type = "WARNING";
			SetConsoleTextAttribute(_con, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case LogLevel::Event:
			_type = "EVENT";
			break;
		case LogLevel::Info:
			_type = "INFO";
			break;
		case LogLevel::Debug:
			_type = "DEBUG INFO";
			SetConsoleTextAttribute(_con, FOREGROUND_INTENSITY);
			break;
		};

		//String _str = String::Format("%s(%d)<%s>: %s", _file, _line, _func, _type);
		
		if (s_mask & Mask::Timestamp)
		{

		}

		if (s_mask & Mask::FileLine)
		{
			printf("%s(%d)", _file, _line);
		}

		if (s_mask & Mask::Function)
		{
			printf("<%s>", _func);
		}

		if (s_mask & Mask::Level)
		{
			if ((s_mask & Mask::FileLine) && (s_mask & Mask::Function))
			{
				printf(": ");
			}
			printf("%s", _type);

		}

		if (s_mask & Mask::Message)
		{
			if (_msg)
			{
				if((s_mask & Mask::FileLine) || (s_mask & Mask::Function) || (s_mask & Mask::Level))
					printf(" : ");
				vprintf(_msg, _args);
			}
		}

		//printf("%s(%d)<%s>: %s", _file, _line, _func, _type);

		printf("\n");

		SetConsoleTextAttribute(_con, _ci.wAttributes);

		if (_level == LogLevel::Assert || _level == LogLevel::Fatal)
		{

			HANDLE _process = GetCurrentProcess();
			char _buff[1024];
			DWORD _size = sizeof(_buff);
			QueryFullProcessImageNameA(_process, 0, _buff, &_size);

			String _title = _buff;
			String _message;
			_title += ": ";
			_title += _level == LogLevel::Assert ? "Assertion Failed" : "Fatal error";
			_message += String::FormatV(_msg, _args);
			_message += "\n\n";
			_message += "File: " + String(_file) + "\n";
			_message += "Line: " + String::Format("%d", _line) + "\n";
			_message += "Function: " + String(_func) + "\n";


			int _ret = MessageBoxA(0, _message, _title, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_ICONERROR | MB_SYSTEMMODAL | MB_SETFOREGROUND);

			if (_ret == IDABORT)
			{
				DebugBreak();
			}
		}
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	//
	//----------------------------------------------------------------------------//
}
