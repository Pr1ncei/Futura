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

#include "FuturaLibrary/core/c_camera.h"
#include "FuturaLibrary/core/c_Layer.h"
#include "FuturaLibrary/events/e_KeyEvent.h"
#include "FuturaLibrary/events/e_MouseEvent.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/renderer/r_Mesh.h"

class GameLayer : public FuturaLibrary::Layer
{
public:
	GameLayer();
	~GameLayer() override = default;

	void OnAttach() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnEvent(FuturaLibrary::Event& event) override;

private:
	bool OnKeyPressed(FuturaLibrary::KeyPressedEvent& event);
	bool OnKeyReleased(FuturaLibrary::KeyReleasedEvent& event);
	bool OnMouseMoved(FuturaLibrary::MouseMovedEvent& event);
	bool OnMouseScrolled(FuturaLibrary::MouseScrolledEvent& event);

	FuturaLibrary::Ref<FuturaLibrary::Mesh> m_CubeMesh;
	FuturaLibrary::Ref<FuturaLibrary::Material> m_CubeMaterial;
	Camera m_Camera;
	float m_LastFrameTime = 0.0f;
	float m_LastMouseX = 0.0f;
	float m_LastMouseY = 0.0f;
	bool m_FirstMouseMove = true;
	bool m_MoveForward = false;
	bool m_MoveBackward = false;
	bool m_MoveLeft = false;
	bool m_MoveRight = false;
};
