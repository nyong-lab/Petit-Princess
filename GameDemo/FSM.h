#pragma once
#include <../GameEngine/Component.h>
#include "State.h"




/// <summary>
/// 일단 상태를 다 적어보자
/// 적다보니 중복되는것도있고 아닌것도 있고
/// </summary>
//enum class State
//{
//	//순찰병
//	Idle,
//	patrol,
//	chasing,
//	called,
//	chasefinding, //추적중 두리번
//	patrolfinding, // 순찰중 두리번
//	fishinged,
//	comeback,
//
//	//상인
//	shopkeeperIdle, //기본
//	calling, //순찰병 호출
//	doubt, //눈치 게이지 일정 수준 이하
//	angry, //눈치 게이지 일정 수준 이상
//
//	//플레이어
//	playerIdle,
//	Steel,
//	Eat,
//	hide,
//	fishing, //경비병 낚시
//
//};


/// <summary>
/// 2024.01.31
/// 순찰병의 상태를 만들어야한다
/// 대기(일반) / 순찰 / 추격 / 호출 / 복귀 / 두리번(추격중 목표를 찾지 못함/ 순찰 중 일정거리 움직이고 목푤를 탐색함) / 유도됨
/// 
/// 모든 캐릭터들이 상태에 따른 다른 애니메이션을 할것이다
/// 상인은 눈치게이지에따라 상태가 변할거고
/// 플레이어는 키입력과 충돌에 따른 상태가 변할것이다
/// 결국 공통적으로 fsm을 갖고 있다 그러니 코드 중복 귀찮으니 클래스 묶어 상속받자
/// 
/// state design pattern
/// 
/// 작성 유승운
/// </summary>
class FSM :
    public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(FSM)

	// Serialize
	void Serialize(nlohmann::json& json) abstract;
	void Deserialize(const nlohmann::json& json) abstract;

public:

	FSM();
	virtual ~FSM();

	virtual void OnStateEnter() abstract; //상태 진입시 한번만 발생하는 함수
	virtual void OnStateExit() abstract; //상태 변화에 발생하는 함수

	virtual void Update(double dt) abstract;
	virtual void Render(double dt) abstract;
	virtual void Finalize() abstract;
	virtual std:: shared_ptr<Component> Clone() const abstract;

	std::shared_ptr<State> GetState() const { return m_curState; }


protected:
	void ChangeState(std::shared_ptr<State> state);
	std::shared_ptr<State> m_curState;

	///fsm condition, transition
	std::vector < std::vector<bool>> m_transition;
};

