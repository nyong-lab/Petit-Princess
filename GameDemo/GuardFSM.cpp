
#include "GameDemo_pch.h"
#include "GuardFSM.h"


#include "Entity.h"
#include "Transform.h"

#include <set>
#include <vector>

#include "InputManager.h"	
#include "ResourceManager.h"


void GuardFSM::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_patrolPath"] = nlohmann::json::array();
	for (const auto& path : m_patrolPath)
	{
		json["m_patrolPath"].push_back
		(
			{
				{"x", path.x},
				{"y", path.y},
				{"z", path.z}
			}
		);
	}

	json["m_combackPoint"] =
	{
		{"x", m_combackPoint.x},
		{"y", m_combackPoint.y},
		{"z", m_combackPoint.z}
	};
}

void GuardFSM::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_patrolPath") && json["m_patrolPath"].is_array())
		{
			m_patrolPath.clear();
			for (const auto& pathJson : json["m_patrolPath"])
			{
				Vector3 point;
				point.x = pathJson["x"];
				point.y = pathJson["y"];
				point.z = pathJson["z"];
				m_patrolPath.push_back(point);
			}
		}

		if (json.contains("m_combackPoint"))
		{
			m_combackPoint.x = json["m_combackPoint"]["x"];
			m_combackPoint.y = json["m_combackPoint"]["y"];
			m_combackPoint.z = json["m_combackPoint"]["z"];
		}

	}
}

GuardFSM::GuardFSM() : FSM(), cheat(false)
{
}

GuardFSM::~GuardFSM()
{
	//여기 잘못 지우는듯?
	const int size = 151;

	if (map)
	{
		delete map;
	}

	if (visitMap)
	{
		delete visitMap;
	}
}

void GuardFSM::Start()
{
	const int size = 151;

	if (m_owner.lock())
	{
		grid = m_owner.lock()->GetResourceManager().lock()->GetMapInfo();

		//일단 test용 grid map
		//grid = new int* [size];

		/*for (int i = 0; i < size; i++)
		{
			grid[i] = new int[size];

			for (int j = 0; j < size; j++)
			{
				grid[i][j] = 0;
			}
		}*/

		visitMap = new bool* [size];
		//내 경로를 저장
		for (int i = 0; i < size; i++)
		{
			visitMap[i] = new bool[size];
			for (int j = 0; j < size; j++)
			{
				visitMap[i][j] = false;
			}
		}

		map = new AstarNode * [size];

		//노드의 정보를 담을 세부 맵을 초기화했다
		for (int i = 0; i < size; ++i)
		{
			map[i] = new AstarNode[size];
		}


		//test destination
		m_destination = Vector3(5.0f, 0, 0.f);
		m_combackPoint = GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
		m_curPoint = GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();

		//이거 갖거 있는거 맞냐?
		m_guardStateList[Guard::idle] = std::make_shared<GuardIdle>(shared_from_this());
		m_guardStateList[Guard::patrol] = std::make_shared<GuardPatrol>(shared_from_this());
		m_guardStateList[Guard::chasing] = std::make_shared<GuardChasing>(shared_from_this());
		m_guardStateList[Guard::called] = std::make_shared<GuardCalled>(shared_from_this());
		m_guardStateList[Guard::search] = std::make_shared<GuardSearch>(shared_from_this());
		m_guardStateList[Guard::fishinged] = std::make_shared<GuardFishinged>(shared_from_this());
		m_guardStateList[Guard::comeback] = std::make_shared<GuardComeBack>(shared_from_this());
		m_guardStateList[Guard::set] = std::make_shared<GuardSet>(shared_from_this());

		// 에디터에서 조작 할 경비병 스피드
		m_guardSpeed = static_pointer_cast<GuardState>(m_guardStateList[Guard::idle])->GetMoveSpeed();

		//이거 맞냐?
		std::vector<bool> idleTransition = { true,true, true, true, true, true, true,true };
		m_transition.push_back(idleTransition);
		std::vector<bool> patrolTransition = { true,true, true, true, true, true, false, true };
		m_transition.push_back(patrolTransition);
		std::vector<bool> chasingTransition = { true,false, true, false, true, true, false, false };
		m_transition.push_back(chasingTransition);
		std::vector<bool> calledTransition = { true,false, true, true, true, false, false,true };
		m_transition.push_back(calledTransition);
		std::vector<bool> searchTransition = { true,true, true, true, true, true, true,true };
		m_transition.push_back(searchTransition);
		std::vector<bool> fishingedTransition = { true,false, false, false, true, false, false,false };
		m_transition.push_back(fishingedTransition);
		std::vector<bool> comebackTransition = { true,true, true, true, true, true, true,true };
		m_transition.push_back(comebackTransition);
		std::vector<bool> setTransition = { false,false, true, false, false, false, false,true };
		m_transition.push_back(setTransition);

		m_curState = nullptr;
		m_preState = Guard::idle;
		ChangeState(Guard::patrol);
	}
}

void GuardFSM::OnStateEnter()
{
	m_curState->OnStateEnter();
}

void GuardFSM::OnStateExit()
{
	m_curState->OnStateExit();
}

void GuardFSM::Update(double dt)
{
	//숫자 1 키  넘패드 말고
	if (InputManager::GetInstance()->IsKeyDown(0x31))
	{
		cheat = !cheat;
		FSM::ChangeState(m_guardStateList[Guard::idle]);

	}

	m_curPoint = GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition();
	m_curState->Update(dt);
}

void GuardFSM::Render(double dt)
{

}

void GuardFSM::Finalize()
{
	for (auto state : m_guardStateList)
	{
		state.second->Finalize();
		state.second.reset();
	}

	m_curState.reset();
	m_guardStateList.clear();
}

void GuardFSM::ChangeState(Guard state)
{
#ifdef _DEBUG
	/*AllocConsole();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD dwByte(0);
	const char* teststr = "Guard : \t";
	WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);

	std::string x = std::to_string(m_curPoint.x);
	std::string z = std::to_string(m_curPoint.z);
	std::string a = "cur pos(" + x + ", " + z + ")\t";

	const char* ta = a.c_str();
	WriteFile(hConsole, ta, strlen(ta), &dwByte, nullptr);

	x = std::to_string(m_destination.x);
	z = std::to_string(m_destination.z);
	a = "destination pos(" + x + ", " + z + ")";
	ta = a.c_str();
	WriteFile(hConsole, ta, strlen(ta), &dwByte, nullptr);*/

#endif // DEBUG


	if (cheat)
	{
		state = Guard::idle;
		m_preState = Guard::idle;
	}

	if (m_transition[static_cast<int>(m_preState)][static_cast<int>(state)])
	{

		m_currentState = state;

		switch (state)
		{

		case Guard::idle:
			m_preState = state;
			FSM::ChangeState(m_guardStateList[Guard::idle]);

			break;
		case Guard::called:
			//if (preState != Guard::chasing)
		{
			m_preState = state;
			Astar(m_destination);
			dynamic_pointer_cast<GuardCalled>(m_guardStateList[Guard::called])->SetPath(m_path);
			FSM::ChangeState(m_guardStateList[Guard::called]);
		}
		break;
		case Guard::chasing:
		{
			m_preState = state;
			Astar(m_destination);
			dynamic_pointer_cast<GuardState>(m_guardStateList[Guard::chasing])->SetPath(m_path);
		}
		FSM::ChangeState(m_guardStateList[Guard::chasing]);
		break;
		case Guard::comeback:
			m_preState = state;
			Astar(m_combackPoint);
			dynamic_pointer_cast<GuardState>(m_guardStateList[Guard::comeback])->SetPath(m_path);
			FSM::ChangeState(m_guardStateList[Guard::comeback]);
			break;
		case Guard::search:
			m_preState = state;
			FSM::ChangeState(m_guardStateList[Guard::search]);
			break;
		case Guard::fishinged:
			m_preState = state;
			FSM::ChangeState(m_guardStateList[Guard::fishinged]);
			break;
		case Guard::patrol:
		{
			static int count = 0;
			m_preState = state;

			// fsm 추가하면 경로가 0인 상태에서 배열에 접근하여 터지므로 일단 예외처리
			if (!m_patrolPath.empty())
			{
				Astar(m_patrolPath[count]);
				count++;

				if (count >= m_patrolPath.size())
				{
					count = count % m_patrolPath.size();
				}
			}

			dynamic_pointer_cast<GuardPatrol>(m_guardStateList[Guard::patrol])->SetPath(m_path);
			FSM::ChangeState(m_guardStateList[Guard::patrol]);
		}
		break;
		case Guard::set:
			m_preState = state;
			FSM::ChangeState(m_guardStateList[Guard::set]);
			break;

		}
	}
}

std::shared_ptr<Component> GuardFSM::Clone() const
{
	std::shared_ptr<GuardFSM> clone = std::make_shared<GuardFSM>();

	// 시리얼라이즈 json 에서 터지므로 주석 
	// 직접 깊은 복사 하자
	//clone->Start(); 

	clone->m_curPoint = m_curPoint;
	clone->m_destination = m_destination;
	clone->m_combackPoint = m_combackPoint;
	clone->m_patrolPath = m_patrolPath;
	clone->m_path = m_path;

	const int size = 151;

	if (grid)
	{
		clone->grid = m_owner.lock()->GetResourceManager().lock()->GetMapInfo();
	}

	if (visitMap)
	{
		clone->visitMap = new bool* [size];
		for (int i = 0; i < size; ++i)
		{
			clone->visitMap[i] = new bool[size];
			for (int j = 0; j < size; ++j)
			{
				clone->visitMap[i][j] = visitMap[i][j];
			}
		}
	}

	if (map)
	{
		clone->map = new AstarNode * [size];

		//노드의 정보를 담을 세부 맵을 초기화했다
		for (int i = 0; i < size; ++i)
		{
			clone->map[i] = new AstarNode[size];
		}
	}

	return clone;
}

void GuardFSM::SetDestination(Vector3 destination)
{
	m_destination = destination;
}

void GuardFSM::SetPatrolPath(Vector3 destination)
{
	m_patrolPath.push_back(destination);
}

void GuardFSM::Astar(Vector3 goal)
{
	Point startPoint = CalcPoint(m_curPoint);
	Point destination = CalcPoint(goal);

	//맵 초기화
	int size = 151;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			visitMap[i][j] = false;
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			map[i][j].F = DBL_MAX;
			map[i][j].G = DBL_MAX;
			map[i][j].H = DBL_MAX;

			map[i][j].isDestinaition = false;
			map[i][j].isStart = false;
			map[i][j].pos = Point(-1, -1);
			map[i][j].preNode = Point(-1, -1);
		}
	}

	//
	int temp = startPoint.first;
	startPoint.first = startPoint.second;
	startPoint.second = temp;

	temp = destination.first;
	destination.first = destination.second;
	destination.second = temp;

	//시작점이 유효한가
	if (!IsValid(startPoint))
	{
		/*AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwByte(0);
		const char* teststr = "Start Point is InValid\n";
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);*/
		return;
	}

	//목적지가 유효한가
	if (!IsValid(destination))
	{
		/*AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwByte(0);
		const char* teststr = "Destination is InValid\n";
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);*/
		return;
	}

	//시작점이 벽인가?
	if (grid[static_cast<int>(m_curPoint.x)][static_cast<int>(m_curPoint.z)] == 1)
	{
		/*AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwByte(0);
		const char* teststr = "Start is Wall\n";
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);*/
		return;
	}

	//목적지가 벽인가?
	if (grid[static_cast<int>(destination.first)][static_cast<int>(destination.second)] == 1)
	{
		/*AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD dwByte(0);
		const char* teststr = "Destination is Wall\n";
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);*/
		return;
	}

	//시작점과 목적지가 이미 동일한가?
	if (startPoint == destination)
	{
		return;
	}

	//시작점 세팅
	map[startPoint.first][startPoint.second].pos = startPoint;
	map[startPoint.first][startPoint.second].isStart = true;
	map[startPoint.first][startPoint.second].G = 0;
	map[startPoint.first][startPoint.second].preNode = startPoint;

	visitMap[startPoint.first][startPoint.second] = true;

	//목적지 세팅
	map[destination.first][destination.second].pos = destination;
	map[destination.first][destination.second].isDestinaition = true;

	AstarNode startNode = map[startPoint.first][startPoint.second];
	AstarNode destinationNode = map[destination.first][destination.second];

	//방향
	int dirX[] = { -1,0,1 };
	int dirY[] = { -1,0,1 };

	std::set<AstarNode> calcPath; //탐색을 하면서 저장해놨던 경로들 탐색중 경로가 막히면 다음 경로를 탐색하기위해 저장한다
	calcPath.clear();
	calcPath.insert(map[startPoint.first][startPoint.second]);


	AstarNode CurNode = startNode;
	AstarNode PreNode = startNode;

	bool findDestination = false;


	std::vector<Point> answer;


	while (!calcPath.empty())
	{
		//이전 위치에서 현재위치가 비용이 가장 저렴
		AstarNode curPos = *calcPath.begin();

		//해당 위치를 방문
		visitMap[curPos.pos.first][curPos.pos.second] = true;

		//이 경로를 탐색해 보겠다 그래서 해당 경로를 지정했기때문에 다음 탐색시 탐색 목록에서 없어야한다
		calcPath.erase(calcPath.begin());

		//벽이 아닌 현재 위치에서 진행 할 수 있는 노드의 F 를 구해 진행할 노드를 찾자
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Point calcPos = Point(curPos.pos.first + dirY[i], curPos.pos.second + dirX[j]);
				//노드의 진행방향이 유효한 범위인가?
				if (IsValid(calcPos))
				{
					//진행방향에 목적지가 존재하는가? 
					//대각선으로도 접근할 수도 있고 직진으로도 접근이 되는데 지금 set에 처음 목적지를 찾아버리면 뒤에꺼 계산 안함
					if (calcPos == destination)
					{
#ifdef _DEBUG			
						/*AllocConsole();
						HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


						std::string test1 = std::to_string(destination.first);
						std::string test2 = std::to_string(destination.second);
						std::string test = "\n Find Destination! (" + test1 + ", " + test2 + ")\n";

						DWORD dwByte(0);
						const char* teststr = test.c_str();
						WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);*/
#endif
						findDestination = true;

						//경로 초기화
						if (!m_path.empty())
						{
							while (!m_path.empty())
							{
								m_path.pop();
							}
						}

						map[destination.first][destination.second].preNode = curPos.pos;
						destination = calcPos;

						//최종 경로 저장
						do
						{
							m_path.push(map[destination.first][destination.second]);
							destination = map[destination.first][destination.second].preNode;

						} while (!m_path.top().isStart);

						///path trace
						//PathTrace(m_path);

						return;
					}

					//벽이 아니고 지나간적 없는 노드인가?
					if (visitMap[calcPos.first][calcPos.second] == false && grid[calcPos.first][calcPos.second] != 1)
					{
						double g = 0;
						double h = 0;
						double f = 0;

						if (i % 2 == 0 && j % 2 == 0)
						{
							//대각선 움직일때
							g = map[CurNode.pos.first][CurNode.pos.second].G + 1.414;
							h = CalcHuristic(calcPos, destination);
							f = h + g;
						}
						else
						{
							//이건 직진으로 움직일때
							g = map[CurNode.pos.first][CurNode.pos.second].G + 1;
							h = CalcHuristic(calcPos, destination);
							f = h + g;
						}

						//계산한 경로가 한번도 계산된 적이 없는가?
						//이미 계산되어 있다면 기존의 f 값보다 작은가?
						//여기서 g 값이 제대로 갱신이 안되고 있는거같다 g 값이 안올라가
						if (map[calcPos.first][calcPos.second].F == DBL_MAX
							|| map[calcPos.first][calcPos.second].F > f)
						{
							//비용계산
							map[calcPos.first][calcPos.second].pos = calcPos;
							map[calcPos.first][calcPos.second].G = g; //여기서 부모노드의 g 값이 제대로 안더해짐 - 해결
							map[calcPos.first][calcPos.second].H = h;
							map[calcPos.first][calcPos.second].F = f;
							map[calcPos.first][calcPos.second].preNode = Point(curPos.pos.first, curPos.pos.second);

							//탐색하면서 기존에 있던 비용, 새로운 경로들을  갱신하겠다
							calcPath.insert(map[calcPos.first][calcPos.second]);
						}
					}
				}
			}
		}

		//이게 문제다 - G값이 계속 0으로 더하는 문제 어떻게 해야할까?
		//CurNode = map[curPos.first][curPos.second];
	}

	if (findDestination == false)
	{
		AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwByte(0);
		const char* teststr = "Destination Searching Failed \n";
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);

		return;
	}
}

bool GuardFSM::IsValid(Vector3 point)
{
	const int size = mapsize;

	return (0 <= point.x && point.x < size &&
		0 <= point.y && point.y < size &&
		0 <= point.z && point.z < size);
}

bool GuardFSM::IsValid(Point point)
{
	const int size = mapsize;

	return (0 <= point.first && point.first < size &&
		0 <= point.second && point.second < size);
}

double GuardFSM::CalcHuristic(Point pos, Point destination)
{
	//유클리드
	return  sqrt(pow((pos.first - destination.first), 2) + pow((pos.second - destination.second), 2));
}

void GuardFSM::PathTrace(std::stack<AstarNode> path)
{
	while (!path.empty())
	{

		AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::string test1 = std::to_string(path.top().pos.first);
		std::string test2 = std::to_string(path.top().pos.second);
		std::string test = "(" + test1 + ", " + test2 + ") -> ";


		DWORD dwByte(0);
		const char* teststr = test.c_str();
		WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);

		path.pop();
	}

	AllocConsole();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD dwByte(0);
	const char* teststr = "\n\n";
	WriteFile(hConsole, teststr, strlen(teststr), &dwByte, nullptr);

}

Point GuardFSM::CalcPoint(Vector3 point)
{
	if (point.x - static_cast<int>(point.x) > 0.5 && point.z - static_cast<int>(point.z) > 0.5)
	{
		return Point(static_cast<int>(point.x) + 1, static_cast<int>(point.z) + 1);
	}
	else if (m_curPoint.x - static_cast<int>(point.x) > 0.5 && point.z - static_cast<int>(point.z) < 0.5)
	{
		return Point(static_cast<int>(point.x + 1), static_cast<int>(point.z));
	}
	else if (point.x - static_cast<int>(point.x) < 0.5 && point.z - static_cast<int>(point.z) > 0.5)
	{
		return Point(static_cast<int>(point.x), static_cast<int>(point.z) + 1);
	}
	else
	{
		return Point(static_cast<int>(point.x), static_cast<int>(point.z));
	}
}

void GuardFSM::UpdateGuardSpeed()
{
	static_pointer_cast<GuardState>(m_guardStateList[Guard::idle])->SetMoveSpeed(m_guardSpeed);
}
