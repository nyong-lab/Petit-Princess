#include "GameEngine_pch.h"
#include "MeshFilter.h"
#include "IGraphics.h"
#include "Entity.h"

void MeshFilter::PBRSet::Serialize(nlohmann::json& json)
{
	json["PBRSet"] = *this;

	json["PBRSet"]["baseColor"] =
	{
		{"x", baseColor.x},
		{"y", baseColor.y},
		{"z", baseColor.z},
		{"w", baseColor.w}
	};
}

void MeshFilter::PBRSet::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("baseColor"))
		{
			baseColor.x = json["baseColor"]["x"];
			baseColor.y = json["baseColor"]["y"];
			baseColor.z = json["baseColor"]["z"];
			baseColor.w = json["baseColor"]["w"];
		}
	}
}


void MeshFilter::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	m_pbrSet.Serialize(json);

	json["m_cullingBox"] =
	{
		{"x", m_cullingBox.x},
		{"y", m_cullingBox.y},
	};
}

void MeshFilter::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_cullingBox"))
		{
			m_cullingBox.x = json["m_cullingBox"]["x"];
			m_cullingBox.y = json["m_cullingBox"]["y"];
		}

		if (json.contains("PBRSet"))
			m_pbrSet.Deserialize(json["PBRSet"]);
	}
}

MeshFilter::MeshFilter()
	: m_cloneModelKey(GraphicsTransfer::ModelKey::None)
{

}

MeshFilter::MeshFilter(const std::string& name)
	: m_cloneModelKey(GraphicsTransfer::ModelKey::None)
{

}

void MeshFilter::UpdateModelKey()
{
	SetModelKey(m_cloneModelKey);
}

void MeshFilter::SetModelKey(GraphicsTransfer::ModelKey modelKey)
{
	m_cloneModelKey = modelKey;
	m_modelKey = m_owner.lock()->GetGraphics().lock()->LoadModel(m_cloneModelKey);
}

std::shared_ptr<Component> MeshFilter::Clone() const
{
	auto clone = std::make_shared<MeshFilter>();

	clone->m_cloneModelKey = m_cloneModelKey;
	clone->m_pbrSet = m_pbrSet;

	return clone;
}

