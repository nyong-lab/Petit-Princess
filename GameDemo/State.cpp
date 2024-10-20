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
	//���� ��ΰ� �غ�Ǿ������� ������� ����
	curPath = m_path.top();

	//���� ���͸� �������ش�
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

	///�ش� ��ġ ��ó�� �����ϸ� ��ǥ ��θ� ���� �̰� ������ �ʹ� Ŀ��!!! �������� -> �߰��� pos���� �ٲ�� ���� �ȵ�����
	//TODO:: �̰� �ذ��ؾ��Ѵ� - ��� �ε�ġ�� ��δ�� ���� ���Ϳ��� ���� ����ġ���� ��η� �ٽ� Ž���ؾ��Ұ�
	if (abs(rangeX) < 0.2 && abs(rangeY) < 0.2)
	{

		//��ó�� �����ϱ� ��ġ ����
		m_fsm->GetOwner().lock()->GetComponent<Transform>()->SetWorldPosition(Vector3(curPath.pos.second, 1.5f, curPath.pos.first));
		//�ش� ��θ� ���������� ��� ��Ͽ��� ����
		prePath = m_path.top();
		m_path.pop();
	}

	//�״�� transform ���Ѵ�
	Vector3 direction(m_dirX, 0, m_dirY);
	//���⺤�� �븻������
	direction.Normalize(direction);

	m_fsm->GetOwner().lock()->GetComponent<Transform>()->AddPosition(direction * dt * m_moveSpeed);
	m_targetVector += direction;


	m_targetVector.Normalize();

	// �ε巴�� ���� ��ȯ�ϱ�
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
	//update���� �� ��ġ �Ź� ���� �ʿ�
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);
	}
	//��ο� �����ߴ�
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

	// �߰� �� �����ȯ
	SoundManager::GetInstance()->FadeOutBGM("InGame_ChaseState.mp3", 1.f);
	SoundManager::GetInstance()->ResumeFadeInBGM("InGame_Default.wav", 0.5f, 1.f);
}

void GuardChasing::Update(double dt)
{
	static double time = 0;
	time += dt;

	//update���� �� ��ġ �Ź� ���� �ʿ�
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();


	if (!m_path.empty())
	{
		Move(dt);
	}
	//��ο� �����ߴ�
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
	//����� ��η� �����̱⸸ �ϰ� �ϰ�
	//fsm���� ��θ� ���� ��ο� ���� ���͸� �Ѱܼ� �����̰� �Ѵ�?

	//update���� �� ��ġ �Ź� ���� �ʿ�
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);
	}
	//��ο� �����ߴ�
	else
	{
		//test ���� ����
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

	//TODO::ȸ���� 0���� �ʱ�ȭ �̰� ���� ������ ó�� �ʿ� - ���ݺ��� �������� ������ϴµ�?
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
	//update���� �� ��ġ �Ź� ���� �ʿ�
	m_curPoint = m_fsm->GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

	if (!m_path.empty())
	{
		Move(dt);

	}
	//��ο� �����ߴ�
	else
	{
		//test ���� ����
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

		//�����̻� �ð� 3��
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
	// ȣ���� <-����
	SoundManager::GetInstance()->PlaySFX("Guard_Chase.wav");

	// �߰� ���� �����ȯ
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

	//���� ������ �׷��Ƚ��� ���ų� �޽����Ͱ� ���°ǵ� 
	// ���� �����ϸ鼭 fsm start�� addentity �Ǵ� �޽����� �ִ°ź��� ���� �����ϸ� �ȴ�
	//���º�ȭ ������ ��� �ݺ�
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"MerchantIdle");

}

void ShopKeeperIdle::OnStateExit()
{
	m_time = 0;
}

void ShopKeeperIdle::Update(double dt)
{
	//��׷� Ǯ���� ���� �ð� ������ ��ġ������ �϶�

	//test�� update 3���ϰ� ���� sense down ���
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

	//Ư�� ��ġ �̻��� ��ġ�������� ���� �ִϸ��̼��� �ٲ�����Ѵ�
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

	//���º�ȭ ������ ��� �ݺ�
	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"MerchantShouting");

}

void ShopKeeperCalling::OnStateExit()
{
}

void ShopKeeperCalling::Update(double dt)
{
	//���� ȣ��
	//���� �ð��� ���� ����
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
	//���� ��ȭ ������ �ݺ����
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
	//���� ��ȭ ������ �ݺ����
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
	//���� ��ȭ ������ �ݺ����
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
	//1�� ���
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
	//1�� ���

	m_fsm->GetOwner().lock()->GetGraphics().lock()->AnimationControl(m_fsm->GetOwner().lock()->GetComponent<MeshFilter>()->GetModelKey(), L"PlayerLifting");

}

void PlayerLifting::OnStateExit()
{

}

void PlayerLifting::Update(double dt)
{
	//���� ���
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

