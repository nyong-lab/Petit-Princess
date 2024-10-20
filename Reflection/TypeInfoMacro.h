#pragma once

#include "TypeInfo.h"


/// 클래스 타입 정보를 생성하는 매크로
///
/// 클래스의 타입 정보를 담고 있는 TypeInfo 객체를 생성
/// 자신의 타입과 부모 타입의 별칭을 일관된 이름으로 선언 (Super, ThisType)
/// 
/// 1. Super라는 부모 클래스 타입의 별칭과 ThisType이라는 자신의 타입 별칭 선언
/// ThisType은 매크로의 TypeName을 그대로 이용할 수 있지만, 부모 클래스의 타입은
/// 부모 클래스가 없는 경우로 인해 추가적인 처리가 필요하다.
/// 부모 클래스 타입 추론을 위해 SuperClassTypeDeducation 템플릿을 사용한다.
/// 
/// 2. 클래스에 대한 TypeInfo 객체를 정적 멤버 변수로 선언.
/// TypeInfo는 타입의 상속 관계를 표현한고 추후에 해당 타입이 가지고 있는
/// 멤버 변수들에 대한 정보를 가지게 된다. (Property class)
/// Super 타입추론을 했을때 처럼 부모가 없는 경우에 대한 처리가 필요하다.
/// 해당 처리는 TypeInfo 초기화용으로 사용되는 TypeInitializer 구조체에서 처리한다.
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
