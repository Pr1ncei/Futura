#include "pch.h"
#include "c_Layer.h"

namespace FuturaLibrary
{
	Layer::Layer(const std::string& debugName) : m_DebugName(debugName) {}
	Layer::~Layer() {}
	void Layer::OnAttach() {}
	void Layer::OnDetach() {}
	void Layer::OnUpdate() {}
	void Layer::OnRender() {}
	void Layer::OnEvent(Event& event) {}
}
