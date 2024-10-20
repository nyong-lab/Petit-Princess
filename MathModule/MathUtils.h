#pragma once

/// <summary>
/// 작성자 : 손서희
/// </summary>
namespace MathUtils
{
template <typename T>
	static T Clamp(
		const T& value, 
		const T& min,
		const T& max);

template <typename T>
static T Lerp(const T& arg1, const T& arg2, float ratio);


static Vector2 WorldToScreenPixel(Vector3 worldPos, Matrix viewProj, int screenWidth, int screenHeight)
{
	Vector3 screenPos = XMVector3TransformCoord(worldPos, viewProj);

	// 스크린 좌표를 [-1, 1] 범위로 정규화
	float screenX = screenPos.x;
	float screenY = screenPos.y;

	// 정규화된 스크린 좌표를 픽셀 좌표로 변환
	float pixelX = (screenX + 1.0f) * 0.5f * screenWidth;
	float pixelY = (1.0f - screenY) * 0.5f * screenHeight;

	return Vector2(pixelX, pixelY);
};

};


template <typename T>
T MathUtils::Clamp(const T& value, const T& min, const T& max)
{
	return (value < min) ? min : (value > max) ? max : value;
}


template<>
Vector3 MathUtils::Clamp<Vector3>(const Vector3& value, const Vector3& min, const Vector3& max)
{
	float clampedX = Clamp(value.x, min.x, max.x);
	float clampedY = Clamp(value.y, min.y, max.y);
	float clampedZ = Clamp(value.z, min.z, max.z);

	return Vector3(clampedX, clampedY, clampedZ);
}


//-------------------------------------------------------------------------------------------------------------


template <typename T>
T MathUtils::Lerp(const T& v1, const T& v2, float ratio)
{
	if (ratio < 0) ratio = 0;
	else if (1 < ratio) ratio = 1;

	return v1 * (1 - ratio) + v2 * ratio;
}

template <>
Vector3 MathUtils::Lerp<Vector3>(const Vector3& arg1, const Vector3& arg2, float ratio)
{
	Vector3 result;
	result.x = Lerp(arg1.x, arg2.x, ratio);
	result.y = Lerp(arg1.y, arg2.y, ratio);
	result.z = Lerp(arg1.z, arg2.z, ratio);

	return result;
}

template <>
Vector4 MathUtils::Lerp<Vector4>(const Vector4& arg1, const Vector4& arg2, float ratio)
{
	Vector4 result;
	result.x = Lerp(arg1.x, arg2.x, ratio);
	result.y = Lerp(arg1.y, arg2.y, ratio);
	result.z = Lerp(arg1.z, arg2.z, ratio);
	result.w = Lerp(arg1.w, arg2.w, ratio);

	return result;
}