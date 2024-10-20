#include "Reflection_pch.h"
#include <fstream>
#include <iostream>
#include <random>

#include "TypeInfoMacro.h"
#include "PropertyMacro.h"


//#include <glassify/glassify.h>
#include "json.hpp"
using json = nlohmann::json;

struct MyVector3
{
	GENERATE_CLASS_TYPE_INFO(MyVector3)
		PROPERTY(x)
		PROPERTY(y)
		PROPERTY(z)


public:
	float x{}, y{}, z{};


	MyVector3() {}
	MyVector3(float a, float b, float c)
		: x(a), y(b), z(c) {}
};



struct MyQuaternion
{
	GENERATE_CLASS_TYPE_INFO(MyQuaternion)
		PROPERTY(x)
		PROPERTY(y)
		PROPERTY(z)
		PROPERTY(w)

public:
	float x{}, y{}, z{}, w{};

	MyQuaternion() {};
	MyQuaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}
};

struct Transform
{
	GENERATE_CLASS_TYPE_INFO(Transform)
		PROPERTY(position)
		PROPERTY(rotation)
		PROPERTY(scale)

public:
	MyVector3 position{};
	MyQuaternion rotation{};
	MyVector3 scale{};
};


//template <typename T>
//void DeserializeRecursive(const json& jsonData, Property::ReturnValueWrapper<T>& data)
//{
//	// jsonData 에 들어있는 값을 T 에 넣는 작업..
//	// jsonData 의 값의 타입과 T 는 항상 일치한다는 가정을 함
//	// T가 프로퍼티를 가진 타입(구조체, 클래스)이라면 재귀적으로 호출 되어야 함
//
//	// 리플렉션 된 타입일 경우
//	if constexpr (HasStaticTypeInfo<T>)
//	{
//		const TypeInfo& typeInfo = T::StaticTypeInfo();
//
//		for (const auto& property : typeInfo.GetProperties()) 
//		{
//			const char* propertyName = property->GetName();
//			const json& propertyData = jsonData[propertyName];
//
//			DeserializeRecursive(propertyData, property->Get(data));
//		}
//	}
//	else // c++ 기본 타입일 경우
//	{
//		data = jsonData.get<T>();
//	}
//}


template <typename T>
void DeserializeRecursive(const json& jsonData, T& data)
{
	// jsonData 에 들어있는 값을 T 에 넣는 작업..
	// jsonData 의 값의 타입과 T 는 항상 일치한다는 가정을 함
	// T가 프로퍼티를 가진 타입(구조체, 클래스)이라면 재귀적으로 호출 되어야 함

	// 리플렉션 된 타입일 경우
	if constexpr (HasStaticTypeInfo<T>)
	{
		const TypeInfo& typeInfo = T::StaticTypeInfo();

		assert(typeInfo.GetProperties().size() == jsonData.size()); // All properties must be reflected

		for (const auto& property : typeInfo.GetProperties()) {
			const char* propertyName = property->GetName();
			const json& propertyData = jsonData[propertyName];

			// 재귀적으로 DeserializeRecursive 호출

			using Type = int;

			if (property->GetTypeInfo().GetName() == "unreflected_type_variable")
			{
				if (propertyData.is_number_float())
				{
					DeserializeRecursive<float>(propertyData, property->Get<float>(&data));
				}
				else if (propertyData.is_number_integer())
				{
					DeserializeRecursive<int>(propertyData, property->Get<int>(&data));
				}
				else if (propertyData.is_boolean())
				{
					DeserializeRecursive<bool>(propertyData, property->Get<bool>(&data));
				}
				else if (propertyData.is_string())
				{
					DeserializeRecursive<std::string>(propertyData, property->Get<std::string>(&data));
				}
			}
			else if (property->GetTypeInfo().GetName() == "Vector3")
			{
				DeserializeRecursive<MyVector3>(propertyData, property->Get<MyVector3>(&data));
			}
			else if (property->GetTypeInfo().GetName() == "Quaternion")
			{
				DeserializeRecursive<MyQuaternion>(propertyData, property->Get<MyQuaternion>(&data));
			}
		}
	}
	else // c++ 기본 타입일 경우
	{
		data = jsonData.get<T>();
	}
}

decltype(auto) to_type(const char* s)
{

}

// DeserializeRecursive 함수를 TypeInfo를 활용하여 만들도록 수정
template <typename T>
void Deserialize(const json& jsonPoolData, std::vector<T>& pool)
{
	pool.reserve(jsonPoolData.size());

	const TypeInfo& typeInfo = T::StaticTypeInfo();

	for (const auto& jsonData : jsonPoolData)
	{
		T data;

		DeserializeRecursive(jsonData, data);

		pool.push_back(std::move(data));
	}
}

int main()
{

	auto& typeInfo = Transform::StaticTypeInfo();

	auto p1 = typeInfo.GetProperty("position");
	auto pp1 = p1->GetTypeInfo().GetProperty("x");
	auto p2 = typeInfo.GetProperty("rotation");
	auto p3 = typeInfo.GetProperty("scale");



	Transform tt;
	p1->Set(&tt, MyVector3{ 20.f, 30.f, 40.f });
	pp1->Set(&tt, 999.f);
	p2->Set(&tt, MyQuaternion{ 20.f, 30.f, 40.f, 50.f });
	p3->Set(&tt, MyVector3{ 20.f, 30.f, 40.f });

	auto x = typeInfo.GetProperty("position")->Get<MyVector3>(&tt);

	x.GetValue().y = 0.f;
	//// json을 파일로 저장
	//std::ofstream outputFile("../Data/Pool/Transform.json");
	//outputFile << jsonTransformPool.dump(2);  // 두 칸 들여쓰기로 보기 좋게 저장
	//outputFile.close();


	// JSON 파일 읽기
	std::ifstream inputFile("../Resource/JasonData/Test/Transform.json");
	if (!inputFile.is_open())
	{
		std::cerr << "Failed to open JSON file." << std::endl;
		return 1;  // 파일 열기 실패 시 종료
	}

	json jsonTransformPool;
	inputFile >> jsonTransformPool;


	if (jsonTransformPool.is_array()) {
		for (const auto& jsonTransform : jsonTransformPool) {
			Transform transform;

			transform.position.x = jsonTransform["position"]["x"];
			transform.position.y = jsonTransform["position"]["y"];
			transform.position.z = jsonTransform["position"]["z"];

			// 나머지 Transform 멤버 변수들도 읽어와야 함

			// 읽어온 데이터 활용
			std::cout << "Transform: (" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << ")" << std::endl;
		}
	}

	std::vector<Transform> vv;
	Deserialize<Transform>(jsonTransformPool, vv);

	return 0;
}
