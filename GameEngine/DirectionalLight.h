#pragma once
#include "Component.h"

class IGraphics;
class CameraModule;

class DirectionalLight : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(DirectionalLight);
		PROPERTY(m_direction)
		PROPERTY(m_color)
		PROPERTY(m_intensity)
		PROPERTY(m_ambient)
		PROPERTY(m_bloomIntensity)
		PROPERTY(m_bloomThreshold)
		PROPERTY(m_tonemapThreshold)

	// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(DirectionalLight, m_intensity
			, m_bloomIntensity, m_bloomThreshold, m_tonemapThreshold)
	virtual void Serialize(nlohmann::json& json);
	virtual void Deserialize(const nlohmann::json& json);

public:
	DirectionalLight();
	virtual ~DirectionalLight();

public:
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;
	virtual std::shared_ptr<Component> Clone() const override;

	void SetDirection(Vector3 dir) { m_direction = dir; }
	void SetColor255(Vector3 color) { m_color = Vector3(color.x /255, color.y/255, color.z/255); }
	void SetIntensity(float intens) { m_intensity = intens; }
	void SetAmbient255(Vector4 amb) { m_ambient = Vector4(amb.x/255, amb.y/255, amb.z/255, amb.w/255); }
	void SetBloomThreshold(float arg) { m_bloomThreshold = arg; }
	void SetBloomIntensity(float intens) { m_bloomIntensity = intens; }
private:
	GraphicsTransfer::DirectionalLightData m_dLight;
	Vector3 m_direction;
	Vector3 m_color;
	float m_intensity;
	Vector4 m_ambient;

	GraphicsTransfer::LightWorldProperty m_worldProperty;
	float m_bloomIntensity;
	float m_bloomThreshold;
	float m_tonemapThreshold;

	std::shared_ptr<CameraModule> m_mainCamera;
	std::weak_ptr<IGraphics> m_graphics;
};

