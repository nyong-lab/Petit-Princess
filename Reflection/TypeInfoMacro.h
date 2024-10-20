#pragma once

#include "TypeInfo.h"


/// Ŭ���� Ÿ�� ������ �����ϴ� ��ũ��
///
/// Ŭ������ Ÿ�� ������ ��� �ִ� TypeInfo ��ü�� ����
/// �ڽ��� Ÿ�԰� �θ� Ÿ���� ��Ī�� �ϰ��� �̸����� ���� (Super, ThisType)
/// 
/// 1. Super��� �θ� Ŭ���� Ÿ���� ��Ī�� ThisType�̶�� �ڽ��� Ÿ�� ��Ī ����
/// ThisType�� ��ũ���� TypeName�� �״�� �̿��� �� ������, �θ� Ŭ������ Ÿ����
/// �θ� Ŭ������ ���� ���� ���� �߰����� ó���� �ʿ��ϴ�.
/// �θ� Ŭ���� Ÿ�� �߷��� ���� SuperClassTypeDeducation ���ø��� ����Ѵ�.
/// 
/// 2. Ŭ������ ���� TypeInfo ��ü�� ���� ��� ������ ����.
/// TypeInfo�� Ÿ���� ��� ���踦 ǥ���Ѱ� ���Ŀ� �ش� Ÿ���� ������ �ִ�
/// ��� �����鿡 ���� ������ ������ �ȴ�. (Property class)
/// Super Ÿ���߷��� ������ ó�� �θ� ���� ��쿡 ���� ó���� �ʿ��ϴ�.
/// �ش� ó���� TypeInfo �ʱ�ȭ������ ���Ǵ� TypeInitializer ����ü���� ó���Ѵ�.
#define GENERATE_CLASS_TYPE_INFO(TypeName) \
private: \
    friend struct SuperClassTypeDeducation<TypeName>; \
    friend struct TypeInfoInitializer<TypeName>; \
\
public: \
    using Super = typename SuperClassTypeDeducation<TypeName>::Type; \
    using ThisType = TypeName; \
\
    static TypeInfo& StaticTypeInfo() \
    { \
        static TypeInfo typeInfo{ TypeInfoInitializer<TypeName>(#TypeName) }; \
        return typeInfo; \
    } \
\
    virtual const TypeInfo& GetTypeInfo() const\
    { \
        return m_typeInfo; \
    } \
\
private: \
	inline static TypeInfo& m_typeInfo = StaticTypeInfo(); \
\
private: \
