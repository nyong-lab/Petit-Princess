#pragma once
#include "IFactory.h"
#include "Entity.h"
#include "Component.h"

#include <stdio.h> //�������� ���
#include <stdarg.h> //�������� ���


/// <summary>
/// 2024.01.02
/// ��ƼƼ�� ���� ���丮 Ŭ���� ������Ʈ ������ �޾� �������ٰ��̴�
/// 
/// 2024.01.18
/// create�� �ϸ鼭 ������Ʈ�� ���� �ٷ� �������ְ� ������? -> �̰� �������� ���ִ°� ����? ������ ������Ʈ�� �����ڳ�
/// 
/// </summary>
class EntityFactory :
	public IFactory<Entity, Component>
{
public:
	EntityFactory();
	~EntityFactory();

	//���� ����
	EntityFactory(const EntityFactory& factory) = delete;
	EntityFactory& operator= (const EntityFactory& factory) = delete;

	//�̵�����
	EntityFactory(const EntityFactory&& factory) = delete;
	EntityFactory&& operator= (const EntityFactory&& factory) = delete;

	//TODO:�ڲ� create �Լ��� ��ã�´ٰ��Ѵ� �̰� ���ø� ���Ӽ� ������ Ȯ���غ���! - �ذ� ��ũ ��������
	virtual std::shared_ptr<Entity> Create(std::string name, std::shared_ptr<IBuilder<Component>> builder) override;
	virtual std::shared_ptr<Entity> Create(int count, std::string name, std::shared_ptr<IBuilder<Component>> builder...) override;
	virtual std::shared_ptr<Entity> Create(int count, std::string name, IBuilder<Component>&& builder...) override;

	//���� ������Ʈ�� �������� ������Ʈ�� �ְڴٸ�?
	void AddComponents(int count, std::shared_ptr < Entity> entity, std::shared_ptr<IBuilder<Component>> builder...);

private:
	std::shared_ptr<Entity> m_returnEntity;
};

