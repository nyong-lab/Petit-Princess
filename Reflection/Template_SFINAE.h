#pragma once

#include "MathHeaders.h"

class TypeInfo;
class BoxCollider;
class CapsuleCollider;
class SphereCollider;
class PlaneCollider;

// 전방선언만 하고 실제로 값을 사용하지 않았기에
// 게임엔진에 종속되지 않는다.
enum class Layers;
enum class LayerCollision;
enum class BodyType;
enum class ProjectionMode;
enum class FoodType;

namespace GraphicsTransfer
{
	enum class ModelKey;
}

/// std::void_t 는 가변 인자 템플릿으로 들어온 값들을 void로 만드는 단순한 함수이다.
/// 다만 SFINE를 이용할 수 있게되어 T::ThisType이 없는 경우 이 템플릿은 후보에서 제외된다.
/// 그런데 결국 T::ThisType 이 부모 클래스의 타입이 될 수 있는가하면 헷갈릴 수 있는데
/// 부모타입이 될 수 있다. 왜냐하면 A 클래스를 상속받는 B 클래스가 있다고 가정한다면
/// B 클래스에 ThisType 이 선언되기 전까지는 B::ThisType 은 A::ThisType이기 때문이다.
/// 
/// GENERATE_CLASS_TYPE_INFO 매크로를 확인해본다면 Super 타입 추론이 
/// ThisType을 얻어내는 과정 전 단계에 있으므로 Super 에서 사용되는 ThisType은 
/// 부모의 ThisType이다. 
/// 즉 부모가 있다면 Super의 타입은 부모의 ThisType (부모 클래스 타입)
/// 부모가 없다면 타입은 void를 사용한다.
template <typename T, typename U = void>
struct SuperClassTypeDeducation
{
	using Type = void;
};

template <typename T>
struct SuperClassTypeDeducation<T, std::void_t<typename T::ThisType>>
{
	using Type = T::ThisType;
};

// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/// TypeInfo에서 부모가 있을때와 없을때의 처리를 위해 HasSuper 템플릿을 사용한다.
/// C++20의 Concept와 if constexpr을 통해서 부모 타입의 TypeInfo를 얻어온다.
/// HasSuper는 클래스 T 멤버 변수로 Super 별칭이 있고 Super 가 void가 아닌것을 요구한다.
/// 이를 통해 Super타입 별칭이 없거나 void라면 부모 타입의 TypeInfo는 nullptr이 된다.
template <typename T>
concept HasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;

template <typename T>
concept HasStaticTypeInfo = requires{ T::StaticTypeInfo(); };

// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/// Vector
template <typename>
struct IsVector : std::false_type {};

template <typename T, typename Alloc>
struct IsVector<std::vector<T, Alloc>> : std::true_type {};

/// Map
template <typename>
struct IsMap : std::false_type {};

template <typename Key, typename Value>
struct IsMap<std::map<Key, Value>> : std::true_type {};

/// UnorderedMap
template <typename>
struct IsUnorderedMap : std::false_type {};

template <typename Key, typename Value>
struct IsUnorderedMap<std::unordered_map<Key, Value>> : std::true_type {};

/// String
template <typename>
struct IsString : std::false_type {};

template <>
struct IsString<std::string> : std::true_type {};

template <>
struct IsString<std::wstring> : std::true_type {};

/// 사용자 정의 구조체  Vector3, Quaternion, Rotator, Vector2
template <typename>
struct IsVector3 : std::false_type {};

template <>
struct IsVector3<Vector3> : std::true_type {};

template <typename>
struct IsVector2 : std::false_type {};

template <>
struct IsVector2<Vector2> : std::true_type {};

template <typename>
struct IsVector4 : std::false_type {};

template <>
struct IsVector4<Vector4> : std::true_type {};

template <typename>
struct IsQuaternion : std::false_type {};

template <>
struct IsQuaternion<Quaternion> : std::true_type {};

/// Enum class
/// Enum class 종류만큼 특수화 버전 만들어주기 
template <typename>
struct IsEnumClass : std::false_type {};

template <>
struct IsEnumClass<Layers> : std::true_type {};

template <>
struct IsEnumClass<LayerCollision> : std::true_type {};

template <>
struct IsEnumClass<BodyType> : std::true_type {};

template <>
struct IsEnumClass<ProjectionMode> : std::true_type {};

template <>
struct IsEnumClass<FoodType> : std::true_type {};

template <>
struct IsEnumClass<GraphicsTransfer::ModelKey> : std::true_type {};


/// 콜라이더를 상속받고 있는지 체크 (Scene, editor 에서 사용)
template <typename T>
struct IsCollider : std::false_type {};

template <>
struct IsCollider<BoxCollider> : std::true_type {};

template <>
struct IsCollider<CapsuleCollider> : std::true_type {};

template <>
struct IsCollider<PlaneCollider> : std::true_type {};

template <>
struct IsCollider<SphereCollider> : std::true_type {};