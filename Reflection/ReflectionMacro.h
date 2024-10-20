#pragma once

class TypeInfo;

/// 클래스
///
/// 클래스의 타입 정보를 담고 있는 객체를 생성
/// 자신의 타입과 부모 타입의 별칭을 일관된 이름으로 선언.

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


/// 멤버 변수
///
/// 변수 정보는 각 변수마다 정의된 등록 구조체이 생성자에 선언된 
/// PropertyRegister 객체를 통해 생성된다.
/// 템플릿 클래스는 PropertyRegister는 아래와 같은 정보를 담고 있어 각각의
/// 변수마다 유일한 타입을 갖게 한다.
/// 
/// 1. ThisType : 소유하고 있는 객체의 타입
/// 2. decltype(m_##Name) : 변수의 타입
/// 3. decltype(&ThisType::m_##Name) : 변수 포인터 타입 (static or 멤버)
/// 4. &ThisType::m_##Name : 변수의 주소
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
