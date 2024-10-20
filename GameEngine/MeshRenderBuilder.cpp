#include "GameEngine_pch.h"

#include "Component.h"
#include "MeshRenderBuilder.h"
#include "MeshRenderer.h"

MeshRenderBuilder::MeshRenderBuilder()
{

}

MeshRenderBuilder::MeshRenderBuilder(const std::string& name)
{

}

MeshRenderBuilder::MeshRenderBuilder(const std::string& name, IGraphics* graphics)
{

}

MeshRenderBuilder::~MeshRenderBuilder()
{

}

std::shared_ptr<Component> MeshRenderBuilder::Create()
{
	std::shared_ptr<MeshRenderer> returnRenderer = std::make_shared<MeshRenderer>();


	return  returnRenderer;
}
