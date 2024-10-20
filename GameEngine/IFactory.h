#pragma once

#include "IBuilder.h"
#include <string>
#include <memory>

/// <summary>
/// ������ �Ű������� �޾� ��ü�� ������� ���丮�� �߻�Ŭ����
/// </summary>
/// <typeparam name="T">���� ��ü�� Ÿ��</typeparam>
/// <typeparam name="U">������ Ÿ��</typeparam>
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
