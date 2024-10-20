#pragma once
#include "Component.h"
#include "GraphicsTransfer.h"  //잘 쓰다가 왜이래 진짜

class IGraphics;
class CameraModule;

class PointLight : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PointLight);
		PROPERTY(m_color)
		PROPERTY(m_positionOffset)
		PROPERTY(m_intensity)
		PROPERTY(m_attenuation)
		PROPERTY(m_range)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PointLight, m_intensity, m_range)
	virtual void Serialize(nlohmann::json& json);
	virtual void Deserialize(const nlohmann::json& json);

public:
	PointLight();
	virtual ~PointLight();

public:
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void DebugRender() override;
	virtual std::shared_ptr<Component> Clone() const override;

	void SetColor255(Vector3 color) { m_color = Vector3(color.x/255, color.y/255, color.z/255); }
	void SetIntensity(float intens) { m_intensity = intens; }
	float GetIntensity() { return m_intensity; }
	void SetPosOffset(const Vector3& vec) { m_positionOffset = vec; }

private:
	Vector3 m_color = Vector3(1.f, 1.f, 1.f);
	Vector3 m_positionOffset = Vector3(0.f, 0.f, 0.f);
	float m_intensity = 1.f;
	Vector3 m_attenuation = Vector3(1, 0.5, 0.5);  //감쇠( 각각 constant 0차, linear 1차, Quadratic 2차)
	float m_range = 25.f;

	GraphicsTransfer::PointLightData m_light;
	std::shared_ptr<CameraModule> m_mainCamera;
	std::weak_ptr<IGraphics> m_graphics;

	/// ** Debug Render **
	bool m_isModelCreate = false;
	std::wstring m_modelKey = L"";
	GraphicsTransfer::ModelKey m_cloneModelKey;	 // Clone을 위한 원본 키
};

