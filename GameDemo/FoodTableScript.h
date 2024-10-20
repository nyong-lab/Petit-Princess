#pragma once

#include "Script.h"
#include "MiniGameDefine.h"


class PlayerMoveScript;


/// <summary>
/// 2024.02.14
/// 미니게임을 생성,삭제 등 관리하는 관리자 클래스.
/// 이 클래스에서 미니게임 스크립트를 들고있는 엔티티를 생성하기도 하고 삭제하기도 한다.
/// 
/// 작성자 : 박연하,김동일
/// </summary>
class FoodTableScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(FoodTableScript)
	PROPERTY(m_isClear)
	//PROPERTY(m_foodType)
	PROPERTY(m_isDeliciousFood)
	PROPERTY(m_reVisitEndTime)
	PROPERTY(m_merchantSenseTime)
	PROPERTY(m_isGuardArrive)
	
	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FoodTableScript, m_name, m_UID, m_miniGameID, m_isClear, m_foodType
	, m_isDeliciousFood, m_reVisitEndTime, m_merchantSenseTime, m_isGuardArrive)

	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	FoodTableScript();
	FoodTableScript(FoodType type);
	virtual ~FoodTableScript();

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);
	virtual void Update(double dt) override;
	virtual void Render(double dt)	 override;
	void SetDelicious(bool truefalse) { m_isDeliciousFood = truefalse; }

	virtual std::shared_ptr<Component> Clone() const override;

public:
	bool GetInteractive() const { return m_isInteractiveTrigger; }
	FoodType GetMiniGameType() const { return m_foodType; }

	// FSM이 들고있는 상인 눈치게이지를 포인터로 가져온다.
	// 단 FSM은 Start 함수 호출되있어야 한다.
	void SetPointerSense(double* pointerSense) { m_merchantSense = pointerSense; }
	void SetSense(double value) { *m_merchantSense = value; }

	// 에디터에서 음식 가게 필터를 고를 시 음식 타입을 설정해 준다.
	void SetFoodType(FoodType foodType);
	
	// 플레이어가 훔쳐먹기를 끝냈을 때 상태 변화
	void SetAteFood(bool value) { m_isPlayerAteFood = value; }

public:
	// 게임 매니저에서 모든 미니게임 클리어 조건을 체크하기 위한 ID
	static int m_miniGameLastID;
	int m_miniGameID = 0;

	bool m_isInteractiveTrigger = false;
	bool m_isClear = false;
	bool m_isMiniGamePlay = false;

	// 타입에 해당하는 스테이지별 미니게임 정보들
	std::vector<MiniGameInfo> m_miniGameInformations;

	// 현재 스테이지에 해당하는 미니게임 정보
	std::shared_ptr<MiniGameInfo> m_currentMiniGameInfo;

	// 미니게임 타입
	FoodType m_foodType = FoodType::None;

	// 실제 미니게임 로직을 체크하는 엔티티 (QTE 스크립트를 가지고 있음)
	std::shared_ptr<Entity> m_miniGameEntity;

	// 상인 눈치 게이지 
	double* m_merchantSense = nullptr;

	// 플레이어 허기 게이지
	float* m_playerHunger = nullptr;

	// 플레이어의 미니게임 성공 여부
	bool* m_isPlayerSuccessedMiniGame = nullptr;

	// 플레이어의 미니게임 플레이 여부
	bool* m_isPlayerPlayMiniGame = nullptr;

	// 맛집 여부
	bool m_isDeliciousFood = false;

	// 플레이어가 음식 훔쳐먹기를 끝냈는지
	bool m_isPlayerAteFood = false;

	// 재방문 시간
	float m_reVisitEndTime = 5.f;
	
	// 재방문 경과 시간
	float m_reVisitElapsedTime = 0.f;

	// 맛집 - 상인 눈치 100퍼 발동 시간
	float m_merchantSenseTime = 3.f;

	// 경비병이 도착했는지
	bool m_isGuardArrive = false;

	// UI에서 사용하기 위한 (점수) 함수 호출을 위한 스크립트 포인터
	std::weak_ptr<PlayerMoveScript> m_playerScript;

	float m_senseElapsedTime = 0.f;
};
