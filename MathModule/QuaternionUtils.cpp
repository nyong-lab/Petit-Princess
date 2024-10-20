#include "MathHeaders.h"
#include "QuaternionUtils.h"


// 보기 편하게 디그리로 
DirectX::SimpleMath::Vector3 QuaternionUtils::ToEulerAngles(Quaternion& q)
{
	//Vector3 angles;

	//// roll (x-axis rotation)
	//double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	//double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	//angles.x = std::atan2(sinr_cosp, cosr_cosp);

	//// pitch (y-axis rotation)
	//double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	//double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	//angles.y = 2 * std::atan2(sinp, cosp) - 3.14159 / 2;

	//// yaw (z-axis rotation)
	//double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	//double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	//angles.z = std::atan2(siny_cosp, cosy_cosp);

	////// 라디안에서 디그리로 변환
	////angles.x = DirectX::XMConvertToDegrees(angles.x);
	////angles.y = DirectX::XMConvertToDegrees(angles.y);
	////angles.z = DirectX::XMConvertToDegrees(angles.z);

	//return angles;

	const float xx = q.x * q.x;
	const float yy = q.y * q.y;
	const float zz = q.z * q.z;

	const float m31 = 2.f * q.x * q.z + 2.f * q.y * q.w;
	const float m32 = 2.f * q.y * q.z - 2.f * q.x * q.w;
	const float m33 = 1.f - 2.f * xx - 2.f * yy;

	const float cy = sqrtf(m33 * m33 + m31 * m31);
	const float cx = atan2f(-m32, cy);
	if (cy > 16.f * FLT_EPSILON)
	{
		const float m12 = 2.f * q.x * q.y + 2.f * q.z * q.w;
		const float m22 = 1.f - 2.f * xx - 2.f * zz;

		return Vector3(cx, atan2f(m31, m33), atan2f(m12, m22));
	}
	else
	{
		const float m11 = 1.f - 2.f * yy - 2.f * zz;
		const float m21 = 2.f * q.x * q.y - 2.f * q.z * q.w;

		return Vector3(cx, 0.f, atan2f(-m21, m11));
	}
}

DirectX::SimpleMath::Quaternion QuaternionUtils::MyCreateFromYawPitchRoll(const Vector3& angles)
{
    Quaternion R;
    XMStoreFloat4(&R, DirectX::XMQuaternionRotationRollPitchYawFromVector(angles));
    return R;
}

DirectX::SimpleMath::Vector3 QuaternionUtils::RotateByPivot(const Vector3& originPos, const Vector3& pivot, const Vector3& rotation)
{
	Vector3 result;

	//회전 중심을 옮기기 위해 위치를 pivot만큼 이동
	result -= pivot;

	//쿼터니언을 이용한 회전
	DirectX::XMMATRIX mat = EulerToQuaternion(rotation);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(originPos.x, originPos.y, originPos.z, 1.f);
	vec = XMVector3Transform(vec, mat);

	// 계산 값을 다시 가져온다
	DirectX::XMVECTORF32 data;
	data.v = vec;

	result.x = data.f[0];
	result.y = data.f[1];
	result.z = data.f[2];
	result += pivot;

	return result;
}

Matrix QuaternionUtils::EulerToQuaternion(Vector3 euler)
{
	// 쿼터니언`으로 회전을 생성합니다.
	DirectX::XMVECTOR quaternionRotation = DirectX::XMQuaternionRotationRollPitchYaw(euler.x, euler.y, euler.z);

	// 쿼터니언을 회전 매트릭스로 변환합니다.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternionRotation);

	return rotationMatrix;
}
