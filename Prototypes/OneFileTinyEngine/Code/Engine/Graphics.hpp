#pragma once

#include "System.hpp"
#include "Resource.hpp"
#include "Math.hpp"

namespace Tiny
{
	//!
	enum class CompareFunc
	{
		Never = 0x0200, // GL_NEVER,
		Less = 0x0201, // GL_LESS,
		LessEqual = 0x0203, // GL_LEQUAL,
		Equal = 0x0202, // GL_EQUAL,
		NotEqual = 0x0205, // GL_NOTEQUAL,
		GreaterEqual = 0x0206, // GL_GEQUAL,
		Greater = 0x0204, // GL_GREATER,
		Always = 0x0207, // GL_ALWAYS,
	};

	//!
	enum class StencilOp
	{
		Keep = 0x1E00, // GL_KEEP
		Zero = 0x0000, // GL_ZERO
		Replace = 0x1E01, // GL_REPLACE
		IncrWrap = 0x8507, // GL_INCR_WRAP
		DecrWrap = 0x8508, // GL_DECR_WRAP
		Incr = 0x1E02, // GL_INCR
		Decr = 0x1E03, // GL_DECR
		Invert = 0x150A, //	GL_INVERT
	};

	//!
	/*enum class PolygonFace
	{
		Front = 0x0404, // GL_FRONT,
		Back = 0x0405, // GL_BACK,
		Both = 0x0408, // GL_FRONT_AND_BACK,
	};*/

	//----------------------------------------------------------------------------//
	// DepthStencilState
	//----------------------------------------------------------------------------//

	//!
	typedef class DepthStencilState* DepthStencilStateID;

	//!
	class DepthStencilState : public NonCopyable
	{
	public:

		//!
		struct Desc
		{
			//!
			bool depthTest = true;
			//!
			bool depthWrite = true;
			//!
			CompareFunc depthFunc = CompareFunc::LessEqual;

			//!
			bool stencilTest = false;
			//!
			uint8 stencilMask = 0xff; // rw
			//!
			CompareFunc stencilfunc = CompareFunc::Always;
			//!
			StencilOp stencilFail = StencilOp::Keep;
			//!
			StencilOp depthFail = StencilOp::Keep;
			//!
			StencilOp depthPass = StencilOp::Keep;

			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		DepthStencilState(const Desc& _desc) : m_desc(_desc) { }
		//!
		void _Bind(uint _ref);

		//!
		Desc m_desc;
		//!
		DepthStencilState* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// BlendState
	//----------------------------------------------------------------------------//

	//!
	typedef class BlendState* BlendStateID;

	//!
	class BlendState : public NonCopyable
	{
	public:

		//!
		enum class Factor
		{
			One = 0x0000, //GL_ZERO,
			Zero = 0x0001, //GL_ONE,
			SrcColor = 0x0300, //GL_SRC_COLOR,
			OneMinusSrcColor = 0x0301, //GL_ONE_MINUS_SRC_COLOR,
			DstColor = 0x0306, //GL_DST_COLOR,
			OneMinusDstColor = 0x0307, //GL_ONE_MINUS_DST_COLOR,
			SrcAlpha = 0x0302, //GL_SRC_ALPHA,
			OneMinusSrcAlpha = 0x0303, //GL_ONE_MINUS_SRC_ALPHA,
			DstAlpha = 0x0304, //GL_DST_ALPHA,
			OneMinusDstAlpha = 0x0305, //GL_ONE_MINUS_DST_ALPHA,
			//SrcAlphaSaturate = 0x0308, //GL_SRC_ALPHA_SATURATE,
		};

		//!
		enum class Mode
		{
			Add = 0x8006, // GL_FUNC_ADD,
			Subtract = 0x800A, // GL_FUNC_SUBTRACT,
			ReverseSubtract = 0x800B, // GL_FUNC_REVERSE_SUBTRACT,
			Min = 0x8007, // GL_MIN,
			Max = 0x8008, // GL_MAX,
		};

		//!
		struct Desc
		{
			bool enabled = false;
			bool write = true;
			Factor src = Factor::One;
			Factor dst = Factor::Zero;
			Mode mode = Mode::Add;

			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		BlendState(const Desc& _desc) : m_desc(_desc) { }
		//!
		void _Bind(void);

		//!
		Desc m_desc;
		//!
		BlendState* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// RasterizerState
	//----------------------------------------------------------------------------//

	//!
	typedef class RasterizerState* RasterizerStateID;

	//!
	class RasterizerState : public NonCopyable
	{
	public:

		struct Desc
		{
			/*
			  rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_FRONT;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = true;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	rasterizerState.ForcedSampleCount = 0;
			*/


			// depthBias
			// depthClip



			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		RasterizerState(const Desc& _desc) : m_desc(_desc) { }
		//!
		void _Bind(void);

		//!
		Desc m_desc;
		//!
		RasterizerState* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// PolygonMode
	//----------------------------------------------------------------------------//

	//!
	typedef class PolygonMode* PolygonModeID;

	//!
	class PolygonMode : public NonCopyable
	{
	public:

		//!
		enum class Fill
		{
			Wire,
			Solid,
		};

		//!
		enum class Face
		{
			None,
			Back,
			Front,
			Both,
		};

		//!
		struct Desc
		{
			//!
			Fill fill = Fill::Solid;
			//!
			Face cull = Face::None;
			//!
			bool fronCCW = true;

			//!
			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		PolygonMode(const Desc& _desc) : m_desc(_desc) { }
		//!
		void _Bind(void);

		//!
		Desc m_desc;
		//!
		PolygonMode* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// IndexFormat 
	//----------------------------------------------------------------------------//

	//!
	enum class IndexFormat
	{
		UShort,
		UInt,
	};

	//----------------------------------------------------------------------------//
	// VertexFormat 
	//----------------------------------------------------------------------------//

	//!
	typedef class VertexFormat* VertexFormatID;

	//!
	class VertexFormat : public NonCopyable
	{
	public:

		//!
		static const uint MaxAttribs = 16;
		//!
		static const uint MaxStreams = 16;

		enum class DataType
		{
			Unknown = 0,
			Byte = 0x1400, // GL_BYTE
			UByte = 0x1401, // GL_UNSIGNED_BYTE
			Short = 0x1402, // GL_SHORT
			UShort = 0x1403, // GL_UNSIGNED_SHORT
			Int = 0x1404, // GL_INT,
			UInt = 0x1405, // GL_UNSIGNED_INT
			Half = 0x140B, // GL_HALF_FLOAT
			Float = 0x1406,	// GL_FLOAT
		};

		//!
		struct Attrib
		{
			uint index = 0;
			DataType type = DataType::Unknown;
			uint components = 0; // 1-4
			bool normalized = false;
			uint offset = 0;
			uint stream = 0; // buffer slot
		};

		//!
		struct Stream
		{
			uint index = 0;
			uint stride = 0;
			uint divisor = 0; // 0 - per vertex, 1 - per instance
		};

		//!
		struct Desc
		{
			//!
			uint numAttribs = 0;
			//!
			uint numStreams = 0;
			//!
			Attrib attribs[MaxAttribs];
			//!
			Stream streams[MaxStreams];

			//!
			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		VertexFormat(const Desc& _desc);
		//!
		void _Bind(uint& _attribsMask);

		//!
		Desc m_desc;
		//!
		uint m_attribMask = 0;
		//!
		VertexFormat* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// Buffer
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class Buffer> BufferRef;

	//!
	class Buffer : public Object
	{
	public:
		//!
		RTTI(Buffer);

		//!
		enum class Usage
		{
			Static = 0x88E4,
			Dynamic = 0x88E8,
		};

		//!
		enum class LockMode
		{
			ReadOnly = 0x0001, // GL_MAP_READ_BIT,
			WriteDiscard = 0x0002 | 0x0004, // GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
			WriteNoOverwrite = 0x0002, // GL_MAP_WRITE_BIT,
			WriteDiscardUnsynchronized = WriteDiscard | 0x0020, // GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT,
		};

		//!
		Buffer(Usage _usage = Usage::Static);
		//!
		~Buffer(void);
		//!
		bool IsStatic(void) { return m_usage == Usage::Static; }

		//!
		uint Size(void) { return m_size; }
		//!
		void Realloc(uint _newSize, const void* _data = nullptr, bool _discard = false);
		//!
		uint8* Map(LockMode _mode, uint _offset = 0, int _size = -1);
		//!
		void Unmap(void);
		//!
		void Write(const void* _src, uint _offset, uint _size);
		//!
		void Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size);

		// [STREAM]
		// it's additional mechanism for streaming draw. Do not use for important data.

		//!
		uint8* GetStreamData(void) { return m_streamData.data(); }
		//!
		uint GetStreamSize(void) { return (uint)m_streamData.size(); }
		//!
		void ClearStream(bool _invalidate = true);
		//!
		void InvalidateStream(void);
		//!
		uint8* ReallocStreamData(uint _size);
		//!
		uint8* AppendStreamData(uint _size);
		//!
		void AppendStreamData(const void* _data, uint _size);
		//! discard buffer and initialize him by stream data. Called automatically by Graphics
		void UploadStream(void);

		//!
		template<class T> T* Elements(void) { return reinterpret_cast<T*>(m_streamData.data()); }
		//!
		template <class T> uint ElementsCount(void) { return m_streamData.size() / sizeof(T); }
		//!
		template <class T> T* ReallocElements(uint _size) { return reinterpret_cast<T*>(ReallocStreamData(_size * sizeof(T))); }
		//!
		template <class T> T* AppendElements(uint _count) { return reinterpret_cast<T*>(AppendStreamData(_count * sizeof(T))); }
		//!
		template <class T> uint AppendElements(const T* _data, uint _count)
		{
			uint _index = ElementsCount<T>();
			AppendStreamData(_data, _count * sizeof(T));
			return _index;
		}
		//!
		template <class T> uint AppendElements(const T* _data, uint _count, uint _base)
		{
			uint _index = ElementsCount<T>();
			T* _dst = AppendElements<T>(_count);
			for (uint i = 0; i < _count; ++i)
				_dst[i] = _data[i] + _base;
			return _index;
		}
		//!
		template <class T> uint AppendElement(const T& _e) { return AppendElements<T>(&_e, 1); }

	protected:
		friend class Graphics;

		//!
		Usage m_usage;
		//!
		uint m_size = 0;
		//!
		uint m_handle = 0;
		//!
		bool m_uploaded = true;
		//!
		std::vector<uint8> m_streamData;
	};

	//----------------------------------------------------------------------------//
	// PixelFormat
	//----------------------------------------------------------------------------//

	//!
	class PixelFormat
	{
	public:

		//!
		enum ID
		{
			Unknown = 0,
			RGB8,
			RGBA8,
			RGB10A2,
			R32F,
			RG32F,
			RGB32F,
			RGBA32F,
			RGBA16F,
			RG11B10F,
			D24S8,
			DXT1,
			DXT5,
		};

		//!
		struct Desc
		{
			const char* name;
			uint bpp;
			uint iformat;
			uint format;
			uint type;
			bool compressed;
		};

		//!
		static const Desc& GetDesc(PixelFormat::ID _format) { return s_desc[_format]; }
		//!
		static uint BitsPerPixel(PixelFormat::ID _format) { return s_desc[_format].bpp; }
		//!
		static bool IsCompressed(PixelFormat::ID _format) { return s_desc[_format].compressed; }

	protected:

		//!
		static const Desc s_desc[];
	};

	//----------------------------------------------------------------------------//
	// TextureBuffer
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class TextureBuffer> TextureBufferRef;

	//!
	class TextureBuffer : public Buffer
	{
	public:
		//!
		RTTI(TextureBuffer);

		//!
		TextureBuffer(PixelFormat::ID _format = PixelFormat::RGBA32F);
		//!
		~TextureBuffer(void);

	protected:
		friend class Graphics;
		//!
		static void _Bind(uint _slot, TextureBuffer* _buffer);

		//!
		PixelFormat::ID m_format;
		//!
		uint m_texture = 0;
	};

	//----------------------------------------------------------------------------//
	// Texture
	//----------------------------------------------------------------------------//

	//!
	typedef Reference<class Texture> TextureRef;

	//!
	class Texture : public Resource
	{
	public:
		//!
		RTTI(Texture);

		//!
		enum class Type
		{
			Quad, // 2D
			Volume, // 3D
			Array,
			Cube,
		};

		//!
		enum class Wrap
		{
			Repeat = 0x2901, // GL_REPEAT
			Clamp = 0x812F, // GL_CLAMP_TO_EDGE
		};

		//!
		enum class Filter
		{
			Nearest,
			Linear,
			Trilinear,
		};

		//!
		Texture(void);
		//!
		~Texture(void);

		//!
		Type GetType() { return m_type; }
		//!
		PixelFormat::ID Format(void) { return m_format; }
		//!
		uint Width(void) { return m_width; }
		//!
		uint Height(void) { return m_height; }
		//!
		uint Depth(void) { return m_depth; }
		//!
		const ivec2& InvSize(void) { return m_invSize; }
		//!
		uint _Handle(void) { return m_handle; }

		//!
		void Init(Type _type = Type::Quad, PixelFormat::ID _format = PixelFormat::DXT5, int _lods = -1);
		//!
		void Realloc(uint _width, uint _height, uint _depth = 0);
		//!
		void Write(PixelFormat::ID _format, const void* _data) { Write(_format, 0, 0, 0, m_width, m_height, m_depth, _data); }
		//!
		void Write(PixelFormat::ID _format, uint _x, uint _y, uint _z, uint _width, uint _height, uint _depth, const void* _data);
		//!
		void GenerateMipmap(void);

		//!
		static const uint MaxUnits = 15;
		//!
		static const uint MinDepth[];
		//!
		static const uint MaxDepth[];

	protected:
		friend class Graphics;

		//!
		void _CreateHandle(void);
		//!
		void _DeleteHandle(void);
		//!
		void _ResetParams(void);
		//!
		static void _Bind(uint _slot, Texture* _texture);

		//!
		Type m_type = Type::Quad;
		//!
		PixelFormat::ID m_format = PixelFormat::RGBA8;
		//!
		bool m_initialized = false;
		//!
		int m_desiredLods = -1;
		//!
		uint m_lods = 0;
		//!
		uint m_width = 0;
		//!
		uint m_height = 0;
		//!
		uint m_depth = 0;
		//!
		uint m_handle = 0;
		//!
		vec2 m_invSize = { 0, 0 };
	};

	//----------------------------------------------------------------------------//
	// Sampler
	//----------------------------------------------------------------------------//

	//!
	typedef class Sampler* SamplerID;

	//!
	class Sampler : public NonCopyable
	{
	public:

		//!
		struct Desc
		{
			//!
			Texture::Wrap wrap = Texture::Wrap::Repeat;
			//!
			Texture::Filter filter = Texture::Filter::Nearest;
			//!
			uint anisotropy = 0;
			//!
			CompareFunc depthFunc = CompareFunc::Never;

			bool operator == (const Desc& _rhs) { return memcmp(this, &_rhs, sizeof(Desc)) == 0; }
		};

	protected:
		friend class Graphics;

		//!
		Sampler(const Desc& _desc);
		//!
		~Sampler(void);
		//!
		void _Bind(void);

		//!
		Desc m_desc;
		//!
		uint m_handle = 0;
		//!
		Sampler* m_next = nullptr;
	};

	//----------------------------------------------------------------------------//
	// FrameBuffer
	//----------------------------------------------------------------------------//

	//!
	class FrameBuffer : public Object
	{
	public:
		//!
		RTTI(FrameBuffer);

		//!
		static const uint MaxRenderTargets = 4;

		//!
		enum class Type
		{
			Color = 0x00004000, // GL_COLOR_BUFFER_BIT,
			Depth = 0x00000100, // GL_DEPTH_BUFFER_BIT,
			Stencil = 0x00000400, // GL_STENCIL_BUFFER_BIT,
			DepthStencil = Depth | Stencil,
			All = Color | DepthStencil,
		};

		FrameBuffer(void);
		~FrameBuffer(void);

		void Init(PixelFormat::ID _format, uint _samples = 1);
		void Realloc(uint _width, uint _height);
		void CopyToTexture(Texture* _texture, uint _z = 0);


	protected:



	};



	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	class Shader
	{
	public:

	protected:
	};

	//----------------------------------------------------------------------------//
	// Graphics
	//----------------------------------------------------------------------------//

	//!
#	define gGraphics Tiny::Graphics::Get()

	//!
	class Graphics : public System, public Singleton<Graphics>
	{
	public:
		//!
		RTTI(Graphics);

		//!
		Graphics(void);

		//!
		bool Startup(void) override;
		//!
		void Shutdown(void) override;
		//!
		bool OnEvent(int _id, void* _arg = nullptr) override;


		//!
		DepthStencilStateID CreateDepthStencilState(const DepthStencilState::Desc& _desc);
		//!
		void SetDepthStencilState(DepthStencilStateID _state, uint _stencilRef = 0);

		//!
		BlendStateID CreateBlendState(const BlendState::Desc& _desc);
		//!
		void SetBlendState(BlendStateID _state);

		//!
		RasterizerStateID CreateRasterizerState(const RasterizerState::Desc& _desc);
		//!
		void SetRasterizerState(RasterizerStateID _state);

		//!
		PolygonModeID CreatePolygonMode(const PolygonMode::Desc& _desc);
		//!
		void SetPolygonMode(PolygonModeID _mode);

		//!
		VertexFormatID CreateVertexFormat(const VertexFormat::Desc& _desc);
		//!
		void SetVertexFormat(VertexFormatID _format);

		//!
		void SetVertexBuffer(uint _stream, Buffer* _buffer, uint _baseElement = 0);
		//!
		void SetIndexBuffer(Buffer* _buffer, IndexFormat _format = IndexFormat::UShort);

		//!
		SamplerID CreateSampler(const Sampler::Desc& _desc);
		//!
		void SetSampler(uint _slot, SamplerID _sampler);

		//!
		void SetTexture(uint _slot, Texture* _texture);
		//!
		void SetTextureBuffer(uint _slot, TextureBuffer* _buffer);

		//!
		void ClearFrameBuffers(uint _buffers, uint _start = 0, uint _num = 1, const vec4& _color = { 0, 0, 0, 1 }, float _depth = 1.f, int _stencil = 0xff);

	protected:

		void _BeforeDraw(void);

		DepthStencilStateID m_firstDepthStencilState = nullptr;
		DepthStencilStateID m_currentDepthStencilState = nullptr;
		uint m_currentStencilRef = 0;

		BlendStateID m_firstBlendState = nullptr;
		BlendStateID m_currentBlendState = nullptr;

		RasterizerStateID m_firstRasterizerState = nullptr;
		RasterizerStateID m_currentRasterizerState = nullptr;

		PolygonModeID m_firstPolygonMode = nullptr;
		PolygonModeID m_currentPolygonMode = nullptr;

		VertexFormatID m_firstVertexFormat = nullptr;
		VertexFormatID m_currentVertexFormat = nullptr;
		uint m_currentVertexAttribMask = 0;


		struct VertexBufferSlot
		{
			BufferRef buffer;
			uint baseElement = 0;
			bool changed = false;
		};

		VertexBufferSlot m_currentVertexBuffers[VertexFormat::MaxStreams];

		BufferRef m_currentIndexBuffer;
		IndexFormat m_currentIndexFormat = IndexFormat::UShort;

		SamplerID m_firstSampler = nullptr;
		SamplerID m_currentSamplers[Texture::MaxUnits];

		struct TextureSlot
		{
			TextureRef texture;
			TextureBufferRef buffer;
		};

		TextureSlot m_currentTextures[Texture::MaxUnits];
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
