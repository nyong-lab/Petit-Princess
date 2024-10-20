#pragma once

/// <summary>
/// ���÷����� ���ʹϾ��� ���� �������� �� �����ϰ� ȸ������ �����ǰԲ� ���ִ� ��ƿ Ŭ����
/// �ۼ��� : �赿��
/// ���� �ռ��� -> rotateBypivot, eulertoquat �Լ� �߰�
/// </summary>
class QuaternionUtils
{
public:
	// ���Ϸ� ������ ��ȯ�Ѵ�.
	static Vector3 ToEulerAngles(Quaternion& q);
	static Quaternion MyCreateFromYawPitchRoll(const Vector3& angles);
	static Vector3 RotateByPivot(const Vector3& originPos, const Vector3& pivot, const Vector3& rotation);
	static Matrix EulerToQuaternion(Vector3 euler);
};

