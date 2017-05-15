/// <summary>
/// ���@�ɒǏ]����J�����̃N���X�̃w�b�_
/// </summary>
#pragma once

#include "Camera.h"

class FollowCamera :public Camera
{
private:
	static const float CAMERA_DISTANCE;

	DirectX::SimpleMath::Vector3 m_target_pos;
	float m_target_deg;

public:
	FollowCamera(float width, float height);
	~FollowCamera();

	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//�Ǐ]�Ώۂ̉�]�p�x���Z�b�g
	void SetTargetAngle(const float& targetdeg);

	void Update() override;
	

};
