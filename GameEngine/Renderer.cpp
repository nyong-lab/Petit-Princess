#include "GameEngine_pch.h"
#include "Renderer.h"
#include "IGraphics.h"

Renderer::Renderer()
	: Component()
{

}

Renderer::Renderer(const std::string& name)
	: Component(name)
{

}

Renderer::~Renderer()
{

}

void Renderer::Update(double dt)
{

}

void Renderer::Finalize()
{

}

void Renderer::Render(double dt)
{

}

std::shared_ptr<Component> Renderer::Clone() const
{
	return nullptr;
}

