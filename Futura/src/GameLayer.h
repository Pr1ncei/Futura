/**
 *  @file GameLayer.h
 *
 *  @brief Declares the first refactored gameplay/render layer for Futura.
 *
 *  GameLayer is the Phase 2 vertical slice that proves the application,
 *  window, graphics wrappers, and renderer facade can draw through the new
 *  engine-style architecture.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 01, 2026
 *      Last Modified on:    July 01, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_FPSCameraController.h"
#include "FuturaLibrary/core/c_Layer.h"
#include "FuturaLibrary/events/e_KeyEvent.h"
#include "FuturaLibrary/renderer/r_DebugOverlay.h"
#include "FuturaLibrary/renderer/r_DebugRenderer.h"
#include "FuturaLibrary/renderer/r_Material.h"

#include "SceneWorld.h"

class GameLayer : public FuturaLibrary::Layer
{
public:
	GameLayer();
	~GameLayer() override = default;

	void OnAttach() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnEvent(FuturaLibrary::Event& event) override;

private:
	bool OnKeyPressed(FuturaLibrary::KeyPressedEvent& event);

	FuturaLibrary::Ref<FuturaLibrary::Material> m_DefaultMaterial;
	SceneWorld m_SceneWorld;
	FuturaLibrary::FPSCameraController m_CameraController;
	FuturaLibrary::DebugOverlayState m_DebugOverlayState;
	FuturaLibrary::DebugOverlayFrameData m_DebugOverlayFrameData;
	float m_LastFrameTime = 0.0f;
	float m_FPSUpdateTimer = 0.0f;
	uint32_t m_FrameCounter = 0;
};
