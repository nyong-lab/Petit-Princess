#pragma once

#include "IPropertyHandler.h"

/// <summary>
/// ��� ������ �����ϴ� Handler Ŭ����
/// ������ ���������� ������ �ְų� �����´�.
/// �ۼ��� : �赿��
/// </summary>
/// <typeparam name="TClass">�����ϰ� �ִ� ��ü�� Ÿ��</typeparam>
/// <typeparam name="T">���� Ÿ��</typeparam>
template<typename TClass, typename T>
class PropertyHandler : public IPropertyHandler<T>
{
	GENERATE_CLASS_TYPE_INFO(PropertyHandler)

	// TClass ���� T Ÿ�� ��� ���� ������
	// ���ø� ���ڰ� ������ Ÿ�ӿ� �����Ǿ�� �ϱ� ������
	// ��� ���� ������ Ÿ������ �ּҸ� �����Ѵ�.
	using MemberPtr = T TClass::*;

public:
	// ������ �Ҹ���
	PropertyHandler() {}; // m_defaultConstructor�� �ʱ�ȭ ��Ű�� ���ؼ� �־���.
	explicit PropertyHandler(MemberPtr ptr) : m_ptr(ptr) {}
	virtual ~PropertyHandler() = default;

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����ؼ� ���� �����´�.
	virtual T& Get(void* object) const override
	{
		return static_cast <TClass*>(object)->*m_ptr;
	}

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����Ͽ� ���ϴ� ������ �����Ѵ�.
	virtual void Set(void* object, const T& value) const override
	{
		static_cast<TClass*>(object)->*m_ptr = value;
	}

private:
	MemberPtr m_ptr = nullptr;
};