#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Scene.hpp"

//---------------------------------------------------------------------------//
// Level 
//---------------------------------------------------------------------------//

//!
#define gLevel Level::Get()

//!
class Level : public System, public Singleton<Level>
{
public:
	RTTI(Level);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;


	//!	temp
	void SetCurrentScene(Scene* _scene) { m_currentScene = _scene; }
	//!	temp
	Scene* GetCurrentScene(void) { return m_currentScene; }

protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	//!
	//std::vector<SharedPtr<Scene>> m_scenes;
	//!
	//uint m_currentSceneId = 0;
	//!
	SharedPtr<Scene> m_currentScene;
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//

