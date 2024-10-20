#pragma once
#include "IBuilder.h"
#include "IGraphics.h"

class Component;

class MeshRenderBuilder : public IBuilder<Component>
{
public:
	MeshRenderBuilder();
	MeshRenderBuilder(const std::string& name);
	MeshRenderBuilder(const std::string& name, IGraphics* graphics);
	~MeshRenderBuilder();

	virtual std::shared_ptr<Component> Create() override;
private:

	std::string m_name;
	IGraphics* m_graphics;
};

