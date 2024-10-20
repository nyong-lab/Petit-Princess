#pragma once
/// <summary>
/// 2024.01.02
///	무언가를 만들어낼 Builder의 인터페이스를 담은 추상클래스
/// 작성 : 유승운
/// </summary>
/// <typeparam name="T">만들 객체 타입</typeparam>
template<typename T>
class IBuilder
{
public:
	virtual ~IBuilder() {}

	virtual std::shared_ptr<T> Create() abstract;
};