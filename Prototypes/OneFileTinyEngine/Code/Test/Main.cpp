#include "TinyEngine.hpp"

using namespace Tiny;

namespace Tiny
{
	class Entity;
	class Component;
	class Scene;


	/*
	������� ����

	Q:���������� ���� (�������, ������ � ��) ��������� �� ������ ������ ��� ��� ������ ����� ����?
	A:

	Q: ����� �������� ���������� ��� ���� �����, ����������� ����������
	A:

	Q: ������� � ��������������� �����
	A:

	Q: ������� ��������� �� ����� ����� � ������ (�������� ��������� ������ � ������)
	A:

	Q: ������������� �������
	A:


	������� 1:
	- ���� �����, ����������� ����������, � ������ ������ �������� ���� �������� �����
	- ����� �������� � ���� ���������� (������� ������ � ��)

	������� 2:

	- ����� �������� ������ ������� ������
	- ���������� ����� �������� ����������� �� ������ ������
	- ��� ��������� �����, �� ���������� ����������� � ��������������� ����������
	

	+�������� ������ ��������� �������, ������ ������������ � ��
	+�������� ������� ������������� ������� (���������������� ���������� � ��)


	*/

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//


	class RenderableComponent : public Component
	{

	};

	class Camera : public RenderableComponent
	{

	};

	class Light : public RenderableComponent
	{

	};

	class MeshInstance : public RenderableComponent
	{

	};

	//! ???
	class MeshAnimator : public Component
	{
		//skeleton
	};

	class Terrain : public RenderableComponent
	{

	};

	class ParticleEmitter : public RenderableComponent
	{

	};

	// 2D

	class RenderableComponent2D : public RenderableComponent
	{

	};

	class Sprite : public RenderableComponent2D
	{

	};



	class RenderableScene : public SceneSubsystem, public Singleton<RenderableScene>
	{

	};

	class Renderer : public SceneSubsystem, public Singleton<Renderer>
	{

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	//!
	class PhysicsComponent : public Component
	{
	public:

	protected:

	};

	//!
	class RigidBody : public PhysicsComponent
	{

	};

	//!
	class PhysicsShape : public PhysicsComponent
	{

	};

	//!
	class PhysicsConstraint : public PhysicsComponent
	{

	};

	//!
	class Physics : public SceneSubsystem, public Singleton<Physics>
	{
	public:

	protected:

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	class GUIComponent
	{

	};

	class GUISystem
	{

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	class ScriptComponent : public Component
	{
	public:

	protected:

	};

	class ScriptSystem : public SceneSubsystem, public Singleton<ScriptSystem>
	{

	};

	class ScriptEngine : public SceneSubsystem, public Singleton<ScriptEngine>
	{

	};

	/*
	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	//!
	class Level : public System, public Singleton<Level>
	{
	public:
		//!
		RTTI(Level);

		//!
		bool Startup(void) override
		{
			return true;
		}
		//!
		void Shutdown(void) override
		{

		}
		//!
		bool OnEvent(int _id, void* _arg = nullptr) override
		{

		}

		//!
		Scene* GetCurrentScene(void) { return m_currentScene; }

	protected:


		SceneRef m_currentScene;
		//std::vector<SceneRef> m_scenes;
	};


	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	class GraphicsWorld : public Component
	{
	public:

	protected:
	};

	class GraphicsWorld2D : public Component
	{
	public:

	protected:
	};


	class PhysicsWorld : public Component
	{
	public:

	protected:
	};

	class PhysicsWorld2D : public Component
	{
	public:

	protected:
	};

	class GuiSystem : public Component
	{
	public:

	protected:
	};

	class SoundWorld : public Component
	{
	public:

	protected:
	}; */


}

class Shader : public Resource
{

	class Definition
	{

	};
	
	class Variant
	{
		//std::vector<definitions>	
	};

	class Param
	{
		//location
		//type
		//value
	};
};


class VideoDriver
{
public:

	struct RasterizerState;
	struct Buffer;
	struct Texture;
	struct Sampler;

	struct Shader;
	struct 
};


int main()
{
	{
		Reference<Engine> _engine = new Engine;

		if (_engine->Startup())
		{
			EntityRef _scene = gScene;
			/*VertexFormat::Desc _vfd;
			_vfd.numAttribs = 3;

			_vfd.attribs[0].index = 0;
			_vfd.attribs[1].index = 1;
			_vfd.attribs[2].index = 7;

			VertexFormatID _vf1 = gGraphics->CreateVertexFormat(_vfd);

			_vfd.attribs[0].index = 0;
			_vfd.attribs[1].index = 1;
			_vfd.attribs[2].index = 2;

			VertexFormatID _vf2 = gGraphics->CreateVertexFormat(_vfd);

			_vfd.attribs[0].index = 9;
			_vfd.attribs[1].index = 10;
			_vfd.attribs[2].index = 11;

			VertexFormatID _vf3 = gGraphics->CreateVertexFormat(_vfd);


			gGraphics->SetVertexFormat(_vf1);
			gGraphics->SetVertexFormat(_vf2);
			gGraphics->SetVertexFormat(_vf3);
			gGraphics->SetVertexFormat(_vf1); */


			TextureBufferRef _tbo = new TextureBuffer;

			gGraphics->SetTextureBuffer(0, _tbo);


			while (!gWindow->ShouldClose())
				gEngine->Frame();

			_engine->Shutdown();
		}
	}

	

	system("pause");
	return 0;
}