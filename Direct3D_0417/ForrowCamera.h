/// <summary>
/// ���@�ɒǏ]����J�����̃N���X�̃w�b�_
/// </summary>
#pragma once

#include <windows.h>
#include <Keyboard.h>
#include "Camera.h"

class FollowCamera :public Camera
{
public:
	static const float CAMERA_DISTANCE;

private:
	

	DirectX::SimpleMath::Vector3 m_target_pos;
	float m_target_deg;
	DirectX::Keyboard* m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	bool m_fps_flag;

public:
	FollowCamera(float width, float height);
	~FollowCamera();

	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//�Ǐ]�Ώۂ̉�]�p�x���Z�b�g
	void SetTargetAngle(const float& targetdeg);

	//�L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);

	void SetTPS(DirectX::SimpleMath::Vector3& eyepos, DirectX::SimpleMath::Vector3& refpos);
	void SetFPS(DirectX::SimpleMath::Vector3& eyepos, DirectX::SimpleMath::Vector3& refpos);

	void Update() override;
	

};
