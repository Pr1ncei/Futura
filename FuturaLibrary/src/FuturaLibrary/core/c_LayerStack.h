#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "c_Layer.h"
#include <vector>

namespace FuturaLibrary
{
	// It basiscally uses the Stack Data Structure (Push and Pop)
	// This is use for the UI hierarchy such as the GameUI (Menu, Health, Weapon Select) is on top of the Game
	// I mean you kinda have to stack the layers, what else should you do for it :P
	class FT_API LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack()
		{
			for (auto layer : m_Layers)
			{
				layer->OnDetach();
				delete layer;
			}
		}

		void PushLayer(Layer* layer)
		{
			m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
			m_LayerInsertIndex++;
			layer->OnAttach();
		}

		void PushOverlay(Layer* overlay)
		{
			m_Layers.emplace_back(overlay); 
			overlay->OnAttach(); 
		}

		void PopLayer(Layer* layer)
		{
			auto target = std::find(m_Layers.begin(), m_Layers.end(), layer); 
			if (target != m_Layers.end())
			{
				layer->OnDetach(); 
				m_Layers.erase(target);
				m_LayerInsertIndex--; 
			}
		}

		void PopOverlay(Layer* overlay)
		{
			auto target = std::find(m_Layers.begin(), m_Layers.end(), overlay);
			if (target != m_Layers.end())
			{
				overlay->OnDetach(); 
				m_Layers.erase(target); 
			}
		}

		template<typename T> 
			requires(std::is_base_of_v<Layer, T>)
		T* GetLayer()
		{
			for (const auto& layer : m_Layers) 
			{
				if (auto casted = dynamic_cast<T*>(layer.get()))
				{
					return casted; 
				}
			}

			return nullptr; 
		}

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private: 
		std::vector<Layer*> m_Layers; 
		unsigned int m_LayerInsertIndex = 0; 
	};
}