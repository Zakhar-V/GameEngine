#include "Renderer.hpp"
#include "Factory.hpp"
#include "Level.hpp"


//---------------------------------------------------------------------------//
// GraphicsComponent
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
void GraphicsComponent::_AddToScene(void)
{
	m_world = m_scene->GetOrAddComponent<GraphicsWorld>();
	_AddToSceneImpl();
}
//---------------------------------------------------------------------------//
void GraphicsComponent::_RemoveFromScene(void)
{
	_RemoveFromSceneImpl();
	m_world = nullptr;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Camera
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
void Camera::_AddToSceneImpl(void)
{
	m_world->_AddCamera(this);
}
//---------------------------------------------------------------------------//
void Camera::_RemoveFromSceneImpl(void)
{
	m_world->_RemoveCamera(this);
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// MeshInstance
//---------------------------------------------------------------------------//
	
//---------------------------------------------------------------------------//
void MeshInstance::_AddToSceneImpl(void)
{
	m_world->_AddMeshInstance(this);
}
//---------------------------------------------------------------------------//
void MeshInstance::_RemoveFromSceneImpl(void)
{
	m_world->_RemoveMeshInstance(this);
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// GraphicsWorld
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
void GraphicsWorld::SetMainCamera(Camera* _camera)
{
	m_mainCamera = _camera;
}
//---------------------------------------------------------------------------//
void GraphicsWorld::_AddCamera(Camera* _camera)
{
	if (_camera && _camera->GetScene() == m_scene)
	{
		List::Link(m_cameraList, _camera, _camera->m_prevCamera);
	}
}
//---------------------------------------------------------------------------//
void GraphicsWorld::_RemoveCamera(Camera* _camera)
{
	if (_camera && _camera->GetScene() == m_scene)
	{
		List::Unlink(m_cameraList, _camera, _camera->m_prevCamera);
	}
}
//---------------------------------------------------------------------------//
void GraphicsWorld::_AddMeshInstance(MeshInstance* _mesh)
{

}
//---------------------------------------------------------------------------//
void GraphicsWorld::_RemoveMeshInstance(MeshInstance* _mesh)
{

}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Renderer
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
EventResult Renderer::OnEvent(int _id, const Variant& _args)
{
	switch (_id)
	{
	case FrameEvent::Begin:
	{
		Scene* _scene = gLevel->GetCurrentScene();
		GraphicsWorld* _world = _scene ? _scene->GetComponent<GraphicsWorld>(): nullptr;
		if(_world)
			_Draw(_world); // _world->Draw();

	} break;
	};

	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool Renderer::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	gFactory->Register<Mesh>();


	gFactory->Register<Camera>();
	gFactory->Register<Sprite>();

	return true;
}
//---------------------------------------------------------------------------//
void Renderer::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);
}
//---------------------------------------------------------------------------//
void Renderer::_Draw(GraphicsWorld* _world)
{

}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
