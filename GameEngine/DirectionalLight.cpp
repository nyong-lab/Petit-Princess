#include "GameEngine_pch.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "IGraphics.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "MathUtils.h"

void DirectionalLight::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_direction"] =
	{
		{"x", m_direction.x},
		{"y", m_direction.y},
		{"z", m_direction.z}
	};
	json["m_color"] =
	{
		{"x", m_color.x},
		{"y", m_color.y},
		{"z", m_color.z}
	};
	json["m_ambient"] =
	{
		{"x", m_ambient.x},
		{"y", m_ambient.y},
		{"z", m_ambient.z},
		{"w", m_ambient.w}
	};
}

void DirectionalLight::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_direction"))
		{
			m_direction.x = json["m_direction"]["x"];
			m_direction.y = json["m_direction"]["y"];
			m_direction.z = json["m_direction"]["z"];
		}

		if (json.contains("m_color"))
		{
			m_color.x = json["m_color"]["x"];
			m_color.y = json["m_color"]["y"];
			m_color.z = json["m_color"]["z"];
		}

		if (json.contains("m_ambient"))
		{
			m_ambient.x = json["m_ambient"]["x"];
			m_ambient.y = json["m_ambient"]["y"];
			m_ambient.z = json["m_ambient"]["z"];
			m_ambient.w = json["m_ambient"]["w"];
		}
	}
}

DirectionalLight::DirectionalLight()
	:m_direction(Vector3(0.6,-1.5,0.5)), m_color(Vector3(1,1,1)), m_intensity(0.14f), m_ambient(Vector4(1,1,1,247.f/255)),
	m_bloomIntensity(1.f), m_bloomThreshold(1.5f), m_tonemapThreshold(1.4f)
{
	m_dLight = GraphicsTransfer::DirectionalLightData();
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Start()
{
	m_mainCamera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();
	m_graphics = m_owner.lock()->GetGraphics();
}

void DirectionalLight::Update(double dt)
{
	m_color = MathUtils::Clamp(m_color, Vector3(0, 0, 0), Vector3(1, 1, 1));
	(m_intensity < 0) ? m_intensity = 0 : m_intensity;
	(m_ambient.w < 0) ? m_ambient.w = 0 : m_ambient.w;
	(m_bloomIntensity < 0) ? m_bloomIntensity = 0 : m_bloomIntensity;
	(m_bloomThreshold < 0) ? m_bloomThreshold = 0 : m_bloomThreshold;
	(m_tonemapThreshold < 0) ? m_tonemapThreshold = 0 : m_tonemapThreshold;
}

void DirectionalLight::Render(double dt)
{
	m_dLight.color = m_color;
	m_dLight.intensity = m_intensity;
	m_dLight.direction = Vector4(m_direction.x, m_direction.y, m_direction.z, 0);
	Vector3 eye = m_mainCamera->GetPosition();
	m_dLight.eyePos = Vector4(eye.x, eye.y, eye.z, 1.f);
	m_dLight.ambient = m_ambient;

	m_graphics.lock()->LightRender(m_dLight);
//------------------------------------------------------------------------------------

	m_worldProperty.bloomIntensity = m_bloomIntensity;
	m_worldProperty.bloomThreshold = m_bloomThreshold;
	m_worldProperty.tonemapThreshold = m_tonemapThreshold;

	m_graphics.lock()->SetWorldLightProperty(m_worldProperty);
}

void DirectionalLight::Finalize()
{
	m_dLight.color = Vector3(1, 1, 1);
	m_dLight.intensity = 0.f;
	m_dLight.direction = Vector4(m_direction.x, m_direction.y, m_direction.z, 0);
	Vector3 eye = {};
	if(m_mainCamera)
		eye = m_mainCamera->GetPosition();
	m_dLight.eyePos = Vector4(eye.x, eye.y, eye.z, 1.f);
	m_dLight.ambient = m_ambient;

	if(m_graphics.lock())
		m_graphics.lock()->LightRender(m_dLight);
}

std::shared_ptr<Component> DirectionalLight::Clone() const
{
	auto clone = std::make_shared<DirectionalLight>();
	clone->m_direction = this->m_direction;
	clone->m_color = this->m_color;
	clone->m_intensity = this->m_intensity;
	clone->m_dLight = this->m_dLight;
	clone->m_ambient = this->m_ambient;

	return clone;
}
