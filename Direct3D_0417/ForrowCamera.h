/// <summary>
/// 自機に追従するカメラのクラスのヘッダ
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

	//追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//追従対象の回転角度をセット
	void SetTargetAngle(const float& targetdeg);

	//キーボードをセット
	void SetKeyboard(DirectX::Keyboard* keyboard);

	void SetTPS(DirectX::SimpleMath::Vector3& eyepos, DirectX::SimpleMath::Vector3& refpos);
	void SetFPS(DirectX::SimpleMath::Vector3& eyepos, DirectX::SimpleMath::Vector3& refpos);

	void Update() override;
	

};
