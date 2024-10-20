#pragma once

#include "MathHeaders.h"

class TypeInfo;
class BoxCollider;
class CapsuleCollider;
class SphereCollider;
class PlaneCollider;

// ���漱�� �ϰ� ������ ���� ������� �ʾұ⿡
// ���ӿ����� ���ӵ��� �ʴ´�.
enum class Layers;
enum class LayerCollision;
enum class BodyType;
enum class ProjectionMode;
enum class FoodType;

namespace GraphicsTransfer
{
	enum class ModelKey;
}

/// std::void_t �� ���� ���� ���ø����� ���� ������ void�� ����� �ܼ��� �Լ��̴�.
/// �ٸ� SFINE�� �̿��� �� �ְԵǾ� T::ThisType�� ���� ��� �� ���ø��� �ĺ����� ���ܵȴ�.
/// �׷��� �ᱹ T::ThisType �� �θ� Ŭ������ Ÿ���� �� �� �ִ°��ϸ� �򰥸� �� �ִµ�
/// �θ�Ÿ���� �� �� �ִ�. �ֳ��ϸ� A Ŭ������ ��ӹ޴� B Ŭ������ �ִٰ� �����Ѵٸ�
/// B Ŭ������ ThisType �� ����Ǳ� �������� B::ThisType �� A::ThisType�̱� �����̴�.
/// 
/// GENERATE_CLASS_TYPE_INFO ��ũ�θ� Ȯ���غ��ٸ� Super Ÿ�� �߷��� 
/// ThisType�� ���� ���� �� �ܰ迡 �����Ƿ� Super ���� ���Ǵ� ThisType�� 
/// �θ��� ThisType�̴�. 
/// �� �θ� �ִٸ� Super�� Ÿ���� �θ��� ThisType (�θ� Ŭ���� Ÿ��)
/// �θ� ���ٸ� Ÿ���� void�� ����Ѵ�.
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

// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

/// TypeInfo���� �θ� �������� �������� ó���� ���� HasSuper ���ø��� ����Ѵ�.
/// C++20�� Concept�� if constexpr�� ���ؼ� �θ� Ÿ���� TypeInfo�� ���´�.
/// HasSuper�� Ŭ���� T ��� ������ Super ��Ī�� �ְ� Super �� void�� �ƴѰ��� �䱸�Ѵ�.
/// �̸� ���� SuperŸ�� ��Ī�� ���ų� void��� �θ� Ÿ���� TypeInfo�� nullptr�� �ȴ�.
template <typename T>
concept HasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;

template <typename T>
concept HasStaticTypeInfo = requires{ T::StaticTypeInfo(); };

// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

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

/// ����� ���� ����ü  Vector3, Quaternion, Rotator, Vector2
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
/// Enum class ������ŭ Ư��ȭ ���� ������ֱ� 
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


/// �ݶ��̴��� ��ӹް� �ִ��� üũ (Scene, editor ���� ���)
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