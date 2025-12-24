/*
    THIS IS CURRENTLY A TESTING PHASE JUST TO CHECK IF THE PREVIOUS CODE WORKS. 
    OBVIOUSLY, IT MADE A LOT OF REFACTORING AND IT TOOK ME A MORE THAN A WHOLE DAY JUST TO LINK IT ALL TOGETHER
    ARGHHHHHHHHHHHHHHHH. CHATGPT SUCKS, ALL I CAN SAY IS THAT DON'T LET THE AI DO EVERYTHING FOR YOU 
            - Prince Pamintuan, 2025
*/
#include "pch.h"
#include "c_application.h"

// MODELS -- TEMPORARY CAUSE IM TOO LAZY AND I JUST WANT TO LINK IT 
#include "../models/Container.h"
#include "../models/Plane.h"

namespace FuturaLibrary
{
	Application::Application(const std::string& assetRoot)
		: m_AssetRoot(assetRoot)
	{
	}

	Application::~Application() {}

	void Application::Run()
	{
		Window window;
		Input input;
		Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

		if (!window.Initialize(&camera, &input))
			return;

		glEnable(GL_DEPTH_TEST);
		// EventLog::enableReportGlErrors();

		// Use m_AssetRoot instead of hardcoding "../assets/"
		Shader containerShader(
			(m_AssetRoot + "shaders/Container.vs").c_str(),
			(m_AssetRoot + "shaders/Container.fs").c_str()
		);

		Mesh cube(Models::vertices, 5);
		Mesh plane(Models::planeVertices, Models::planeVertexCount, 5);

		Texture texture;
		texture.Add(
			Texture::Dimensions::D2,
			Texture::Wrap::Repeat,
			Texture::Mipmap::LinearLinear,
			true,
			(m_AssetRoot + "texture/gate.jpg").c_str()
		);

		Texture groundTexture;
		groundTexture.Add(
			Texture::Dimensions::D2,
			Texture::Wrap::Repeat,
			Texture::Mipmap::LinearLinear,
			true,
			(m_AssetRoot + "texture/grass.jpg").c_str()
		);

		containerShader.Use();
		containerShader.setInt("texture", 0);

		float lastFrame = 0.0f;

		while (!window.ShouldClose())
		{
			float time = static_cast<float>(glfwGetTime());
			float deltaTime = time - lastFrame;
			lastFrame = time;

			input.Process(window.getWindow(), camera, deltaTime);

			glClearColor(0.58f, 0.925f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			containerShader.Use();

			glm::mat4 projection = glm::perspective(
				glm::radians(camera.Zoom),
				800.0f / 600.0f,
				0.1f,
				100.0f
			);

			containerShader.setMat4("projection", projection);
			containerShader.setMat4("view", camera.GetViewMatrix());

			texture.Use({ texture.GetID() });

			for (int i = 0; i < 2; i++)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), Models::cubePositions[i]);
				model = glm::rotate(
					model,
					time * 25.0f * i,
					glm::vec3(1.0f, 0.3f, 0.5f)
				);

				containerShader.setMat4("model", model);
				cube.Draw();
			}

			groundTexture.Use({ groundTexture.GetID() });
			containerShader.setMat4("model", glm::mat4(1.0f));
			plane.Draw();

			window.Update();
		}

		cube.Dispose();
		plane.Dispose();
		window.Terminate();
	}
}
