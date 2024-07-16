#include "Graphics.hpp"
#include "Time.hpp"
#include "Factory.hpp"
#include "Window.hpp"
#include "Internal.hpp"
#include "GL/wglext.h"
#include "OpenGL.inl"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//---------------------------------------------------------------------------//
// Buffer
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
void Buffer::Resize(Type _type, uint _size)
{
	m_type = _type;
	m_size = _size;
	m_data.resize(BitsPerElement() * _size);
}
//---------------------------------------------------------------------------//
Vertex* Buffer::VertexData(void)
{
	ASSERT(m_type == Type::Vertex);
	return reinterpret_cast<Vertex*>(m_data.data());
}
//---------------------------------------------------------------------------//
uint* Buffer::IndexData(void)
{
	ASSERT(m_type == Type::Index);
	return reinterpret_cast<uint*>(m_data.data());
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// PixelFormat 
//---------------------------------------------------------------------------//

const Pixel::Desc Pixel::formats[(int)Pixel::Format::_Count] =
{
	{"Unknown", GL_NONE, GL_NONE, GL_NONE, 0, false},
	{"RGB8", GL_RGB, GL_UNSIGNED_BYTE, GL_RGB8, 24, false},
	{"RGBA8", GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA8,  32, false},
	//{"DXT1", GL_NONE, 4, true},
	//{"DXT5", GL_NONE, 8, true},
};

//---------------------------------------------------------------------------//
// Image 
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
Image::~Image()
{
	Clear();
}
//---------------------------------------------------------------------------//
bool Image::Load(File* _src)
{
	ASSERT(_src != nullptr);

	float _time = gTimer->CurrentTime();

	Clear();

	if (PathUtils::Extension(_src->Name()) == ".json")
	{
		// TODO: load from json
		return false;
	}
	else
	{
		stbi_io_callbacks _cb =
		{
			[](void* _file, char* _dst, int _size) -> int { return reinterpret_cast<File*>(_file)->Read(_dst, _size); },
			[](void* _file, int _pos) { reinterpret_cast<File*>(_file)->Seek(_pos); },
			[](void* _file) -> int { return reinterpret_cast<File*>(_file)->EoF(); }
		};
		int _width, _height, _channels;
		m_data = stbi_load_from_callbacks(&_cb, _src, &_width, &_height, &_channels, 0);

		if (m_data)
		{
			m_format = _channels == 4 ? Pixel::Format::RGBA8 : Pixel::Format::RGB8;
			m_width = _width;
			m_height = _height;


			// flip
			{
				uint _line = _width * _channels;
				uint8* _temp = new uint8[_line];

				for (int u = 0, b = _height - 1; u < b; --b, ++u)
				{
					uint8* _up = m_data + u * _line;
					uint8* _bottom = m_data + b * _line;
					memcpy(_temp, _up, _line);
					memcpy(_up, _bottom, _line);
					memcpy(_bottom, _temp, _line);
				}
				delete[] _temp;
			}

			LOG("Image \'%s\' loaded, %dx%d pixels, %d bpp (%.3f seconds)", _src->Name().c_str(), _width, _height, _channels * 8, (gTimer->CurrentTime() - _time));

			return true;
		}
		else
		{
			LOG("Unable to load image \'%s\' ", _src->Name().c_str());
		}
	}

	return false;
}
//---------------------------------------------------------------------------//
void Image::Clear(void)
{
	if (m_data)
		stbi_image_free(m_data);
	m_format = Pixel::Format::Unknown;
	m_width = 0;
	m_height = 0;
	m_data = nullptr;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Texture
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
bool Texture::Load(File* _src)
{
	ASSERT(_src != nullptr);
	float _time = gTimer->CurrentTime();

	if (PathUtils::Extension(_src->Name()) == ".json")
	{
		// TODO: load from json
		return false;
	}
	else
	{
		SharedPtr<Image> _img = new Image;
		if (_img->Load(_src) && CreateFromImage(_img))
		{
			LOG("Texture \'%s\' loaded (%.3f seconds)", _src->Name().c_str(), gTimer->CurrentTime() - _time);
			return true;
		}

		LOG("ERROR: Unable to create texture from \'%s\'", _src->Name().c_str());
	}

	return false;
}
//---------------------------------------------------------------------------//
Texture::~Texture(void)
{
	// glDeleteTextures
}
//---------------------------------------------------------------------------//
bool Texture::CreateFromImage(Image* _img)
{
	if (_img && _img->Format() != Pixel::Format::Unknown)
	{
		//todo texture slot 5
		if (!m_handle)
			glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // todo
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // todo
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // todo
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // todo

		int _type = Pixel::formats[(int)_img->Format()].glType;
		int _format = Pixel::formats[(int)_img->Format()].glFormat;
		int _iformat = Pixel::formats[(int)_img->Format()].glInternalFormat;
		glTexImage2D(GL_TEXTURE_2D, 0, _iformat, _img->Width(), _img->Height(), 0, _format, _type, _img->Data());
		glGenerateMipmap(GL_TEXTURE_2D);  // 

		return true;
	}
	else
	{
		// if handle ...
	}

	return false;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Graphics
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
EventResult Graphics::OnEvent(int _id, const Variant& _args)
{
	switch (_id)
	{
	case FrameEvent::Begin:
	{
		glClearColor(0.65f, 0.55f, 0.55f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	} break;

	case FrameEvent::End:
	{
		glfwSwapBuffers(gWindow->Handle());

	} break;

	};
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool Graphics::_Startup(void)
{
	LOG("%s", __FUNCTION__);
	LOG("OpenGL: %s on %s by %s", glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));
	
	if (!_LoadOpenGL())
		return false;

	gFactory->Register<Image>();
	gFactory->Register<Texture>();

	glMatrixMode(GL_MODELVIEW);


	return true;
}
//---------------------------------------------------------------------------//
void Graphics::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);
}
//---------------------------------------------------------------------------//
void Graphics::SetViewport(const IntRect& _rect)
{
	glViewport(_rect.left, 0 - _rect.top, _rect.Width(), _rect.Height()); // ??
}
//---------------------------------------------------------------------------//
void Graphics::SetCameraMatrix(const Matrix44& _m)
{
	Matrix44 _tm = _m.Copy().Transpose();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_tm.v);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------//
void Graphics::SetModelMatrix(const Matrix44& _m)
{
	Matrix44 _tm = _m.Copy().Transpose();
	glLoadMatrixf(_tm.v);
}
//---------------------------------------------------------------------------//
void Graphics::SetTexture(Texture* _tex)
{
	if (m_texture != _tex)
	{
		m_texture = _tex;
		if (_tex)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _tex->Handle());
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
}
//---------------------------------------------------------------------------//
void Graphics::SetVertexBuffer(Buffer* _buffer)
{
	m_vertexBuffer = _buffer;
}
//---------------------------------------------------------------------------//
void Graphics::SetIndexBuffer(Buffer* _buffer)
{
	m_indexBuffer = _buffer;
}
//---------------------------------------------------------------------------//
void Graphics::Draw(PrimitiveType _type, uint _start, uint _count)
{
	int _psize = (_type == PrimitiveType::Point ? 1 : (_type == PrimitiveType::Line ? 2 : 3));

	ASSERT(m_vertexBuffer != nullptr);
	ASSERT(_start + _count <= m_vertexBuffer->Size());

	Vertex* _vdata = m_vertexBuffer->VertexData();

	glBegin((int)_type);

	for (uint i = 0; i < _count; ++i)
	{
		Vertex* _v = _vdata + _start + i;

		glNormal3fv(&_v->normal.x);
		glColor4ubv(&_v->color.r);
		glTexCoord2fv(&_v->texcoord.x);
		glVertex3fv(&_v->pos.x);
	}

	glEnd();
}
//---------------------------------------------------------------------------//
void Graphics::DrawIndexed(PrimitiveType _type, uint _start, uint _count)
{
	int _psize = (_type == PrimitiveType::Point ? 1 : (_type == PrimitiveType::Line ? 2 : 3));

	ASSERT(m_vertexBuffer != nullptr);
	ASSERT(m_indexBuffer != nullptr);
	ASSERT(_start + _count <= m_indexBuffer->Size());

	Vertex* _vdata = m_vertexBuffer->VertexData();
	uint* _idata = m_indexBuffer->IndexData();

	glBegin((int)_type);

	for (uint i = 0; i < _count; ++i)
	{
		uint _i = _idata[_start + i];
		ASSERT(_i < m_vertexBuffer->Size());
		Vertex* _v = _vdata + _i;

		glNormal3fv(&_v->normal.x);
		glColor4ubv(&_v->color.r);
		glTexCoord2fv(&_v->texcoord.x);
		glVertex3fv(&_v->pos.x);
	}

	glEnd();
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
