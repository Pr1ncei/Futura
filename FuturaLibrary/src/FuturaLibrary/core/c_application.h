#pragma once

#include "c_core.h"
#include <memory>
#include "c_window.h"
#include "c_input.h"
#include "../graphics/g_shader.h"
#include "../graphics/g_mesh.h"
#include "../graphics/g_texture.h"
#include "../core/c_camera.h"

namespace FuturaLibrary
{
	class FT_API Application
	{
	public:
		Application(const std::string& assetRoot);
		virtual ~Application();

		void Run();

	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnShutdown() {}

	private:
		bool m_Running = true; 
		std::string m_AssetRoot; 
	};

	std::unique_ptr<Application> CreateApplication(const std::string& assetRoot); 
}