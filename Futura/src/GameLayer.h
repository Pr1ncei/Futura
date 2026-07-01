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
#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_texture.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"

class GameLayer : public FuturaLibrary::Layer
{
public:
	GameLayer();
	~GameLayer() override = default;

	void OnAttach() override;
	void OnUpdate() override;
	void OnRender() override;

private:
	FuturaLibrary::Ref<FuturaLibrary::Shader> m_Shader;
	FuturaLibrary::Ref<FuturaLibrary::Texture2D> m_Texture;
	FuturaLibrary::Ref<FuturaLibrary::VertexArray> m_VertexArray;
	Camera m_Camera;
};
