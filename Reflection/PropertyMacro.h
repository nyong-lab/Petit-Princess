#pragma once


#include "Property.h"

/// ��� ���� ������ �����ϴ� ��ũ��
///
/// ���� ������ �� �������� ���ǵ� ��� ����ü�� �����ڿ� ����� 
/// PropertyRegister ��ü�� ���� �����ȴ�.
/// ���ø� Ŭ������ PropertyRegister�� �Ʒ��� ���� ������ ��´�.
/// 
/// 1. ThisType : �����ϰ� �ִ� ��ü�� Ÿ��
/// 2. decltype(Name) : ������ Ÿ��
/// 3. decltype(&ThisType::Name) : ���� ������ Ÿ��
/// 4. &ThisType::Name : ���� ������
/// 
/// 3���� 4������ ��� Ŭ����::Name(��� ���� ������ Ÿ��) ���� �����ϴµ�
/// ���ø� ���ڰ� ������ Ÿ�ӿ� �����Ǿ�� �ϱ� ������ 
/// �������� �ؼ��Ǵ� ��� ǥ�������� ��� ���� ������ Ÿ������ ����Ѵ�.
/// 
/// ���� &Name(������ Ÿ��) ���� �ϰ� �ȴٸ� 
/// ��Ÿ�� ������ �����Ǵ� �ּҸ� ����ϰ� �ǰ� ������ ����.
/// 
/// �߿��� ���� ��� ���� �����ͷ� Property�� �����ϰ� �ֱ⿡
/// ���Ŀ� ��� ������ �����ϴ� Ŭ������ PropertyHandler Ŭ�������� 
/// ��� ������ �ּҸ� ��� Ÿ�Ե� ���������� ��� ���� ������ Ÿ���̾�� �Ѵ�.
/// 
 #define PROPERTY(Name, ...) \
inline static struct RegisterPropertyExecutor_##Name \
	{ \
		RegisterPropertyExecutor_##Name() \
		{ \
			static PropertyRegister \
			< \
				ThisType \
				, decltype(Name) \
				, decltype(&ThisType::##Name) \
				, &ThisType::##Name \
				, ##__VA_ARGS__ \
			> property_register_##Name \
			{ #Name, ThisType::StaticTypeInfo()}; \
		} \
\
	} register_##Name; \

//#define PROPERTY(Name) \
//inline static struct RegisterPropertyExecutor_##Name \
//	{ \
//		RegisterPropertyExecutor_##Name() \
//		{ \
//			static PropertyRegister<ThisType, decltype(m_##Name), \
//			decltype(&ThisType::m_##Name), &ThisType::m_##Name> property_register_ ## Name \
//			{ "m_"#Name, ThisType::StaticTypeInfo()}; \
//		} \
//\
//	} register_##Name; \

//#define PROPERTY(Name) \
//inline static struct RegisterPropertyExecutor_##Name \
//{ \
//    RegisterPropertyExecutor_##Name() \
//    { \
//        static PropertyRegister<ThisType, decltype(&ThisType::m_##Name), \
//            decltype(&ThisType::m_##Name), &ThisType::m_##Name> property_register_##Name \
//            { #Name, ThisType::StaticTypeInfo()}; \
//    } \
//\
//} register_##Name;