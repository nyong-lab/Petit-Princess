#pragma once
#include "IFactory.h"
#include "Entity.h"
#include "Component.h"

#include <stdio.h> //가변인자 사용
#include <stdarg.h> //가변인자 사용


/// <summary>
/// 2024.01.02
/// 엔티티를 만들 팩토리 클래스 컴포넌트 빌더를 받아 생성해줄것이다
/// 
/// 2024.01.18
/// create를 하면서 컴포넌트의 값을 바로 지정해주고 싶은걸? -> 이건 빌더에서 해주는게 맞지? 빌더가 컴포넌트를 만들자나
/// 
/// </summary>
class EntityFactory :
	public IFactory<Entity, Component>
{
public:
	EntityFactory();
	~EntityFactory();

	//복사 금지
	EntityFactory(const EntityFactory& factory) = delete;
	EntityFactory& operator= (const EntityFactory& factory) = delete;

	//이동금지
	EntityFactory(const EntityFactory&& factory) = delete;
	EntityFactory&& operator= (const EntityFactory&& factory) = delete;

	//TODO:자꾸 create 함수를 못찾는다고한다 이거 템플릿 종속성 문제를 확인해보자! - 해결 링크 문제였음
	virtual std::shared_ptr<Entity> Create(std::string name, std::shared_ptr<IBuilder<Component>> builder) override;
	virtual std::shared_ptr<Entity> Create(int count, std::string name, std::shared_ptr<IBuilder<Component>> builder...) override;
	virtual std::shared_ptr<Entity> Create(int count, std::string name, IBuilder<Component>&& builder...) override;

	//기존 컴포넌트에 여러개의 컴포넌트를 넣겠다면?
	void AddComponents(int count, std::shared_ptr < Entity> entity, std::shared_ptr<IBuilder<Component>> builder...);

private:
	std::shared_ptr<Entity> m_returnEntity;
};

