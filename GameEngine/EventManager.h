#pragma once
#include "ISingleton.h"
#include <vector>
#include <set>
/// <summary>
/// 2024.01.24
/// 각종 리소스, 오브젝트들이 삭제가 될때 문제가 되지 않도록 관리할 매니저
/// 2024.01.25
/// 이벤트 매니저
/// 뭔가 충돌이 일어났는데 바로 삭제하면 문제가 생김
/// 어느 시점에 삭제를 할 것인가
/// 상호작용을 하다가 갑자기 삭제 되면 ?
/// 어떤 사건이 발생하면 그게 한프레임에 모두가 같은 시점에 인지해야한다
/// 먼저 업데이트하는애는 안 죽었다 생각하고 나중에 업데이트하는 애들은 죽었다 생각할거자나
/// 그러니까 이게 다 동일하게 적용이 되어야한다
/// 지연처리하고 다음 프레임에 알게 할 것이다
/// 생성도 같은 원리로 생성해야한다
/// 어딘가에 삭제할 애들을 모아두고 삭제를 진행 할 것이다
/// 생성인지 삭제인 어떤 상황인지 알아야할 변수가 필요
/// 우리 게임에 있는 상황이 일단 뭔지 찾자
/// entity 생성
/// entity 삭제
/// Scene Change
/// 특점 시점에 이벤트를 매번 넣어줘야하니까 콜백함수로 만들면 좋아보이는데? 
/// -> 이건 고민해보고 일단 싱글턴으로 박아서 mk1 만들자
/// 
/// 작성 : 유승운
/// 
/// 2024.02.05 23:59 
/// 수정자 : 김동일 -> 디시리얼라이즈를 하는 도중 collider, fsm 같은 경우 transform 과 start 그리고 할당된 씬에 의존하고 있다.
///	하지만 디시리얼라이즈 중에 씬은 이벤트로써 체인지가 되는데, 즉 이벤트로써 씬 체인지가 되고 나서야 start 함수를 호출 해야하는 상황이다.
/// 임시로 start 함수가 호출되어야 하는 component들을 담은 벡터를 받아서 씬 체인지가 되고 난 후에 start 함수 호출 시켜주게 만들었다.
/// </summary>


class Scene;
class Entity;

//이벤트의 종류
enum class EventType
{
	WaitCreate,
	Create,
	WaitDestory,
	Destory,
	SceneChange,
	GameOver,

};


//CRTP
class EventManager : public ISingleton<EventManager>
{
	friend class ISingleton<EventManager>;
public:
	void Start();
	void Update();
	void AddEvent(std::shared_ptr <Entity> event, EventType type);
	void NextFrameStartComponents(std::vector<std::shared_ptr<Component>> components);
	void SceneChange(std::shared_ptr<Scene> scene);
		void SetCurScene(std::shared_ptr<Scene> scene) { m_curScene = scene; }
	std::shared_ptr <Scene> GetCurScene() { return m_curScene; }

	//상점 관련 포인터 연결
	void StoreAndAnyPointerConnection(std::shared_ptr<Scene> scene);

private:
	EventManager() {}

	std::shared_ptr<Scene> m_curScene;

	std::vector<std::shared_ptr <Entity>> m_createQueue;
	std::vector<std::shared_ptr <Entity>> m_destroyQueue;
	std::set<std::shared_ptr <Entity>> m_waitDestroyQueue;
	std::vector<std::shared_ptr <Scene>> m_SceneQueue;
	std::vector<std::shared_ptr <Component>> m_ComponentQueue;

	void AddEvent(std::shared_ptr<Scene> scene, std::shared_ptr <Entity> event, EventType type);
};


