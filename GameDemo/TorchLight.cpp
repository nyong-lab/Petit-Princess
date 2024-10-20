#include "GameDemo_pch.h"
#include "TorchLight.h"
#include "PointLight.h"
#include "Entity.h"
#include "MathUtils.h"


TorchLight::TorchLight()
	:m_onTime(60 * 0.85),  //  ( 저녁 ~ 밤 사이 )
	m_changeTime(0), m_isGoingLighter(-1),
	gen(rd()), changeDist(0.2f, 2.5f), onDist(-1.5,1.5)
{

}

TorchLight::~TorchLight()
{

}

void TorchLight::Start()
{
	std::uniform_int_distribution<int> random(143, 203);
	Vector3 color;

	m_point = m_owner.lock()->GetComponent<PointLight>();
	m_point->SetColor255(Vector3(m_color.x, random(gen), m_color.z));

	if (!m_isLightOn)
	{
		m_point->SetIntensity(0.f);
		m_onTime += onDist(gen);     // 켜지는 시간에 약간의 랜덤성 부여
	}
	else
	{
		m_isWorkingDone = true;
	}
}

void TorchLight::Update(double dt)
{
	m_timer += dt;

	// 불 켜기
	if (!m_isLightOn && m_timer > m_onTime)
	{
		m_isLightOn = true;
	}

	if (m_isLightOn && !m_isWorkingDone)
	{
		float intensity = m_point->GetIntensity();
		intensity = MathUtils::Lerp(intensity, m_maxLuma, dt * 0.5);
		m_point->SetIntensity(intensity);

		if ((m_maxLuma + m_minLuma) * 0.5 < intensity)
		{
			m_isWorkingDone = true;
		}
	}


	// 불 깜빡이기
	if (m_isWorkingDone)
	{
		if (m_isGoingLighter == -1)
		{
			float intensity = m_point->GetIntensity();
			intensity = MathUtils::Lerp(m_maxLuma, m_minLuma, m_timer / m_changeTime);
			m_point->SetIntensity(intensity);
		}
		else
		{
			float intensity = m_point->GetIntensity();
			intensity = MathUtils::Lerp(m_minLuma, m_maxLuma, m_timer / m_changeTime);
			m_point->SetIntensity(intensity);
		}

		if (m_changeTime < m_timer)
		{
			m_isGoingLighter *= -1;
			m_timer -= m_changeTime;
			m_changeTime = changeDist(gen);
		}
	}
}

std::shared_ptr<Component> TorchLight::Clone() const
{
	auto clone = std::make_shared<TorchLight>();
	return clone;
}
