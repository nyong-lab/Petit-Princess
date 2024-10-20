#pragma once

#include "IPropertyHandler.h"

/// <summary>
/// �������� ������ ��� ������ �����ϴ� Ŭ����
/// ������ ���������� ������ �ְų� �����´�.
/// �ۼ��� : �赿��
/// </summary>
/// <typeparam name="TClass">�����ϰ� �ִ� ��ü�� Ÿ��</typeparam>
/// <typeparam name="T">���� Ÿ��</typeparam>
template<typename TClass, typename T>
class StaticPropertyHandler : public IPropertyHandler<T>
{
	GENERATE_CLASS_TYPE_INFO(StaticPropertyHandler)

public:
	// ������ �Ҹ���
	StaticPropertyHandler() {}; //  m_defaultConstructor�� �ʱ�ȭ ��Ű�� ���ؼ� �־���.
;	explicit StaticPropertyHandler(T* ptr) : m_ptr(ptr) {}
	virtual ~StaticPropertyHandler() = default;

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����ؼ� ���� �����´�.
	virtual T& Get(void* object) const override
	{
		return *m_ptr;
	}

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����Ͽ� ���ϴ� ������ �����Ѵ�.
	virtual void Set(void* object, const T& value) const override
	{
		*m_ptr = value;
	}

private:
	// ������ ��� ������ �ּҸ� ����ִ� ����
	T* m_ptr = nullptr;
};

