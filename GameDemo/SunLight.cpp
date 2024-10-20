#include "GameDemo_pch.h"
#include "SunLight.h"

#include "Entity.h"
#include "DirectionalLight.h"
#include "GameManager.h"
#include "MathUtils.h"

SunLight::SunLight()
{

}

SunLight::~SunLight()
{

}

void SunLight::Start()
{
    m_gameMgr = m_owner.lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GameManager>();
	m_dir = m_owner.lock()->GetComponent<DirectionalLight>();
	m_endTime = m_gameMgr->GetGameTime();

	float morning = 0.0f;
	float noon = 0.1f;
	float evenStart = 0.7f;
	float even = 0.8f;
	double night = 0.9f;

	m_noonTime = noon * m_endTime;
	m_evenStartTime = evenStart * m_endTime;
	m_evenTime = even * m_endTime;
	m_nightTime = night * m_endTime;

	m_dir->SetBloomThreshold(2.f);
	m_dir->SetBloomIntensity(0.5f);
}

void SunLight::Update(double dt)
{
  	m_timer = m_gameMgr->GetCurrentGameTime();
  
  	// ¹æÇâÀº °è¼Ó ÀÏÁ¤ÇÏ°Ô º¯È­
  	Vector3 curDir = MathUtils::Lerp(m_startDir, m_finalDir, m_timer / m_endTime);
  	m_dir->SetDirection(curDir);
  
  	// ¾ÆÄ§ ~ ³·
  	if ( m_timer < m_noonTime )
  	{
  		Vector3 color = MathUtils::Lerp(m_morningColor, m_noonColor,  m_timer / m_noonTime );
  		m_dir->SetColor255(color);
  	}
  
  	// ³· ~ Àú³á
  	else if (m_timer >= m_evenStartTime && m_timer < m_evenTime)
  	{
  		Vector3 color = MathUtils::Lerp(m_noonColor, m_evenColor, (m_timer - m_evenStartTime) / (m_evenTime - m_evenStartTime));
  		float intens = MathUtils::Lerp(m_noonIntensity, m_evenIntensity, (m_timer - m_evenStartTime) / (m_evenTime - m_evenStartTime));
		Vector4 amb = MathUtils::Lerp(m_noonAmb, m_evenAmb, (m_timer - m_evenStartTime) / (m_evenTime - m_evenStartTime));
  		m_dir->SetColor255(color);
  		m_dir->SetIntensity(intens);
		m_dir->SetAmbient255(amb);
  	}
  	
  	// Àú³á ~ ¹ã
  	else if (m_timer >= m_evenTime && m_timer < m_nightTime)
  	{
  		Vector3 color = MathUtils::Lerp(m_evenColor, m_nightColor, (m_timer - m_evenTime) / (m_nightTime - m_evenTime));
  		float intens = MathUtils::Lerp(m_evenIntensity, m_nightIntensity, (m_timer - m_evenTime) / (m_nightTime - m_evenTime));
  		Vector4 amb = MathUtils::Lerp(m_evenAmb, m_nightAmb, (m_timer - m_evenTime) / (m_nightTime - m_evenTime));
  		m_dir->SetColor255(color);
  		m_dir->SetIntensity(intens);
  		m_dir->SetAmbient255(amb);
  	}
  
  	// ¹ã ~
  	else if(m_timer >= m_nightTime)
  	{
  		Vector4 amb = MathUtils::Lerp(m_nightAmb, m_midNightAmb, (m_timer - m_nightTime) / (m_endTime - m_nightTime));
  		m_dir->SetAmbient255(amb);
  	}
  
  	// Àú³á ºÎÅÍ ºí·ë ÀÓ°è°ª ³·Ãß±â
  	if (m_timer >= m_evenStartTime && m_timer < m_nightTime)
  	{
  		float bloom = MathUtils::Lerp(2.0, 1.1, (m_timer - m_evenStartTime) / (m_nightTime - m_evenStartTime));
  		m_dir->SetBloomThreshold(bloom);
  	}
}


std::shared_ptr<Component> SunLight::Clone() const
{
	auto clone = std::make_shared<SunLight>();
	return clone;
}
