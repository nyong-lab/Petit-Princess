#pragma once
#include "Renderer.h"

class MeshFilter;

/// <summary>
/// Bone이 없는 Mesh의 렌더러
/// 
/// 24.01.17 박연하
/// </summary>
class MeshRenderer : public Renderer
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MeshRenderer)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshRenderer, m_name, m_UID)
	virtual void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	virtual void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	MeshRenderer();
	MeshRenderer(const std::string& name);

public:
	virtual void Update(double dt) override;
	virtual void Finalize() override;
	virtual void Render(double dt) override;
	
	bool IsInFrustum(std::shared_ptr<MeshFilter> filter, const std::weak_ptr<CameraModule> camera);

public:
	virtual std::shared_ptr<Component> Clone() const override;
};

