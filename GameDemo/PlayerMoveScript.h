#pragma once
#include <Script.h>

class TextureBox;
class FoodTableScript;
class ScoreUI;

class PlayerMoveScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PlayerMoveScript)
		PROPERTY(m_moveSpeed)
		PROPERTY(m_hunger)
		PROPERTY(m_notEatenTime)
		PROPERTY(m_hugerDecreaseTime)
		PROPERTY(m_hungerSatisfactionState)
		PROPERTY(m_hungerNormalState)
		PROPERTY(m_satisfactionMoveSpeed)
		PROPERTY(m_normalMoveSpeed)
		PROPERTY(m_dissatisfactionMoveSpeed)
		PROPERTY(m_speedUpBuffTime)
		PROPERTY(m_blockadeActionTime)
		PROPERTY(m_decreaseHunger)
		PROPERTY(m_iSuccessedMiniGame)
		PROPERTY(m_isFoodIntake)
		PROPERTY(m_isPlayerHidden)
		PROPERTY(m_isBlockadeAction)
		PROPERTY(m_skillCount)
		PROPERTY(m_score)
		PROPERTY(m_deliciousScoreIncrease)
		PROPERTY(m_basicScoreIncrease)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerMoveScript, m_name, m_UID, m_moveSpeed, m_hunger
		, m_notEatenTime, m_notEatenTime, m_hungerSatisfactionState, m_hungerNormalState
		, m_satisfactionMoveSpeed, m_normalMoveSpeed, m_dissatisfactionMoveSpeed
		, m_speedUpBuffTime, m_blockadeActionTime, m_decreaseHunger, m_isPlayerHidden
		, m_isBlockadeAction, m_skillCount, m_score, m_basicScoreIncrease, m_deliciousScoreIncrease)
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	~PlayerMoveScript() {};

	void Start() override;
	void Update(double dt) override;
	std::shared_ptr<Component> Clone() const override;

	// 콜백
	virtual void OnCollisionEnter(const Collision& collision);
	virtual void OnCollisionStay(const Collision& collision);
	virtual void OnCollisionExit(const Collision& collision);
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	// 아이템 던지기 (좌클릭)
	void ThrowItem(float power);

	// 푸드 테이블 스크립트 연동
	float* GetHungerPointer() { return &m_hunger; }
	bool* GetSuccessedMiniGameBoolPointer() { return &m_iSuccessedMiniGame; }
	bool* GetPlayMiniGameBoolPointer() { return &m_isPlayMiniGame; }

	// 플레이어의 숨기 상태 포인터 반환
	// 숨기 오브젝트가 해당 bool 값을 조작한다.
	bool* GetPlayerHiddenPointer() { return &m_isPlayerHidden; }

	// 플레이어의 행동 봉쇄 조작 포인터 반환
	// 기술 오브젝트가 해당 bool 값을 조작한다.
	bool* GetPlayerBlockadeActionPointer() { return &m_isBlockadeAction; }

	// 기술 카운트 포인터 반환
	// 기술 오브젝트가 해당 bool 값을 조작한다.
	int* GetPlayerSkillCountPointer() { return &m_skillCount; }

	// 기술 카운트 반환
	int GetSkillCount() const { return m_skillCount; }
	bool AbleSkill() const { return m_ableSkill; }

	// 미니게임에서 조작 할 플레이어 점수
	void BasicScoreIncrease();
	void DeliciousScoreIncrease();

public:
	std::shared_ptr<TextureBox> m_throwObject;
	std::shared_ptr<ScoreUI> m_scoreUI;

	// 캐릭터 이동속도
	float m_moveSpeed = 10.f;						

	// 부드러운 방향전환
	float m_turnSpeed = 7.f;						// 방향전환 속도: 숫자 클수록 빠름
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// 가려는 방향: 방향키에 따라 바로 전환된다
	Vector3 m_headVector = { 0.f, 0.f, -1.f };		// 캐릭터가 바라보는 방향: m_targetVector를 토대로 부드럽게 계산됨

	// 던지기
	Vector3 m_bulletforcevector;
	std::vector<std::shared_ptr<Entity>> m_techItemList;

	// 푸드 테이블 스크립트
	// 콜백 enter, exit 로 관리한다.
	std::shared_ptr<FoodTableScript> m_foodTableScript;


	// ㅡㅡㅡㅡㅡㅡㅡ 플레이어 세부 상태 ㅡㅡㅡㅡㅡㅡㅡㅡ 
	// 배고픔 수치
	float m_hunger = 0.35; /// 100에서 1로 변경

	// 미니게임 성공 (플레이어 행동 봉쇄)
	bool m_iSuccessedMiniGame = false;

	// 음식 섭취 (n초간 이동 속도 증가)
	bool m_isFoodIntake = false;

	// 미니게임 중인가? (플레이어 아이템 던지기와 미니게임 드래그 키가 중복이므로 예외처리)
	bool m_isPlayMiniGame = false;

	// 음식 미섭취 시간
	float m_notEatenTime = 5.f;
	
	// 허기 감소 시간
	float m_hugerDecreaseTime = 1.f;

	// 포만 상태
	float m_hungerSatisfactionState = 0.7f;

	// 보통 상태
	float m_hungerNormalState = 0.5f;

	// 포만 상태 이동 속도
	float m_satisfactionMoveSpeed = 10.f;

	// 보통 상태 이동 속도
	float m_normalMoveSpeed = 8.5f;

	// 공복 상태 이동 속도
	float m_dissatisfactionMoveSpeed = 6.5f;

	// 음식 섭취 후 버프 시간
	float m_speedUpBuffTime = 3.f;

	// 행동 봉쇄 시간
	float m_blockadeActionTime = 1.5f;

	// N초 마다 허기게이지 감소
	float m_decreaseHunger = 0.0175f; /// 0.5에서 0.005로 변경

	// 플레이어가 숨은 상태인지 
	bool m_isPlayerHidden = false;


	// 기술 오브젝트가 사용 할 플레이어 행동 봉쇄 bool 값
	bool m_isBlockadeAction = false;

	// 기술 카운트
	int m_skillCount = 0;
	//스킬 쿨타임
	double m_skillCoolTime = 3;
	//
	bool m_ableSkill = false;

	// 플레이어 점수
	int m_score = 0;

	// 기본 음식가게 점수
	int m_basicScoreIncrease = 1000;

	// 맛집 점수
	int m_deliciousScoreIncrease = 2000;

	bool m_hungerSound = false;
	bool m_hungerSecondSound = false;

	float m_rotationLockTime = -1.f;
};