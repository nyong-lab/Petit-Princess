#pragma once


/// <summary>
/// Handler Ŭ�������� �ֻ�� �θ� Ŭ����
/// Property Ŭ�������� ��� ������ m_handler�� �����ϱ� ���� Ÿ������
/// IPropertyHandler Ŭ������ ����Ϸ� ������ ���ø� Ŭ�������⿡ 
/// IPropertyHandler ���� typename T�� �߷��ϱ� ���� ����� �� �𸣰ڴ�.
/// �׷��⿡ ���ø� Ŭ������ �ƴ� �Ϲ����� Ŭ���� PropertyHandlerBase Ŭ������ �������
/// �̸� IPropertyHandler Ŭ������ ��ӹްԲ� ���� �ʿ��� �� �ٿ�ĳ������ �Ͽ�
/// ������ ������ �ϴ� ����� �����ߴ�.
/// �ۼ��� : �赿��
/// </summary>
class PropertyHandlerBase
{
	GENERATE_CLASS_TYPE_INFO(PropertyHandlerBase);

public:
	virtual ~PropertyHandlerBase() = default;
};

/// <summary>
/// Handler Ŭ������ �������̽�
/// �� �������̽��� ����Ͽ� Property Ŭ������ ��� �ִ�
/// PropertyHandlerBase Ŭ������ �ٿ�ĳ�����ؼ� ������ ������ �ϰ� �����.
/// �ۼ��� : �赿��
/// </summary>
template <typename T>
class IPropertyHandler : public PropertyHandlerBase
{
	GENERATE_CLASS_TYPE_INFO(IPropertyHandler)

public:
	virtual ~IPropertyHandler() = default;

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����ؼ� ���� �����´�.
	virtual T& Get(void* object) const abstract;

	// �ν��Ͻ�ȭ�� ��ü�� �����͸� �޾Ƽ�
	// �ش� ��ü�� ��� ������ �����Ͽ� ���ϴ� ������ �����Ѵ�.
	virtual void Set(void* object, const T& value) const abstract;
};