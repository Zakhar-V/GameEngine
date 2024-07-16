#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Resource.hpp"
#include "Math.hpp"

#include <Windows.h>
#include <gl/gl.h>
#include "GL/glext.h"

//---------------------------------------------------------------------------//
// Vertex 
//---------------------------------------------------------------------------//

//! 44 bytes
struct Vertex
{
	Vector3 pos; // 12
	Vector3 normal; // 12
	Color color; // 4
	Vector2 texcoord; // 8

	Vector4ub index; // 4
	Vector4ub weight;	// 4
};

//---------------------------------------------------------------------------//
// Buffer 
//---------------------------------------------------------------------------//

class Buffer : public Object
{
public:
	RTTI(Buffer);

	//!
	enum class Type
	{
		Vertex = sizeof(Vertex),
		Index = sizeof(uint),
	};

	//!
	Buffer(void) = default;
	//!
	~Buffer(void) = default;

	//!
	Type GetType(void) { return m_type; }
	//!
	uint BitsPerElement(void) { return (uint)m_type; }

	//!
	void Resize(Type _type, uint _size);
	//!
	uint Size(void) { return m_size; }

	//!
	uint8* Data(void) { return m_data.data(); }
	//!
	Vertex* VertexData(void);
	//!
	uint* IndexData(void);

protected:

	//!
	Type m_type = Type::Vertex;
	//!
	uint m_size = 0;
	//!
	std::vector<uint8> m_data;
};

//---------------------------------------------------------------------------//
// PixelFormat 
//---------------------------------------------------------------------------//

//!
struct Pixel
{
	//!
	enum class Format
	{
		Unknown,
		RGB8,
		RGBA8,
		//DXT1,
		//DXT5,
		_Count,
	};

	//!
	struct Desc
	{
		char name[16];
		uint glFormat;
		uint glType;
		uint glInternalFormat;
		uint bitsPerPixel;
		bool compressed;
	};

	//!
	static const Desc formats[(int)Format::_Count];
};

//---------------------------------------------------------------------------//
// Image 
//---------------------------------------------------------------------------//

//!
class Image : public Resource
{
public:
	RTTI(Image);

	//!
	Image(void) = default;
	//!
	~Image(void);

	//!
	bool Load(File* _src) override;

	//!
	Pixel::Format Format(void) { return m_format; }
	//!
	uint Width(void) { return m_width; }
	//!
	uint Height(void) { return m_height; }
	//!
	uint BitsPerPixel(void) { return Pixel::formats[(int)m_format].bitsPerPixel; }
	//!
	uint8* Data(void) { return m_data; }

	//!
	void Clear(void);

protected:

	//!
	Pixel::Format m_format = Pixel::Format::Unknown;
	//!
	uint m_width = 0;
	//!
	uint m_height = 0;
	//!
	uint8* m_data = nullptr;
};

//---------------------------------------------------------------------------//
// Texture 
//---------------------------------------------------------------------------//

class Texture : public Resource
{
public:
	RTTI(Texture);

	//!
	bool Load(File* _src) override;

	//!
	enum Type
	{
		//!
		Type_2D = GL_TEXTURE_2D,
	};

	//!
	Texture(void) = default;
	//!
	~Texture(void);

	//!
	bool CreateFromImage(Image* _img);

	//!
	uint Handle(void) { return m_handle; }

protected:

	//!
	uint m_handle = 0;
};

//---------------------------------------------------------------------------//
// PrimitiveType 
//---------------------------------------------------------------------------//

enum class PrimitiveType
{
	Point = GL_POINTS,
	Line = GL_LINES,
	Triangle = GL_TRIANGLES,
};

//---------------------------------------------------------------------------//
// Graphics 
//---------------------------------------------------------------------------//

//!
#define gGraphics Graphics::Get()

//!
class Graphics : public System, public Singleton<Graphics>
{
public:
	RTTI(Graphics);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;

	//!
	void SetViewport(const IntRect& _rect);
	//!
	void SetCameraMatrix(const Matrix44& _m);
	//!
	void SetModelMatrix(const Matrix44& _m);

	//!
	void SetTexture(Texture* _tex);

	//!
	void SetVertexBuffer(Buffer* _buffer);
	//!
	void SetIndexBuffer(Buffer* _buffer);

	//!
	void Draw(PrimitiveType _type, uint _start, uint _count);
	//!
	void DrawIndexed(PrimitiveType _type, uint _start, uint _count);

protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	//!
	SharedPtr<Texture> m_texture;
	//!
	SharedPtr<Buffer> m_vertexBuffer;
	//!
	SharedPtr<Buffer> m_indexBuffer;
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//
