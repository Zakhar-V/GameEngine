#pragma once

#include "Engine.hpp"
#include "EngineAPI.hpp"
#include "Internal.hpp"

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////

class Scene;
class Component;

//!


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------//
// Timer
//---------------------------------------------------------------------------//






///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////

/*
TODO:




*/


///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
// MAIN
//---------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////

#include <direct.h>
#include <stdarg.h>
#include <Windows.h>

struct StringUtils
{
	//!
	static std::string Format(const char* _fmt, ...)
	{
		va_list _args;
		va_start(_args, _fmt);
		std::string _str = FormatV(_fmt, _args);
		va_end(_args);
		return _str;
	}
	//!
	static std::string FormatV(const char* _fmt, va_list _args)
	{
		char _buff[4096];
		int _length = _vsnprintf(_buff, sizeof(_buff) - 1, _fmt, _args);
		return std::string(_buff, _length);
	}
};


class A : public Object
{
public:
	RTTI(A);
};

class B : public Object
{
public:
	RTTI(B);
};

class C : public A
{
public:
	RTTI(C);
};


class Application : public System, public Singleton<Application>
{
public:
	RTTI(Application);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override
	{
		switch (_id)
		{
		case FrameEvent::Begin:
		{

			m_rot = Quaternion().FromAxisAngle(Vector3::UnitZ, gTimer->DeltaTime()) * Quaternion().FromAxisAngle(Vector3::UnitY, gTimer->DeltaTime()) * Quaternion().FromAxisAngle(Vector3::UnitX, gTimer->DeltaTime()) * m_rot;

			gGraphics->SetVertexBuffer(m_vtest);
			gGraphics->SetIndexBuffer(m_itest);
			gGraphics->SetTexture(m_textest);


			//glMatrixMode(GL_PROJECTION);
			Matrix44 _proj;
			_proj.CreateOrtho(0, gWindow->GetSize().x, gWindow->GetSize().y, 0, 0, 1);
			_proj.CreateProjection(60 * DEGREES, gWindow->GetSize().x / (float)gWindow->GetSize().y, 0.1f, 1000.f);

			_proj = _proj * Matrix44().CreateTranslation({0, 0, 20});

			gGraphics->SetViewport(IntRect({ 0, 0}, gWindow->GetSize()));
			gGraphics->SetCameraMatrix(_proj);
			gGraphics->SetModelMatrix(Matrix44().CreateRotation(m_rot));

			/*_proj = {
				0, 0, 0, 0,
				0, 0, 0, 1,
				0, 0, -1, 0,
				-1, 0, 0, 1
			};*/


			//_proj.Transpose();
			//glLoadIdentity();
			//glLoadMatrixf(_proj.v);


			//glm::mat4x4 _p = glm::orthoRH_NO(0, 800, 600, 0, -1, 1);
			//glLoadMatrixf((float*)&_p);

			//glOrtho(0, 800, 600, 0, -1, 1);

			//Matrix44 _m;
			//glGetFloatv(GL_PROJECTION_MATRIX, _m.v);

			//_m.Transpose();

			/*LOG("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f",
				_m.v[0], _m.v[1], _m.v[2], _m.v[3],
				_m.v[4], _m.v[5], _m.v[6], _m.v[7],
				_m.v[8], _m.v[9], _m.v[10], _m.v[11],
				_m.v[12], _m.v[13], _m.v[14], _m.v[15]);

			system("pause"); */
			//gGraphics->DrawIndexed(PrimitiveType::Triangle, 0, 6);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_mesh->_Bind();
			m_mesh->_DrawAll();


		} break;
		case FrameEvent::End:
		{

		} break;
		};

		return EventResult::Pass;
	}

protected:
	//!
	bool _Startup(void)	override
	{
		LOG("%s", __FUNCTION__);

		m_vtest = new Buffer;

		m_vtest->Resize(Buffer::Type::Vertex, 4);

		Vertex* v = m_vtest->VertexData();

		v[0].pos = Vector3(0, 0, 0); // lt
		v[1].pos = Vector3(500, 0, 0); // rt
		v[2].pos = Vector3(500, 500, 0); //	rb
		v[3].pos = Vector3(0, 500, 0); // lb

		v[0].color = Color(255, 0, 0, 1);  // lt
		v[1].color = Color(0, 255, 0, 1);  // rt
		v[2].color = Color(0, 0, 255, 1);  // rb
		v[3].color = Color(255, 255, 255, 1); // lb

		v[0].texcoord = Vector2(0, 0);
		v[1].texcoord = Vector2(1, 0);
		v[2].texcoord = Vector2(1, 1);
		v[3].texcoord = Vector2(0, 1);


		m_itest = new Buffer;
		m_itest->Resize(Buffer::Type::Index, 6);

		uint* i = m_itest->IndexData();
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 2;
		i[5] = 3;

		m_textest = gResourceManager->Load<Texture>("test.psd");

		m_mesh = gResourceManager->Load<Mesh>("test.obj");

		return true;
	}
	//!
	void _Shutdown(void) override
	{
		LOG("%s", __FUNCTION__);
	}

	SharedPtr<Buffer> m_vtest, m_itest;
	SharedPtr<Texture> m_textest;


	Quaternion m_rot = Quaternion::Identity;

	SharedPtr<Mesh> m_mesh;

};



int main(void)
{
	_chdir("Data/");

	{
		SharedPtr<Engine> e = new Engine;

		gEngine->AddSystem(new Application);

		PeriodTimer _pt(0.5f);

		if (gEngine->Startup())
		{
			float _fps = 0;
			while (!gWindow->UserRequestsExit())
			{
				gEngine->Frame();

				if (_fps != gTimer->FramesPerSecond())
				{
					_fps = gTimer->FramesPerSecond();
					std::string _s = StringUtils::Format("%.2f", gTimer->FramesPerSecond());
					glfwSetWindowTitle(gWindow->Handle(), _s.c_str());
				}
			
			}
		}

		gEngine->Shutdown();
	}


	return 0;
}
