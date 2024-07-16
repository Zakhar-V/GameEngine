#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Scene.hpp"
#include "Model.hpp"

//---------------------------------------------------------------------------//
// Definitions 
//---------------------------------------------------------------------------//

class GraphicsWorld;

//---------------------------------------------------------------------------//
// GraphicsComponent 
//---------------------------------------------------------------------------//

class GraphicsComponent : public Component
{
public:
	RTTI(GraphicsComponent);


protected:
	//!
	void _AddToScene(void) override;
	//!
	void _RemoveFromScene(void) override;

	//!
	virtual void _AddToSceneImpl(void) { }
	//!
	virtual void _RemoveFromSceneImpl(void) { }

	//!
	GraphicsWorld* m_world = nullptr;
};

//---------------------------------------------------------------------------//
// Camera 
//---------------------------------------------------------------------------//

//!
class Camera : public GraphicsComponent
{
public:
	RTTI(Camera);

protected:
	friend class GraphicsWorld;

	//!
	void _AddToSceneImpl(void) override;
	//!
	void _RemoveFromSceneImpl(void) override;

	Camera* m_prevCamera = nullptr;
	Camera* m_nextCamera = nullptr;

	//!
	float m_fov = 60;


	//! 
	bool m_orthographic = false;
};

//---------------------------------------------------------------------------//
// MeshInstance 
//---------------------------------------------------------------------------//

//!
class MeshInstance : public GraphicsComponent
{
public:
	RTTI(MeshInstance);

protected:
	friend class GraphicsWorld;

	//!
	void _AddToSceneImpl(void) override;
	//!
	void _RemoveFromSceneImpl(void) override;

	SharedPtr<Mesh> m_mesh;
	// overrided materials

	MeshInstance* m_prevMesh = nullptr;
	MeshInstance* m_nextMesh = nullptr;
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//

class Sprite : public GraphicsComponent
{
public:
	RTTI(Sprite);

protected:

	SharedPtr<Texture> m_texture;
	Rect m_texcoord;
	Vector2 m_size;
	Vector2 m_pivot;
	Color m_color;
};

//---------------------------------------------------------------------------//
// GraphicsWorld 
//---------------------------------------------------------------------------//
			  
//!
class GraphicsWorld : public SceneSystem
{
public:
	RTTI(GraphicsWorld);


	//!
	void SetMainCamera(Camera* _camera);
	//!
	Camera* GetMainCamera(void) { return m_mainCamera.Get(); }

	//!	
	void _AddCamera(Camera* _camera);
	//!
	void _RemoveCamera(Camera* _camera);

	//!
	void _AddMeshInstance(MeshInstance* _mesh);
	//!
	void _RemoveMeshInstance(MeshInstance* _mesh);

protected:

	WeakRef<Camera> m_mainCamera;

	Camera* m_cameraList = nullptr;
};

//---------------------------------------------------------------------------//
// Renderer 
//---------------------------------------------------------------------------//

//!
#define gRenderer Renderer::Get()

//!
class Renderer : public System, public Singleton<Renderer>
{
public:
	RTTI(Renderer);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;

protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	void _Draw(GraphicsWorld* _world);
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//
