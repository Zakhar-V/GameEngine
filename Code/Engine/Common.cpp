#include "Common.hpp"
#include "String.hpp"

#ifdef _WIN32
#	include <Windows.h>
#endif


namespace Engine
{
	//----------------------------------------------------------------------------//
	// Logger
	//----------------------------------------------------------------------------//

	//!
	Logger Logger::s_instance;
	//!
	int Logger::s_mask = (int)Logger::Mask::All;
	//!
	Logger::Callback Logger::s_callback = Logger::DefaultCallback;

	//----------------------------------------------------------------------------//
	Logger::Logger()
	{
		//setlocale(LC_ALL, "Russian");
		//setlocale(LC_NUMERIC, "English"); // for Tokenizer

		LOG_EVENT("Logger startup");
	}
	//----------------------------------------------------------------------------//
	Logger::~Logger()
	{
		LOG_EVENT("Logger shutdown");
#	ifdef DEBUG
		system("pause");
#	endif
	}
	//----------------------------------------------------------------------------//
	void Logger::SetMask(uint _mask)
	{
		s_mask = _mask;
	}
	//----------------------------------------------------------------------------//
	int Logger::GetMask(void)
	{
		return s_mask;
	}
	//----------------------------------------------------------------------------//
	void Logger::SetCallback(Callback _cb)
	{
		if (_cb != s_callback)
		{
			s_callback = _cb;
		}
	}
	//----------------------------------------------------------------------------//
	Logger::Callback Logger::GetCallback(void)
	{
		return s_callback;
	}
	//----------------------------------------------------------------------------//
	void Logger::Message(Level::Enum _level, const char* _func, const char* _file, int _line, const char* _msg, ...)
	{
		va_list _args;
		va_start(_args, _msg);
		MessageV(_level, _func, _file, _line, _msg, _args);
		va_end(_args);
	}
	//----------------------------------------------------------------------------//
	void Logger::MessageV(Level::Enum _level, const char* _func, const char* _file, int _line, const char* _msg, va_list _args)
	{
		MessageInfo _info;
		_info.level = _level;
		_info.mask = s_mask;
		_info.time = 0; // TODO
		_info.func = _func;
		_info.file = _file;
		_info.line = _line;

		String _tmpstr = String::FormatV(_msg, _args);
		_info.msg = _tmpstr;

		if (s_callback)
			s_callback(_info);
	}
	//----------------------------------------------------------------------------//
	void Logger::DefaultCallback(const MessageInfo& _msg)
	{
		const char* _type = "";
		switch (_msg.level)
		{
		case Level::Assert:
			_type = "ASSERTION FAILED!";
			break;
		case Level::Fatal:
			_type = "FATAL ERROR!";
			break;
		case Level::Error:
			_type = "ERROR";
			break;
		case Level::Warning:
			_type = "WARNING";
			break;
		case Level::Event:
			_type = "EVENT";
			break;
		case Level::Info:
			_type = "INFO";
			break;
		case Level::Debug:
			_type = "DEBUG INFO";
			break;
		};

#ifdef _WIN32
		HANDLE _con = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO _ci;
		GetConsoleScreenBufferInfo(_con, &_ci);

		switch (_msg.level)
		{
		case Level::Assert:
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case Level::Fatal:
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case Level::Error:
			SetConsoleTextAttribute(_con, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case Level::Warning:
			SetConsoleTextAttribute(_con, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case Level::Debug:
			SetConsoleTextAttribute(_con, FOREGROUND_INTENSITY);
			break;
		};

		//String _str = String::Format("%s(%d)<%s>: %s", _file, _line, _func, _type);

		if (s_mask & Mask::Timestamp)
		{
			// TODO
		}

		if (s_mask & Mask::FileLine)
		{
			printf("%s(%d)", _msg.file, _msg.line);
		}

		if (s_mask & Mask::Function)
		{
			printf("<%s>", _msg.func);
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
			if (_msg.msg)
			{
				if ((s_mask & Mask::FileLine) || (s_mask & Mask::Function) || (s_mask & Mask::Level))
					printf(" : ");
				printf(_msg.msg);
			}
		}
		printf("\n");

		SetConsoleTextAttribute(_con, _ci.wAttributes);

		if ((_msg.level == Level::Assert) || (_msg.level == Level::Fatal))
		{
			HANDLE _process = GetCurrentProcess();
			char _buff[1024];
			DWORD _size = sizeof(_buff);
			QueryFullProcessImageNameA(_process, 0, _buff, &_size);

			String _title = _buff;
			String _message;
			_title += ": ";
			_title += (_msg.level == Level::Assert) ? "Assertion Failed" : "Fatal error";
			_message += _msg.msg;
			_message += "\n\n";
			_message += "File: " + String(_msg.file) + "\n";
			_message += "Line: " + String::Format("%d", _msg.line) + "\n";
			_message += "Function: " + String(_msg.func) + "\n";

			int _ret = MessageBoxA(0, _message, _title, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_ICONERROR | MB_SYSTEMMODAL | MB_SETFOREGROUND);

			if (_ret == IDABORT)
			{
				DebugBreak();
			}
		}
#	else
		printf("%s(%d)<%s>: %s : %s\n", _msg.file, _msg.line, _msg.func, _msg.type, _msg.msg);
#	endif
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
