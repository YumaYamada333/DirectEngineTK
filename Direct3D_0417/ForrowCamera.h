/// <summary>
/// 自機に追従するカメラのクラスのヘッダ
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

	//追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//追従対象の回転角度をセット
	void SetTargetAngle(const float& targetdeg);

	void Update() override;
	

};
