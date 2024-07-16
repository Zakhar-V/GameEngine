#include "Graphics.hpp"
#include "Frame.hpp"
#include "Window.hpp"

#include "GLFW/glfw3.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
//#include <GL/wglext.h>

//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//

#define _GLFUNC(R, N, ...) extern R (APIENTRY*N)(##__VA_ARGS__)
#define _EOL ;
#include "OpenGL.inl"

#define _GLFUNC(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__) = nullptr
#define _EOL ;
#include "OpenGL.inl"

namespace Tiny
{
	//----------------------------------------------------------------------------//
	// LOADER
	//----------------------------------------------------------------------------//
	
	//----------------------------------------------------------------------------//
	bool LoadOpenGL(void)
	{
		static const char* _names[] =
		{
#			define _GLFUNC(R, N, ...) #N
#			define _EOL ,
#			include "OpenGL.inl"
		};

		static void** _funcs[] =
		{
#			define _GLFUNC(R, N, ...) (void**)&N
#			define _EOL ,
#			include "OpenGL.inl"
		};

		int _loaded = 0;
		for (uint i = 0; i < sizeof(_funcs) / sizeof(void*); ++i)
		{
			*_funcs[i] = glfwGetProcAddress(_names[i]); //wglGetProcAddress(_names[i]);
			if (*_funcs[i])
			{
				++_loaded;
#				ifdef _DEBUG
				LOG("OpenGL: %s is loaded", _names[i]);
#				endif
			}
			else
			{
				LOG("OpenGL: %s is unavailable", _names[i]);
			}
		}

		LOG("OpenGL: Functions loaded %d/%d", _loaded, sizeof(_funcs) / sizeof(void*));

		return _loaded == (sizeof(_funcs) / sizeof(void*));
	}
	//----------------------------------------------------------------------------//
	bool HasOpenGLExtension(const char* _ext)
	{
		const char* e = (const char*)glGetStringi(GL_EXTENSIONS, 0);
		for (int i = 0; e; e = (const char*)glGetStringi(GL_EXTENSIONS, ++i))
		{
			if (!strcmp(e, _ext))
				return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------//
	 
	//----------------------------------------------------------------------------//
	// GLDebug
	//----------------------------------------------------------------------------//

	struct GLDebug : public NonCopyable
	{
		static bool Enabled;

		GLDebug(const char* _func, const char* _file, int _line, const char* _desc = nullptr) :
			func(_func),
			desc(_desc),
			file(_file),
			line(_line),
			prev(Top)
		{
			Top = this;
		}

		~GLDebug(void)
		{
			Top = prev;
		}

		static void __stdcall OnMessage(uint _source, uint _type, uint _id, uint _severity, int _length, const char* _message, const void* _ud)
		{
			const char* _src;
			const char* _typeStr;
			const char* _levelStr;
			int  _level = 0;
			const char* _func = nullptr;
			const char* _file = nullptr;
			int _line = 0;

			switch (_source)
			{
			case GL_DEBUG_SOURCE_API_ARB:
				_src = "OpenGL";
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
				_src = "Window system";
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
				_src = "Shader compiler";
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
				_src = "Third party";
				break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB:
				_src = "Application";
				break;
			case GL_DEBUG_SOURCE_OTHER_ARB:
				_src = "Other";
				break;
			default:
				return;
			}

			switch (_type)
			{
			case GL_DEBUG_TYPE_ERROR_ARB:
				_typeStr = "Error";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
				_typeStr = "Deprecated behavior";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
				_typeStr = "Undefined behavior";
				break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB:
				_typeStr = "Portability";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB:
				_typeStr = "Performance";
				break;
			case GL_DEBUG_TYPE_OTHER_ARB:
				_typeStr = "Message";
				break;
#if 0
			case GL_DEBUG_TYPE_MARKER:
				_dtype = "Marker";
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				_dtype = "Push group";
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				_dtype = "Pop group";
				break;
#endif
			default:
				return;
			}

			switch (_severity)
			{
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				_levelStr = "high";
				_level = 1;
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				_levelStr = "medium";
				_level = 0;
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				_levelStr = "low";
				_level = -1;
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				_levelStr = "notification";
				_level = -2;
				break;
			default:
				return;
			}

			if (_level >= 0 && Enabled)
			{
				if (Top)
				{
					if (Top->desc)
					{

						LOG("%s(%d)[%s [%s]]: %s: %s(%s): %s", Top->file, Top->line, Top->func, Top->desc, _src, _typeStr, _levelStr, _message);
					}
					else
					{
						LOG("%s(%d)[%s]: %s: %s(%s): %s", Top->file, Top->line, Top->func, _src, _typeStr, _levelStr, _message);
					}
				}
				else
					LOG("%s: %s(%s): %s", _src, _typeStr, _levelStr, _message);
			}
		}

		static void _Init(void)
		{
			if (glDebugMessageCallback)
			{
				Enabled = true;
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(&OnMessage, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, false);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
			}
		}

		const char* func;
		const char* desc;
		const char* file;
		int line;
		GLDebug* prev;
		static GLDebug* Top;
	};

	bool GLDebug::Enabled = false;
	GLDebug* GLDebug::Top = nullptr;

	//----------------------------------------------------------------------------//
	// GL_DEBUG helper
	//----------------------------------------------------------------------------//

#define _DEBUG_GRAPHICS

#ifdef _DEBUG_GRAPHICS
#	define GL_DEBUG(...) GLDebug _gldbg_(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#	define GL_DEBUG(...)
#endif
	//----------------------------------------------------------------------------//
	// Utils
	//----------------------------------------------------------------------------//

	void glEnableEx(uint _state, bool _enabled)
	{
		_enabled ? glEnable(_state) : glDisable(_state);
	}

	//---------------------------------------------------------------------------//
	// DepthStencilState
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	void DepthStencilState::_Bind(uint _ref)
	{
		GL_DEBUG();

		glEnableEx(GL_DEPTH_TEST, m_desc.depthTest);
		glDepthMask((GLenum)m_desc.depthWrite);
		glDepthFunc((GLenum)m_desc.depthFunc);

		if (m_desc.stencilTest)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilMask((GLenum)m_desc.stencilMask);
			glStencilFunc((GLenum)m_desc.stencilfunc, _ref, (GLenum)m_desc.stencilMask);
			glStencilOp((GLenum)m_desc.stencilFail, (GLenum)m_desc.depthFail, (GLenum)m_desc.depthPass);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// BlendState 
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	void BlendState::_Bind(void)
	{
		if (m_desc.enabled)
		{
			glEnable(GL_BLEND);
			glBlendFunc((GLenum)m_desc.src, (GLenum)m_desc.dst);
			glBlendEquation((GLenum)m_desc.mode);
		}
		else
		{
			glDisable(GL_BLEND);
		}
		glColorMask(m_desc.write, m_desc.write, m_desc.write, m_desc.write);
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// RasterizerState 
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	void RasterizerState::_Bind(void)
	{

	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// PolygonMode 
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	void PolygonMode::_Bind(void)
	{
		//glPolygonMode();
		// glCullFace()
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// VertexFormat
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	VertexFormat::VertexFormat(const Desc& _desc) : m_desc(_desc)
	{
		for (uint i = 0; i < _desc.numAttribs; ++i)
			m_attribMask |= 1 << _desc.attribs[i].index;
	}
	//---------------------------------------------------------------------------//
	void VertexFormat::_Bind(uint& _attribsMask)
	{
		GL_DEBUG();

		for (uint i = 0; i < m_desc.numAttribs; ++i)
		{
			const Attrib& _attrib = m_desc.attribs[i];
			glVertexAttribBinding(_attrib.index, _attrib.stream);
			glVertexAttribFormat(_attrib.index, _attrib.components, (GLenum)_attrib.type, _attrib.normalized, _attrib.offset);
		}

		for (uint i = 0; i < m_desc.numStreams; ++i)
		{
			const Stream& _stream = m_desc.streams[i];
			glVertexBindingDivisor(_stream.index, _stream.divisor);
		}

		for (uint i = 0; i < MaxAttribs; ++i)
		{
			uint _bit = 1 << i;
			bool _enabled = _attribsMask & _bit;
			bool _enable = m_attribMask & _bit;
			if (_enabled != _enable)
			{
				if (_enable)
					glEnableVertexAttribArray(i);
				else
					glDisableVertexAttribArray(i);
			}
		}
		_attribsMask = m_attribMask;
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// Buffer
	//---------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	Buffer::Buffer(Usage _usage) : m_usage(_usage)
	{
		GL_DEBUG();

		glGenBuffers(1, &m_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);

#ifdef _STATISTICS
		gGraphics->stats.bufferCount++;
#endif
	}
	//----------------------------------------------------------------------------//
	Buffer::~Buffer(void)
	{
		GL_DEBUG();

		if (m_handle)
			glDeleteBuffers(1, &m_handle);

#ifdef _STATISTICS
		gGraphics->stats.bufferMemory -= m_size;
		gGraphics->stats.bufferCount--;
#endif
	}
	//----------------------------------------------------------------------------//
	void Buffer::Realloc(uint _newSize, const void* _data, bool _discard)
	{
		GL_DEBUG();

		if (_discard && _data)
		{
			// https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
			Realloc(_newSize, nullptr, false);
			Write(_data, 0, _newSize);
			return;
		}

		if (!_newSize && _data)
			_data = nullptr;

#ifdef _STATISTICS
		gGraphics->stats.bufferMemory -= m_size;
		gGraphics->stats.bufferMemory += _newSize;
		if (_data)
			gGraphics->stats.frame.bufferWrite += _newSize;
#endif

		glNamedBufferDataEXT(m_handle, _newSize, _data, (GLenum)m_usage);
		m_size = _newSize;
	}
	//----------------------------------------------------------------------------//
	uint8* Buffer::Map(LockMode _mode, uint _offset, int _size)
	{
		GL_DEBUG();

		ASSERT(_offset <= m_size);

		if (_size < 0)
			_size = m_size - _offset;
		ASSERT((uint)(_offset + _size) <= m_size);

#ifdef _STATISTICS
		switch (_mode)
		{
		case LockMode::ReadOnly:
			gGraphics->stats.frame.bufferRead += _size;
			break;
		case LockMode::WriteDiscard:
			gGraphics->stats.frame.bufferWrite += _size;
			break;
		case LockMode::WriteNoOverwrite:
			gGraphics->stats.frame.bufferRead += _size; // ?
			gGraphics->stats.frame.bufferWrite += _size;
			break;
		}
#endif

		return (uint8*)glMapNamedBufferRangeEXT(m_handle, _offset, _size, (GLenum)_mode);
	}
	//----------------------------------------------------------------------------//
	void Buffer::Unmap(void)
	{
		GL_DEBUG();

		glUnmapNamedBufferEXT(m_handle);
	}
	//----------------------------------------------------------------------------//
	void Buffer::Write(const void* _src, uint _offset, uint _size)
	{
		GL_DEBUG();

		ASSERT(_src || !_size);
		ASSERT((uint)(_offset + _size) <= m_size);

		glNamedBufferSubDataEXT(m_handle, _offset, _size, _src);
#ifdef _STATISTICS
		gGraphics->stats.frame.bufferWrite += _size;
#endif
	}
	//----------------------------------------------------------------------------//
	void Buffer::Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size)
	{
		GL_DEBUG();

		ASSERT(_src != nullptr);
		ASSERT(_dstOffset + _size <= m_size);
		ASSERT(_srcOffset + _size <= _src->m_size);

		glNamedCopyBufferSubDataEXT(_src->m_handle, m_handle, _srcOffset, _dstOffset, _size);
	}
	//----------------------------------------------------------------------------//
	void Buffer::ClearStream(bool _invalidate)
	{
		if (_invalidate)
			m_uploaded = false;
		m_streamData.clear();
	}
	//----------------------------------------------------------------------------//
	void Buffer::InvalidateStream(void)
	{
		m_uploaded = false;
	}
	//----------------------------------------------------------------------------//
	uint8* Buffer::ReallocStreamData(uint _size)
	{
		m_uploaded = false;
		m_streamData.resize(_size);
		return m_streamData.data();
	}
	//----------------------------------------------------------------------------//
	uint8* Buffer::AppendStreamData(uint _size)
	{
		uint _offset = m_streamData.size();
		m_uploaded = false;
		m_streamData.resize((size_t)_offset + _size);
		return m_streamData.data() + _offset;
	}
	//----------------------------------------------------------------------------//
	void Buffer::AppendStreamData(const void* _data, uint _size)
	{
		memcpy(AppendStreamData(_size), _data, _size);
	}
	//----------------------------------------------------------------------------//
	void Buffer::UploadStream(void)
	{
		if (!m_uploaded)
		{
			m_uploaded = true;
			Realloc(m_streamData.size(), m_streamData.data(), true);
		}
	}
	//----------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// PixelFormat
	//---------------------------------------------------------------------------//

	const PixelFormat::Desc PixelFormat::s_desc[] =
	{
		{"Unknown", 0, 0, 0, 0, false },
		{"RGB8", 24, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, false },
		{"RGBA8", 32, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, false },
		{"RGB10A2", 32, GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV, false },
		{"R32F", 32, GL_R32F, GL_RED, GL_FLOAT, false },
		{"RG32F", 64, GL_RG32F, GL_RG, GL_FLOAT, false },
		{"RGB32F", 96, GL_RGB32F, GL_RGB, GL_FLOAT, false },
		{"RGBA32F", 128, GL_RGBA32F, GL_RGBA, GL_FLOAT, false },
		{"RGBA16F", 64, GL_RGBA16F, GL_RGBA, GL_FLOAT, false },
		{"RG11B10F",  32, GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, false },
		{"D24S8", 32, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false },
		{"DXT1", 4, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0, 0, true },
		{"DXT5", 8, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0, true },
	};

	//---------------------------------------------------------------------------//
	// TextureBuffer
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	TextureBuffer::TextureBuffer(PixelFormat::ID _format) : Buffer(Usage::Dynamic), m_format(_format)
	{
		GL_DEBUG();

		glGenTextures(1, &m_texture);
		glTextureBufferEXT(m_texture, GL_TEXTURE_BUFFER, PixelFormat::GetDesc(m_format).iformat, m_handle);
	}
	//---------------------------------------------------------------------------//
	TextureBuffer::~TextureBuffer(void)
	{
		GL_DEBUG();

		glDeleteTextures(1, &m_texture);
	}
	//---------------------------------------------------------------------------//
	void TextureBuffer::_Bind(uint _slot, TextureBuffer* _buffer)
	{
		glBindMultiTextureEXT(GL_TEXTURE0 + _slot, GL_TEXTURE_BUFFER, _buffer ? _buffer->m_texture : 0);
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// Texture
	//---------------------------------------------------------------------------//

	uint TextureType2GL[] =
	{
		GL_TEXTURE_2D, // Quad
		GL_TEXTURE_3D, // Volume
		GL_TEXTURE_2D_ARRAY, // Array
		GL_TEXTURE_CUBE_MAP, // Cube
	};

	const uint Texture::MinDepth[] =
	{
		1, // Quad
		1, // Volume
		1, // Array
		6, // Cube
	};

	const uint Texture::MaxDepth[] =
	{
		1, // Quad
		4096, // Volume
		4096, // Array
		6, // Cube
	};

	//---------------------------------------------------------------------------//
	Texture::Texture(void)
	{
		_ResetParams();
	}
	//---------------------------------------------------------------------------//
	Texture::~Texture(void)
	{
		_DeleteHandle();
	}
	//---------------------------------------------------------------------------//
	void Texture::Init(Type _type, PixelFormat::ID _format, int _lods)
	{
		if (m_type == _type && m_format == _format && m_desiredLods == _lods && m_initialized) // no changes
			return;

		m_type = _type;
		m_format = _format;
		m_desiredLods = _lods;

		_ResetParams();
		_CreateHandle();
	}
	//---------------------------------------------------------------------------//
	void Texture::Realloc(uint _width, uint _height, uint _depth)
	{
		GL_DEBUG();

		if (!_depth)
			_depth = m_depth;
		_depth = glm::clamp<uint>(_depth, MinDepth[(uint)m_type], MaxDepth[(uint)m_type]);

		if (m_width == _width && m_height == _height && m_depth == _depth) // no changes
			return;

		if (!m_initialized)
			_CreateHandle();

		m_width = _width;
		m_height = _height;
		m_depth = _depth;
		m_invSize.x = m_width > 0 ? (1.f / m_width) : 0;
		m_invSize.y = m_height > 0 ? (1.f / m_height) : 0;

		uint _side = max(m_width, m_height);
		uint _maxLods = _side ? glm::log2<int>(_side) : 1;
		m_lods = (m_desiredLods < 0) ? _maxLods : glm::clamp<uint>(m_desiredLods, 1, _maxLods);
		uint _iformat = PixelFormat::GetDesc(m_format).iformat;
		uint _target = TextureType2GL[(uint)m_type];

		if (m_type == Type::Quad || m_type == Type::Cube)
			glTextureStorage2DEXT(m_handle, _target, m_lods, _iformat, m_width, m_height);
		else
			glTextureStorage3DEXT(m_handle, _target, m_lods, _iformat, m_width, m_height, m_depth);

	}
	//---------------------------------------------------------------------------//
	void Texture::Write(PixelFormat::ID _format, uint _x, uint _y, uint _z, uint _width, uint _height, uint _depth, const void* _data)
	{
		GL_DEBUG();

		ASSERT(m_initialized == true);
		ASSERT(_data != nullptr);
		ASSERT(_x + _width <= m_width);
		ASSERT(_y + _height <= m_height);
		ASSERT(_z + _depth <= m_depth);

		const PixelFormat::Desc& _pf = PixelFormat::GetDesc(_format);
		bool _compressed = _pf.compressed;
		uint _bpl = (_width * _height * _pf.bpp) >> 3;
		uint _bpi = _bpl * _depth;
		uint _target = TextureType2GL[(uint)m_type];

		if (m_type == Type::Quad)
		{
			if (_compressed)
				glCompressedTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.iformat, _bpi, _data);
			else
				glTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.format, _pf.type, _data);
		}
		else if (m_type == Type::Volume || m_type == Type::Array)
		{
			if (_compressed)
				glCompressedTextureSubImage3DEXT(m_handle, _target, 0, _x, _y, _z, _width, _height, _depth, _pf.iformat, _bpi, _data);
			else
				glTextureSubImage3DEXT(m_handle, _target, 0, _x, _y, _z, _width, _height, _depth, _pf.format, _pf.type, _data);
		}
		else for (uint i = 0; i < 6; ++i) // Cube
		{
			_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			const void* _img = ((const uint8*)_data) + _bpl * i;
			if (_compressed)
				glCompressedTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.iformat, _bpl, _img);
			else
				glTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.format, _pf.type, _img);
		}
	}
	//---------------------------------------------------------------------------//
	void Texture::GenerateMipmap(void)
	{
		GL_DEBUG();

		if (m_lods && m_handle)
			glGenerateTextureMipmapEXT(m_handle, TextureType2GL[(uint)m_type]);
	}
	//---------------------------------------------------------------------------//
	void Texture::_CreateHandle(void)
	{
		GL_DEBUG();

		_DeleteHandle();
		glGenTextures(1, &m_handle);
		_Bind(MaxUnits + 1, this);
		m_initialized = true;
	}
	//---------------------------------------------------------------------------//
	void Texture::_DeleteHandle(void)
	{
		GL_DEBUG();

		if (m_handle)
		{
			glDeleteTextures(1, &m_handle);
			m_handle = 0;
		}
	}
	//---------------------------------------------------------------------------//
	void Texture::_ResetParams(void)
	{
		m_lods = 0;
		m_width = 0;
		m_height = 0;
		m_depth = MinDepth[(uint)m_type];
		m_initialized = false;
	}
	//---------------------------------------------------------------------------//
	void Texture::_Bind(uint _slot, Texture* _texture)
	{
		GL_DEBUG();

		uint _target = GL_TEXTURE_2D;
		uint _handle = 0;
		if (_texture)
		{
			_target = TextureType2GL[(uint)_texture->m_type];
			_handle = _texture->m_handle;
		}
		glBindMultiTextureEXT(GL_TEXTURE0 + _slot, _target, _handle);
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// Sampler
	//---------------------------------------------------------------------------//
	
	const uint MinFilter2GL[] =
	{
		GL_NEAREST, // Nearest
		GL_LINEAR, // Linear
		GL_LINEAR_MIPMAP_LINEAR, // Trilinear
	};

	const uint MagFilter2GL[] =
	{
		GL_NEAREST, // Nearest
		GL_LINEAR, // Linear
		GL_LINEAR, // Trilinear
	};

	//---------------------------------------------------------------------------//
	Sampler::Sampler(const Desc& _desc)
	{
		GL_DEBUG();

		glGenSamplers(1, &m_handle);
		glBindSampler(Texture::MaxUnits + 1, m_handle);
		glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, MinFilter2GL[(uint)_desc.filter]);
		glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, MagFilter2GL[(uint)_desc.filter]);
		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, (GLenum)_desc.wrap);
		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, (GLenum)_desc.wrap);
		//glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glSamplerParameteri(m_handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, _desc.anisotropy);
		if (_desc.depthFunc != CompareFunc::Never)
		{
			glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, (GLenum)_desc.depthFunc);
		}
	}
	//---------------------------------------------------------------------------//
	Sampler::~Sampler(void)
	{
		glDeleteSamplers(1, &m_handle);
	}
	//---------------------------------------------------------------------------//
	void Sampler::_Bind(void)
	{

	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// Graphics
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	Graphics::Graphics(void)
	{
		// set glfw window hints

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // 
		glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, true);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	}
	//---------------------------------------------------------------------------//
	bool Graphics::Startup(void)
	{
		LOG("Startup: Graphics");

		glfwMakeContextCurrent(gWindow->_GetHandle());

		LOG("OpenGL: %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

		if (!LoadOpenGL())
		{
			LOG("Error: The required version of OpenGL is not supported on this device");
			return false;
		}

		{
			int _p[4];
			glGetIntegerv(GL_MAJOR_VERSION, _p);
			glGetIntegerv(GL_MINOR_VERSION, _p + 1);
			int _ver = _p[0] * 10 + _p[1];

			if (HasOpenGLExtension("GL_ARB_separate_shader_objects") || _ver >= 41)
			{
				LOG("OpenGL: GL_ARB_separate_shader_objects is supported")
			}
			else
			{
				LOG("OpenGL: Error! GL_ARB_separate_shader_objects is unsupported");
				return false;
			}

			if (HasOpenGLExtension("GL_ARB_vertex_attrib_binding") || _ver >= 43)
			{
				LOG("OpenGL: GL_ARB_vertex_attrib_binding is supported")
			}
			else
			{
				LOG("OpenGL: Error! GL_ARB_vertex_attrib_binding is unsupported");
				return false;
			}

			if (HasOpenGLExtension("GL_ARB_shading_language_420pack") || _ver >= 42)
			{
				LOG("OpenGL: GL_ARB_shading_language_420pack is supported")
			}
			else
			{
				LOG("OpenGL: Error! GL_ARB_shading_language_420pack is unsupported");
				return false;
			}

			if (HasOpenGLExtension("GL_ARB_texture_storage") || _ver >= 42)
			{
				LOG("OpenGL: GL_ARB_texture_storage is supported")
			}
			else
			{
				LOG("OpenGL: Error! GL_ARB_texture_storage is unsupported");
				return false;
			}

			if (HasOpenGLExtension("GL_EXT_direct_state_access"))
			{
				LOG("OpenGL: GL_EXT_direct_state_access is supported")
			}
			else
			{
				LOG("OpenGL: Error! GL_EXT_direct_state_access is unsupported");
				return false;
			}

			if (HasOpenGLExtension("GL_ARB_debug_output") || _ver >= 43)
			{
				LOG("OpenGL: GL_ARB_debug_output is supported")
			}

			//"GL_ARB_texture_storage_multisample") || _ver >= 43
			//"GL_ARB_enhanced_layouts") || _ver >= 44
		}

		GLDebug::_Init();

		// depth stencil

		m_firstDepthStencilState = new DepthStencilState(DepthStencilState::Desc()); // default
		m_currentDepthStencilState = nullptr;
		m_currentStencilRef = 0;

		//blend

		m_firstBlendState = new BlendState(BlendState::Desc()); // default;
		m_currentBlendState = nullptr;

		// rasterizer

		m_firstRasterizerState = new RasterizerState(RasterizerState::Desc()); // default;
		m_currentRasterizerState = nullptr;

		// polygon mode

		m_firstPolygonMode = new PolygonMode(PolygonMode::Desc()); // default;
		m_currentPolygonMode = nullptr;

		// vertex format

		m_firstVertexFormat = new VertexFormat(VertexFormat::Desc()); // default;
		m_currentVertexFormat = nullptr;
		
		// sampler

		m_firstSampler = new Sampler(Sampler::Desc()); // default;



		return true;
	}
	//---------------------------------------------------------------------------//
	void Graphics::Shutdown(void)
	{
		LOG("Shutdown: Graphics");

		while (m_firstDepthStencilState)
		{
			auto i = m_firstDepthStencilState->m_next;
			delete m_firstDepthStencilState;
			m_firstDepthStencilState = i;
		}

		while (m_firstBlendState)
		{
			auto i = m_firstBlendState->m_next;
			delete m_firstBlendState;
			m_firstBlendState = i;
		}

		while (m_firstRasterizerState)
		{
			auto i = m_firstRasterizerState->m_next;
			delete m_firstRasterizerState;
			m_firstRasterizerState = i;
		}

		while (m_firstVertexFormat)
		{
			auto i = m_firstVertexFormat->m_next;
			delete m_firstVertexFormat;
			m_firstVertexFormat = i;
		}

		for (uint i = 0; i < VertexFormat::MaxStreams; ++i)
			m_currentVertexBuffers[i].buffer = nullptr;

		m_currentIndexBuffer = nullptr;

		for (uint i = 0; i < Texture::MaxUnits; ++i)
		{
			m_currentTextures[i].texture = nullptr;
			m_currentTextures[i].buffer = nullptr;
		}

		while (m_firstSampler)
		{
			auto i = m_firstSampler->m_next;
			delete m_firstSampler;
			m_firstSampler = i;
		}
	}
	//---------------------------------------------------------------------------//
	bool Graphics::OnEvent(int _id, void* _arg)
	{
		switch (_id)
		{
		case FrameEvent::Begin:
		{
			GL_DEBUG();

			// temp
			SetDepthStencilState(nullptr);
			SetBlendState(nullptr);
			SetRasterizerState(nullptr);

			ClearFrameBuffers((int)FrameBuffer::Type::All, 0, 1, { 0.75f, 0.65f , 0.65f, 1 });
		} break;

		case FrameEvent::End:

			glfwSwapBuffers(gWindow->_GetHandle());

			break;

		default:
			break;
		}

		return false;
	}
	//---------------------------------------------------------------------------//
	DepthStencilStateID Graphics::CreateDepthStencilState(const DepthStencilState::Desc& _desc)
	{
		DepthStencilStateID _last = m_firstDepthStencilState;
		for (auto i = m_firstDepthStencilState; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new DepthStencilState(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetDepthStencilState(DepthStencilStateID _state, uint _stencilRef)
	{
		GL_DEBUG();

		if (!_state)
			_state = m_firstDepthStencilState; // default;

		if (m_currentDepthStencilState != _state || (_state->m_desc.stencilTest && m_currentStencilRef != _stencilRef))
		{
			m_currentDepthStencilState = _state;
			m_currentStencilRef = _stencilRef;
			_state->_Bind(_stencilRef);
		}
	}
	//---------------------------------------------------------------------------//
	BlendStateID Graphics::CreateBlendState(const BlendState::Desc& _desc)
	{
		BlendStateID _last = m_firstBlendState;
		for (auto i = m_firstBlendState; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new BlendState(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetBlendState(BlendStateID _state)
	{
		GL_DEBUG();

		if (!_state)
			_state = m_firstBlendState; // default;

		if (m_currentBlendState != _state)
		{
			m_currentBlendState = _state;
			_state->_Bind();
		}
	}
	//---------------------------------------------------------------------------//
	RasterizerStateID Graphics::CreateRasterizerState(const RasterizerState::Desc& _desc)
	{
		RasterizerStateID _last = m_firstRasterizerState;
		for (auto i = m_firstRasterizerState; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new RasterizerState(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetRasterizerState(RasterizerStateID _state)
	{
		GL_DEBUG();

		if (!_state)
			_state = m_firstRasterizerState; // default;

		if (m_currentRasterizerState != _state)
		{
			m_currentRasterizerState = _state;
			_state->_Bind();
		}
	}
	//---------------------------------------------------------------------------//
	PolygonModeID Graphics::CreatePolygonMode(const PolygonMode::Desc& _desc)
	{
		PolygonModeID _last = m_firstPolygonMode;
		for (auto i = m_firstPolygonMode; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new PolygonMode(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetPolygonMode(PolygonModeID _mode)
	{
		GL_DEBUG();

		if (!_mode)
			_mode = m_firstPolygonMode; // default;

		if (m_currentPolygonMode != _mode)
		{
			m_currentPolygonMode = _mode;
			_mode->_Bind();
		}
	}
	//---------------------------------------------------------------------------//
	VertexFormatID Graphics::CreateVertexFormat(const VertexFormat::Desc& _desc)
	{
		VertexFormatID _last = m_firstVertexFormat;
		for (auto i = m_firstVertexFormat; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new VertexFormat(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetVertexFormat(VertexFormatID _format)
	{
		GL_DEBUG();

		if (!_format)
			_format = m_firstVertexFormat; // default;

		if (m_currentVertexFormat != _format)
		{
			m_currentVertexFormat = _format;
			_format->_Bind(m_currentVertexAttribMask);
		}
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetVertexBuffer(uint _stream, Buffer* _buffer, uint _baseElement)
	{
		GL_DEBUG();

		ASSERT(_stream < VertexFormat::MaxStreams);

		VertexBufferSlot& _slot = m_currentVertexBuffers[_stream];

		if (_slot.buffer != _buffer || _slot.baseElement != _baseElement)
		{
			_slot.buffer = _buffer;
			_slot.baseElement = _baseElement;
			_slot.changed = true;
		}
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetIndexBuffer(Buffer* _buffer, IndexFormat _format)
	{
		GL_DEBUG();

		m_currentIndexFormat = _format;

		if (m_currentIndexBuffer != _buffer)
		{
			m_currentIndexBuffer = _buffer;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer ? _buffer->m_handle : 0);
		}
	}
	//---------------------------------------------------------------------------//
	SamplerID Graphics::CreateSampler(const Sampler::Desc& _desc)
	{
		SamplerID _last = m_firstSampler;
		for (auto i = m_firstSampler; i != nullptr; i = i->m_next)
		{
			if (i->m_desc == _desc)
				return i;

			_last = i;
		}

		return (_last->m_next = new Sampler(_desc));
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetSampler(uint _slot, SamplerID _sampler)
	{
		if (!_sampler)
			_sampler = m_firstSampler;

		if (m_currentSamplers[_slot] != _sampler)
		{
			m_currentSamplers[_slot] = _sampler;
			glBindSampler(_slot, _sampler->m_handle);
		}
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetTexture(uint _slot, Texture* _texture)
	{
		if (m_currentTextures[_slot].texture != _texture)
		{
			m_currentTextures[_slot].buffer = nullptr;
			m_currentTextures[_slot].texture = _texture;
			Texture::_Bind(_slot, _texture);
		}
	}
	//---------------------------------------------------------------------------//
	void Graphics::SetTextureBuffer(uint _slot, TextureBuffer* _buffer)
	{
		if (m_currentTextures[_slot].buffer != _buffer)
		{
			m_currentTextures[_slot].texture = nullptr;
			m_currentTextures[_slot].buffer = _buffer;
			TextureBuffer::_Bind(_slot, _buffer);
		}
	}
	//---------------------------------------------------------------------------//
	void Graphics::ClearFrameBuffers(uint _buffers, uint _start, uint _num, const vec4& _color, float _depth, int _stencil)
	{
		//TODO: ClearColorBuffers(start, num, color) + 	ClrearDepthStencilBuffers(flags, depth, stencil)
		GL_DEBUG();

		// disable blendig ?

		int _viewport[4], _scissor[4];
		glGetIntegerv(GL_VIEWPORT, _viewport);
		glGetIntegerv(GL_SCISSOR_BOX, _scissor);

		int _scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
		glEnable(GL_SCISSOR_TEST);
		glScissor(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

		if (_buffers & (int)FrameBuffer::Type::Color)
		{
			int _colorMask[4];
			glGetIntegerv(GL_COLOR_WRITEMASK, _colorMask);
			glColorMask(1, 1, 1, 1);
			for (uint i = _start; _num; ++i, --_num)
				glClearBufferfv(GL_COLOR, i, &_color.x);
			glColorMask(_colorMask[0], _colorMask[1], _colorMask[2], _colorMask[3]);
		}

		if (_buffers & (int)FrameBuffer::Type::Depth)
		{
			glDepthMask(1);
			glClearBufferfv(GL_DEPTH, 0, &_depth);
			glDepthMask(m_currentDepthStencilState->m_desc.depthWrite);
		}

		if (_buffers & (int)FrameBuffer::Type::Stencil)
		{
			glStencilMask(0xff);
			glClearBufferiv(GL_STENCIL, 0, &_stencil);
			glStencilMask(m_currentDepthStencilState->m_desc.stencilMask);
		}

		glScissor(_scissor[0], _scissor[1], _scissor[2], _scissor[3]);

		if (!_scissorEnabled)
			glDisable(GL_SCISSOR_TEST);
	}
	//---------------------------------------------------------------------------//
	void Graphics::_BeforeDraw(void)
	{
		GL_DEBUG();

		// vertex buffers

		for (uint i = 0; i < m_currentVertexFormat->m_desc.numStreams; ++i)
		{
			const auto& _fmt = m_currentVertexFormat->m_desc.streams[i];
			auto& _slot = m_currentVertexBuffers[_fmt.index];

			if (_slot.buffer)
			{
				_slot.buffer->UploadStream();

				if (_slot.changed)
				{
					glBindVertexBuffer(_fmt.index, _slot.buffer->m_handle, _fmt.stride * _slot.baseElement, _fmt.stride);

					_slot.changed = false;
				}
			}
		}

		// index buffer

		if (m_currentIndexBuffer)
			m_currentIndexBuffer->UploadStream();

		// texture buffers

		for (uint i = 0; i < Texture::MaxUnits; ++i)
		{
			auto& _slot = m_currentTextures[i];
			if (_slot.buffer)
				_slot.buffer->UploadStream();
		}
	}
	//---------------------------------------------------------------------------//
}
