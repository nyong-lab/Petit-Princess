#pragma once

class TypeInfo;

/// Ŭ����
///
/// Ŭ������ Ÿ�� ������ ��� �ִ� ��ü�� ����
/// �ڽ��� Ÿ�԰� �θ� Ÿ���� ��Ī�� �ϰ��� �̸����� ����.

#define GENERATE_CLASS_TYPE_INFO(TypeName) \
private: \
    friend SuperClassTypeDeducation; \
    friend TypeInfoInitializer; \
\
public: \
    using Super = typename SuperClassTypeDeducation<TypeName>::Type; \
    using ThisType = TypeName; \
\
    static TypeInfo& staticTypeInfo() \
    { \
        static TypeInfo typeInfo{ TypeInfoInitializer<TypeName>(#TypeName) }; \
        return typeInfo; \
    } \
\
    virtual const TypeInfo& GetTypeInfo() const \
    { \
            return m_typeInfo; \
    } \
\
private: \
	inline static TypeInfo& m_typeInfo = staticTypeInfo(); \


/// ��� ����
///
/// ���� ������ �� �������� ���ǵ� ��� ����ü�� �����ڿ� ����� 
/// PropertyRegister ��ü�� ���� �����ȴ�.
/// ���ø� Ŭ������ PropertyRegister�� �Ʒ��� ���� ������ ��� �־� ������
/// �������� ������ Ÿ���� ���� �Ѵ�.
/// 
/// 1. ThisType : �����ϰ� �ִ� ��ü�� Ÿ��
/// 2. decltype(m_##Name) : ������ Ÿ��
/// 3. decltype(&ThisType::m_##Name) : ���� ������ Ÿ�� (static or ���)
/// 4. &ThisType::m_##Name : ������ �ּ�
/// 
#define PROPERTY(Name) \
inline static struct RegisterPropertyExecutor_##Name \
	{ \
		RegisterPropertyExecutor_##Name() \
		{ \
			static PropertyRegister<ThisType, decltype(m_##Name), \
			decltype(&ThisType::m_##Name), &ThisType::m_##Name> property_register_##Name \
			{ #Name, ThisType::StaticTypeInfo()}; \
		} \
\
	} \ register_##Name;
