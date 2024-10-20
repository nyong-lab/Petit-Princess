#pragma once

#include "IBuilder.h"
#include <string>
#include <memory>

/// <summary>
/// 빌더를 매개변수로 받아 객체를 만들어줄 팩토리의 추상클래스
/// </summary>
/// <typeparam name="T">만들 객체의 타입</typeparam>
/// <typeparam name="U">빌더의 타입</typeparam>
template<typename T, typename U>
class IFactory
{
public:
	virtual ~IFactory() {}

	virtual std::shared_ptr<T> Create(std::string name, std::shared_ptr<IBuilder<U>> builder) abstract;
	virtual std::shared_ptr<T> Create(int count, std::string name, std::shared_ptr<IBuilder<U>> builder...) abstract;
	virtual std::shared_ptr<T> Create(int count, std::string name, IBuilder<U>&& builder...) abstract;

};


//template<typename T, typename U>
//T* IFactory<T, U>::Create(std::string name, IBuilder<U>* builder)
//{
//	return nullptr;
//}
//
//template<typename T, typename U>
//T* IFactory<T, U>::Create(int count, std::string name, IBuilder<U>* builder...)
//{
//	return nullptr;
//}
