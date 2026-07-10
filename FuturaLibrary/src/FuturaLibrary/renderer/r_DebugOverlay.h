/**
 *  @file r_DebugOverlay.h
 *
 *  @brief Declares the in-game Dear ImGui debug overlay.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/renderer/r_DebugRenderer.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include "FuturaLibrary/resources/r_StaticWorld.h"

#include <functional>

namespace FuturaLibrary
{
	struct DebugOverlayState
	{
		bool ShowStats = false;
		DebugWorldDrawSettings DrawSettings;
	};

	struct DebugOverlayFrameData
	{
		float FPS = 0.0f;
		float FrameTimeMs = 0.0f;
		RenderStats Render;
		DebugDrawStats DebugDraw;
		CollisionQueryStats Collision;
		bool HasPlanes = false;
		uint32_t SelectedSurfaceIndex = 0;
	};

	class FT_API DebugOverlay
	{
	public:
		static void Draw(DebugOverlayState& state, const DebugOverlayFrameData& frameData, const std::function<void()>& selectNextSurface);
	};
}
