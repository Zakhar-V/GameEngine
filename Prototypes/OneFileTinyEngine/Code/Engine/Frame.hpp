#pragma once

#include "Common.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// FrameEvent
	//---------------------------------------------------------------------------//

	//!
	struct FrameEvent
	{
		enum ID
		{
			Begin = ConstHash("FrameEvent::Begin"),
			PreUpdate = ConstHash("FrameEvent::PreUpdate"),
			Update = ConstHash("FrameEvent::Update"),
			PostUpdate = ConstHash("FrameEvent::PostUpdate"),
			Draw = ConstHash("FrameEvent::Draw"),
			End = ConstHash("FrameEvent::End"),
		};
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
