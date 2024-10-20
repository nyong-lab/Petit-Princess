#include "GameEngine_pch.h"
#include "PointLight.h"
#include "Entity.h"
#include "IGraphics.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "MathUtils.h"
#include "GraphicsTransfer.h"

void PointLight::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_color"] =
	{
		{"x", m_color.x},
		{"y", m_color.y},
		{"z", m_color.z}
	};

	json["m_positionOffset"] =
	{
		{"x", m_positionOffset.x},
		{"y", m_positionOffset.y},
		{"z", m_positionOffset.z}
	};

	json["m_attenuation"] =
	{
		{"x", m_attenuation.x},
		{"y", m_attenuation.y},
		{"z", m_attenuation.z}
	};
}

void PointLight::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_positionOffset"))
		{
			m_positionOffset.x = json["m_positionOffset"]["x"];
			m_positionOffset.y = json["m_positionOffset"]["y"];
			m_positionOffset.z = json["m_positionOffset"]["z"];
		}

		if (json.contains("m_color"))
		{
			m_color.x = json["m_color"]["x"];
			m_color.y = json["m_color"]["y"];
			m_color.z = json["m_color"]["z"];
		}

		if (json.contains("m_attenuation"))
		{
			m_attenuation.x = json["m_attenuation"]["x"];
			m_attenuation.y = json["m_attenuation"]["y"];
			m_attenuation.z = json["m_attenuation"]["z"];
		}
	}
}

PointLight::PointLight()
{

}

PointLight::~PointLight()
{

}

void PointLight::Start()
{
	m_mainCamera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();
	m_graphics = m_owner.lock()->GetGraphics();
}

void PointLight::Update(double dt)
{
	m_color = MathUtils::Clamp(m_color, Vector3(0, 0, 0), Vector3(1, 1, 1));
	(m_intensity < 0) ? m_intensity = 0 : m_intensity ;
	(m_attenuation.x < 0) ? m_attenuation.x = 0 : m_attenuation.x;
	(m_attenuation.y < 0) ? m_attenuation.y = 0 : m_attenuation.y;
	(m_attenuation.z < 0) ? m_attenuation.z = 0 : m_attenuation.z;
	(m_range < 0) ? m_range = 0 : m_range;
}

void PointLight::Render(double dt)
{
	m_light.color = Vector4(m_color.x, m_color.y, m_color.z, 1);
	m_light.position = m_positionOffset + GetTransform()->GetWorldPosition();
	m_light.intensity = m_intensity;
	m_light.attenuation = m_attenuation;
	m_light.range = m_range;

	m_graphics.lock()->LightRender(m_light);
}

void PointLight::DebugRender()
{
	if (!m_isModelCreate)
	{
		m_cloneModelKey = ModelKey::WireFrameSphere;
		m_modelKey = m_graphics.lock()->LoadModel(m_cloneModelKey);
		m_isModelCreate = true;
	}

	auto transform = GetTransform();
	Vector3 tsl = transform->GetWorldPosition() + m_positionOffset;
	Quaternion quat = transform->GetWorldRotation();
	Matrix world = Matrix::CreateScale(0.2f, 0.2f, 0.2f)
		* Matrix::CreateFromQuaternion(quat)
		* Matrix::CreateTranslation(tsl);

	// 상수버퍼 업데이트

	GraphicsTransfer::CbTransform cbTransform;
	cbTransform.world = world;
	cbTransform.viewProj = m_mainCamera->GetViewProjectionMatrix();

	GraphicsTransfer::CbPhysics cbPhysics;
	cbPhysics.isCollision = 2;

	GraphicsTransfer::ModelData data;
	data.modelKey = m_modelKey;
	data.transform = cbTransform;
	data.physics = cbPhysics;

	m_graphics.lock()->ObjectRender(data);
}

std::shared_ptr<Component> PointLight::Clone() const
{
	auto clone = std::make_shared<PointLight>();
	clone->m_positionOffset = this->m_positionOffset;
	clone->m_color = this->m_color;
	clone->m_intensity = this->m_intensity;
	clone->m_attenuation = this->m_attenuation;
	clone->m_range = this->m_range;
	clone->m_light = this->m_light;

	return clone;
}