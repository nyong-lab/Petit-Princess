#include "GameDemo_pch.h"
#include "State.h"
#include "FSM.h"

#include <set>

#include "IGraphics.h"
#include "MeshFilter.h"
#include "InputManager.h"
#include "SoundManager.h"

#include "Entity.h"
#include "ShopKeeperFSM.h"
#include "PlayerFSM.h"
#include "GuardFSM.h"


#include "Transform.h"
#include "SphereCollider.h"
#include "ShopKeeperCallingScript.h"
#include "ChaseRangeScript.h"


State::State(std::shared_ptr<FSM> fsm) : m_fsm(fsm)
{

}

State::~State()
{
	m_fsm.reset();
}

#pragma region Guard

#pragma region GuardState
GuardState::GuardState(std::shared_ptr<FSM> fsm) : State(fsm), m_dirX(0.f), m_dirY(0.f), m_curPoint(0, 0, 0), m_destination(0, 0, 0), prePath(), curPath()
{
}

GuardState::~GuardState()
{

}

void GuardState::Finalize()
{

	while (!m_path.empty())
	{
		m_path.pop();
	}
}

void GuardState::SetPath(std::stack<AstarNode> path)
{
	if (!path.empty())
	{
		if (m_path.empty())
		{
			m_path = path;
			curPath = m_path.top();
			prePath = m_path.top();
		}
		else
		{
			while (!m_path.empty())
			{
				m_path.pop();
			}

			m_path = path;
			prePath = m_path.top();
			m_path.pop();
			curPath = m_path.top();
		}
	}
}

void GuardState::Move(double dt)
{
	//다음 경로가 준비되어있으면 다음노드 설정
	curPath = m_path.top();

	//방향 벡터를 연산해준다
	m_dirY = curPath.pos.first - m_curPoint.z;
	m_dirX = curPath.pos.second - m_curPoint.x;

	float rangeX = curPath.pos.second - m_curPoint.x;
	float rangeY = curPath.pos.first - m_curPoint.z;

	/*if (abs(rangeX) > 0.3 || abs(rangeY) > 0.3)
	{
		AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwByte(0);
		std::string test1 = std::to_string(rangeX);
		std::string test2 = std::to_string(rangeY);

		std::string teststr = "X : " + test1 + " Y : " + test2 + "\n";
		WriteFile(hConsole, teststr.c_str(), strlen(teststr.c_str()), &dwByte, nullptr);
	}*/

	///해당 위치 근처에 도달하면 목표 경로를 변경 이거 오차가 너무 커요!!! 어케해줘 -> 중간에 pos값이 바뀌면 절대 안들어오네
	//TODO:: 이걸 해결해야한다 - 어딘가 부딪치면 경로대로 가는 벡터에서 현재 내위치에서 경로로 다시 탐색해야할것
	if (abs(rangeX) < 0.2 && abs(rangeY) < 0.2)
	{

		//근처에 왔으니까 위치 보정
		m_fsm->GetOwner().lock()->GetComponent<Transform>()->SetWorldPosition(Vector3(curPath.pos.second, 1.5f, curPath.pos.first));
		//해당 경로를 설정했으니 경로 목록에서 빼기
		prePath = m_path.top();
		m_path.pop();
	}

	//그대로 transform 더한다
	Vector3 direction(m_dirX, 0, m_dirY);
	//방향벡터 노말라이즈
	direction.Normalize(direction);

	m_fsm->GetOwner().lock()->GetComponent<Transform>()->AddPosition(direction * dt * m_moveSpeed);
	m_targetVector += direction;


	m_targetVector.Normalize();

	// 부드럽게 방향 전환하기
	DirectX::SimpleMath::Quaternion targetQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_targetVector.x, m_targetVector.z), 0.f, 0.f);
	DirectX::SimpleMath::Quaternion headQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_headVector.x, m_headVector.z), 0.f, 0.f);
	DirectX::SimpleMath::Quaternion resultQuat = DirectX::SimpleMath::Quaternion::Slerp(headQuat, targetQuat, dt * m_turnSpeed);
	m_headVector = DirectX::SimpleMath::Vector3::Transform(Vector3::UnitZ, resultQuat);
	m_fsm->GetOwner().lock()->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, DirectX::XMConvertToDegrees(atan2f(m_headVector.x, m_headVector.z)), 0.f));

}

#pragma endregion	

#pragma region GuardIdle
GuardIdle::GuardIdle(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardIdle::~GuardIdle()
{
}

void GuardIdle::OnStateEnter()
{

}

void GuardIdle::OnStateExit()
{
}

void GuardIdle::Update(double dt)
{

}

void GuardIdle::Finalize()
{

}

#pragma endregion

#pragma region GuardPatrol
GuardPatrol::GuardPatrol(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardPatrol::~GuardPatrol()
{

}

void GuardPatrol::OnStateEnter()
{
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolWalk");


	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		//prePath = m_path.top();
		curPath = m_path.top();
	}
}

void GuardPatrol::OnStateExit()
{
	if (!m_path.empty())
	{
		while (!m_path.empty())
		{
			m_path.pop();
		}
	}

}

void GuardPatrol::Update(double dt)
{
	//update마다 내 위치 매번 갱신 필요
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);
	}
	//경로에 도착했다
	else
	{
		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::search);
		return;
	}
}

void GuardPatrol::Finalize()
{

}

#pragma endregion

#pragma region GuardChasing
GuardChasing::GuardChasing(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardChasing::~GuardChasing()
{

}

void GuardChasing::OnStateEnter()
{

	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolRun");

	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		//prePath = m_path.top();
		curPath = m_path.top();
	}
}

void GuardChasing::OnStateExit()
{
	if (!m_path.empty())
	{
		while (!m_path.empty())
		{
			m_path.pop();
		}
	}

	// 추격 끝 브금전환
	SoundManager::GetInstance()->FadeOutBGM("InGame_ChaseState.mp3", 1.f);
	SoundManager::GetInstance()->ResumeFadeInBGM("InGame_Default.wav", 0.5f, 1.f);
}

void GuardChasing::Update(double dt)
{
	static double time = 0;
	time += dt;

	//update마다 내 위치 매번 갱신 필요
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();


	if (!m_path.empty())
	{
		Move(dt);
	}
	//경로에 도착했다
	else
	{
		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::search);
		time = 0;
		return;
	}
}

void GuardChasing::Finalize()
{

}

#pragma endregion

#pragma region GuardCalled
GuardCalled::GuardCalled(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardCalled::~GuardCalled()
{

}

void GuardCalled::OnStateEnter()
{
	//m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolRun");

	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		prePath = m_path.top();
		curPath = m_path.top();
	}
}

void GuardCalled::OnStateExit()
{
	if (!m_path.empty())
	{
		while (!m_path.empty())
		{
			m_path.pop();
		}
	}

}

void GuardCalled::Update(double dt)
{
	//가까운 경로로 움직이기만 하게 하고
	//fsm에서 경로를 갖고 경로에 따른 벡터만 넘겨서 움직이게 한다?

	//update마다 내 위치 매번 갱신 필요
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);
	}
	//경로에 도착했다
	else
	{
		//test 상태 변경
		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::search);
		return;
	}
}

void GuardCalled::Finalize()
{

}

#pragma endregion

#pragma region GuardSearch
GuardSearch::GuardSearch(std::shared_ptr<FSM> fsm) : GuardState(fsm), m_duration(0)
{

}

GuardSearch::~GuardSearch()
{

}

void GuardSearch::OnStateEnter()
{
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolMiss");

	m_duration = 0;
}

void GuardSearch::OnStateExit()
{
	m_duration = 0;

	//TODO::회전값 0으로 초기화 이거 좀더 정교한 처리 필요 - 지금보는 방향으로 해줘야하는뎅?
	//m_fsm->GetOwner().lock()->GetComponent<Transform>()->SetLocalRotation(Vector3(0, -180, 0));
}

void GuardSearch::Update(double dt)
{
	m_duration += dt;

	double resetTime = 3;

	if (m_duration > resetTime)
	{
		m_duration = 0;

		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::patrol);
		//dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::comeback);
		return;
	}

	if (m_duration < resetTime / 2)
	{
		m_fsm->GetOwner().lock()->GetComponent<Transform>()->AddYAxisRotation(-0.25 * dt);
	}
	else
	{
		m_fsm->GetOwner().lock()->GetComponent<Transform>()->AddYAxisRotation(0.25 * dt);
	}

	return;
}

void GuardSearch::Finalize()
{

}

#pragma endregion

#pragma region GuardComeBack
GuardComeBack::GuardComeBack(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardComeBack::~GuardComeBack()
{

}

void GuardComeBack::OnStateEnter()
{
	//m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolRun");


	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		//prePath = m_path.top();
		curPath = m_path.top();
	}
}

void GuardComeBack::OnStateExit()
{
}

void GuardComeBack::Update(double dt)
{
	//update마다 내 위치 매번 갱신 필요
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);

	}
	//경로에 도착했다
	else
	{
		//test 상태 변경
		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::patrol);
		//return;
	}

}

void GuardComeBack::Finalize()
{

}

#pragma endregion

#pragma region GuardFishinged
GuardFishinged::GuardFishinged(std::shared_ptr<FSM> fsm) : GuardState(fsm), isFishinged(false)
{
}

GuardFishinged::~GuardFishinged()
{

}

void GuardFishinged::OnStateEnter()
{
	//m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolMiss");

	isFishinged = true;
}

void GuardFishinged::OnStateExit()
{
	isFishinged = false;
}

void GuardFishinged::Update(double dt)
{
	static double time = 0;

	if (isFishinged)
	{

		time += dt;

		//상태이상 시간 3초
		if (time > 3)
		{
			isFishinged = false;
		}
	}
	else
	{
		time = 0;
		dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::search);
	}
}

void GuardFishinged::Finalize()
{

}

#pragma endregion

#pragma region GuardSet


GuardSet::GuardSet(std::shared_ptr<FSM> fsm) : GuardState(fsm)
{

}

GuardSet::~GuardSet()
{

}

void GuardSet::OnStateEnter()
{
	// 호루라기 <-개별
	SoundManager::GetInstance()->PlaySFX("Guard_Chase.wav");

	// 추격 개시 브금전환
	SoundManager::GetInstance()->CancelFadeoutPlayBGM("InGame_ChaseState.mp3");
	SoundManager::GetInstance()->FadeOutBGM("InGame_Default.wav", 0.5f);

	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PatrolSet");
}

void GuardSet::OnStateExit()
{

}

void GuardSet::Update(double dt)
{
	m_duration += dt;
	double resetTime = 0.5;

	auto modelkey = m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey();
	if (m_fsm->GetOwner().lock()->GetGraphics().lock()->IsAnimationEnd(modelkey))
	{
		if (m_duration > resetTime)
		{
			m_duration = 0;
			dynamic_pointer_cast<GuardFSM>(m_fsm)->ChangeState(GuardFSM::Guard::chasing);
		}
	}

	return;
}

void GuardSet::Finalize()
{

}

#pragma endregion

#pragma endregion

#pragma region Shopkeeper

#pragma region ShopkeeperState
ShopKeeperState::ShopKeeperState(std::shared_ptr<FSM> fsm) : State(fsm)
{

}

ShopKeeperState::~ShopKeeperState()
{

}

//double ShopKeeperState::m_sense = 0;

#pragma endregion

#pragma region ShopkeeperIdle
ShopKeeperIdle::ShopKeeperIdle(std::shared_ptr<FSM> fsm) : ShopKeeperState(fsm)
{

}

ShopKeeperIdle::~ShopKeeperIdle()
{

}

void ShopKeeperIdle::OnStateEnter()
{
	m_time = 0;

	//여기 터지면 그래픽스가 없거나 메쉬필터가 없는건데 
	// 상인 생성하면서 fsm start를 addentity 또는 메쉬필터 넣는거보다 보다 먼저하면 안댐
	//상태변화 없으면 계속 반복
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"MerchantIdle");

}

void ShopKeeperIdle::OnStateExit()
{
	m_time = 0;
}

void ShopKeeperIdle::Update(double dt)
{
	//어그로 풀리면 일정 시간 유지후 눈치게이지 하락

	//test용 update 3번하고 나서 sense down 출력
	m_time += dt;

	if (m_time > m_duration)
	{
		if ((*m_sense) > 0)
		{
			(*m_sense) -= dt * 100;
		}
		else
		{
			(*m_sense) = 0;
		}
	}
}

void ShopKeeperIdle::Finalize()
{

}

#pragma endregion

#pragma region ShopkeeperDoubt

ShopKeeperDoubt::ShopKeeperDoubt(std::shared_ptr<FSM> fsm) : ShopKeeperState(fsm)
{

}

ShopKeeperDoubt::~ShopKeeperDoubt()
{

}

void ShopKeeperDoubt::OnStateEnter()
{
}

void ShopKeeperDoubt::OnStateExit()
{
}

void ShopKeeperDoubt::Update(double dt)
{
	//m_sense += dt * addCoefficient;

	(*m_sense) += 20 * dt;

	//특정 수치 이상의 눈치게이지가 차면 애니메이션을 바꿔줘야한다
	if ((*m_sense) > m_maxSense)
	{
		(*m_sense) = m_maxSense;
		dynamic_pointer_cast<ShopKeeperFSM>(m_fsm)->ChangeState(ShopKeeperFSM::ShopKeeper::calling);
		return;
	}
}

void ShopKeeperDoubt::Finalize()
{

}

#pragma endregion

#pragma region ShopkeeperCalling
ShopKeeperCalling::ShopKeeperCalling(std::shared_ptr<FSM> fsm) : ShopKeeperState(fsm)
{

}

ShopKeeperCalling::~ShopKeeperCalling()
{

}

void ShopKeeperCalling::OnStateEnter()
{
	for (auto child : m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetChildren())
	{
		if (child->GetOwner().lock()->GetComponent<ShopKeeperCallingScript>() != nullptr)
		{
			child->GetOwner().lock()->GetComponent<SphereCollider>()->SetIsTrigger(true);
		}
	}

	//상태변화 없으면 계속 반복
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"MerchantShouting");

}

void ShopKeeperCalling::OnStateExit()
{
}

void ShopKeeperCalling::Update(double dt)
{
	//경비원 호출
	//일정 시간후 상태 변경
	static double time = 0;

	time += dt * 10;

	double offCall = 1;

	if (offCall < time)
	{
		dynamic_pointer_cast<ShopKeeperFSM>(m_fsm)->ChangeState(ShopKeeperFSM::ShopKeeper::idle);
		time = 0;
		return;
	}

}

void ShopKeeperCalling::Finalize()
{

}
#pragma endregion

#pragma endregion

#pragma region Player

#pragma region PlayerState

PlayerState::PlayerState(std::shared_ptr<FSM> fsm) : State(fsm)
{
}

PlayerState::~PlayerState()
{
}

void PlayerState::ReturnIdle()
{
	if (InputManager::GetInstance()->GetKeyState(VK_LEFT) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_RIGHT) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_DOWN) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_UP) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEYSTATE::None)
	{
		return m_fsm->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
	}
}

#pragma  endregion

#pragma region PlayerIdle

PlayerIdle::PlayerIdle(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayerIdle::~PlayerIdle()
{

}

void PlayerIdle::OnStateEnter()
{
	//상태 변화 없으면 반복재생
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerIdle");
}

void PlayerIdle::OnStateExit()
{
}

void PlayerIdle::Update(double dt)
{
	if (InputManager::GetInstance()->IsKeyPress('A')
		|| InputManager::GetInstance()->IsKeyPress('D')
		|| InputManager::GetInstance()->IsKeyPress('W')
		|| InputManager::GetInstance()->IsKeyPress('S'))
	{
		//m_fsm->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_LBUTTON)
		|| InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
	{
		//m_fsm->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Lifting);
	}

}

void PlayerIdle::Finalize()
{

}
#pragma  endregion

#pragma region PlayerEat
PlayerEat::PlayerEat(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayerEat::~PlayerEat()
{

}

void PlayerEat::OnStateEnter()
{
	//상태 변화 없으면 반복재생
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerEat");
}

void PlayerEat::OnStateExit()
{
}

void PlayerEat::Update(double dt)
{
}

void PlayerEat::Finalize()
{

}
#pragma  endregion

#pragma region PlayerHide
PlayerHide::PlayerHide(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayerHide::~PlayerHide()
{

}

void PlayerHide::OnStateEnter()
{

}

void PlayerHide::OnStateExit()
{
}

void PlayerHide::Update(double dt)
{
	if (InputManager::GetInstance()->GetKeyState(VK_LEFT) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_RIGHT) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_DOWN) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_UP) == KEYSTATE::None
		&& InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEYSTATE::None)
	{
		m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerIdle");
	}
}

void PlayerHide::Finalize()
{

}
#pragma  endregion

#pragma region PlayerRun

PlayeRun::PlayeRun(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayeRun::~PlayeRun()
{

}

void PlayeRun::OnStateEnter()
{
	//상태 변화 없으면 반복재생
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerRun");
}

void PlayeRun::OnStateExit()
{
}

void PlayeRun::Update(double dt)
{
	PlayerState::ReturnIdle();
}

void PlayeRun::Finalize()
{

}

#pragma  endregion

#pragma region PlayerThrow
PlayerThrow::PlayerThrow(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayerThrow::~PlayerThrow()
{

}

void PlayerThrow::OnStateEnter()
{
	//1번 재생
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerThrow");
}

void PlayerThrow::OnStateExit()
{
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerIdle");

}

void PlayerThrow::Update(double dt)
{
	auto modelKey = m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey();
	if (m_fsm->GetOwner().lock()->GetGraphics().lock()->IsAnimationEnd(modelKey))
	{
		m_fsm->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
	}
}

void PlayerThrow::Finalize()
{

}

#pragma  endregion

#pragma region PlayerLifting
PlayerLifting::PlayerLifting(std::shared_ptr<FSM> fsm) : PlayerState(fsm)
{

}

PlayerLifting::~PlayerLifting()
{

}

void PlayerLifting::OnStateEnter()
{
	//1번 재생

	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerLifting");

}

void PlayerLifting::OnStateExit()
{

}

void PlayerLifting::Update(double dt)
{
	//조준 취소
	if (InputManager::GetInstance()->IsKeyDown(VK_RBUTTON))
	{
		m_fsm->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
	}
}

void PlayerLifting::Finalize()
{

}
#pragma  endregion

#pragma  endregion

