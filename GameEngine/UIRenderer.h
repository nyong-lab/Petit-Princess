#pragma once
#include "Renderer.h"

class IGraphics;

class UIRenderer : public Renderer
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(UIRenderer)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(UIRenderer, m_name, m_UID)
	virtual void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	virtual void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	UIRenderer();
	UIRenderer(const std::string& name);

public:
	virtual void Update(double dt) override;
	virtual void Finalize() override;
	virtual void Render(double dt) override;

public:
	virtual std::shared_ptr<Component> Clone() const override;
};

