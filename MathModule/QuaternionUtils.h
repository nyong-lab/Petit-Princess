#pragma once

/// <summary>
/// 리플렉션한 쿼터니언의 값을 조작했을 때 유연하게 회전값이 변동되게끔 해주는 유틸 클래스
/// 작성자 : 김동일
/// 수정 손서희 -> rotateBypivot, eulertoquat 함수 추가
/// </summary>
class QuaternionUtils
{
public:
	// 오일러 각으로 변환한다.
	static Vector3 ToEulerAngles(Quaternion& q);
	static Quaternion MyCreateFromYawPitchRoll(const Vector3& angles);
	static Vector3 RotateByPivot(const Vector3& originPos, const Vector3& pivot, const Vector3& rotation);
	static Matrix EulerToQuaternion(Vector3 euler);
};

