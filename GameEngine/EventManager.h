#pragma once
#include "ISingleton.h"
#include <vector>
#include <set>
/// <summary>
/// 2024.01.24
/// ���� ���ҽ�, ������Ʈ���� ������ �ɶ� ������ ���� �ʵ��� ������ �Ŵ���
/// 2024.01.25
/// �̺�Ʈ �Ŵ���
/// ���� �浹�� �Ͼ�µ� �ٷ� �����ϸ� ������ ����
/// ��� ������ ������ �� ���ΰ�
/// ��ȣ�ۿ��� �ϴٰ� ���ڱ� ���� �Ǹ� ?
/// � ����� �߻��ϸ� �װ� �������ӿ� ��ΰ� ���� ������ �����ؾ��Ѵ�
/// ���� ������Ʈ�ϴ¾ִ� �� �׾��� �����ϰ� ���߿� ������Ʈ�ϴ� �ֵ��� �׾��� �����Ұ��ڳ�
/// �׷��ϱ� �̰� �� �����ϰ� ������ �Ǿ���Ѵ�
/// ����ó���ϰ� ���� �����ӿ� �˰� �� ���̴�
/// ������ ���� ������ �����ؾ��Ѵ�
/// ��򰡿� ������ �ֵ��� ��Ƶΰ� ������ ���� �� ���̴�
/// �������� ������ � ��Ȳ���� �˾ƾ��� ������ �ʿ�
/// �츮 ���ӿ� �ִ� ��Ȳ�� �ϴ� ���� ã��
/// entity ����
/// entity ����
/// Scene Change
/// Ư�� ������ �̺�Ʈ�� �Ź� �־�����ϴϱ� �ݹ��Լ��� ����� ���ƺ��̴µ�? 
/// -> �̰� ����غ��� �ϴ� �̱������� �ھƼ� mk1 ������
/// 
/// �ۼ� : ���¿�
/// 
/// 2024.02.05 23:59 
/// ������ : �赿�� -> ��ø������� �ϴ� ���� collider, fsm ���� ��� transform �� start �׸��� �Ҵ�� ���� �����ϰ� �ִ�.
///	������ ��ø�������� �߿� ���� �̺�Ʈ�ν� ü������ �Ǵµ�, �� �̺�Ʈ�ν� �� ü������ �ǰ� ������ start �Լ��� ȣ�� �ؾ��ϴ� ��Ȳ�̴�.
/// �ӽ÷� start �Լ��� ȣ��Ǿ�� �ϴ� component���� ���� ���͸� �޾Ƽ� �� ü������ �ǰ� �� �Ŀ� start �Լ� ȣ�� �����ְ� �������.
/// </summary>


class Scene;
class Entity;

//�̺�Ʈ�� ����
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

	//���� ���� ������ ����
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


