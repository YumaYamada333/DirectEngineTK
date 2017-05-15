/// <summary>
/// 自機に追従するカメラのクラスのソース
/// </summary>

#include "ForrowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(float width, float height)
	:Camera(width, height)
	,m_target_deg(0.0f)
{

}

FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	//カメラの更新
	//自機に追従
	//参照点座標を計算
	m_refpos = m_target_pos + Vector3(0, 2, 0);
	//自機からカメラ座標への差分
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//自機の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//カメラへのベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//カメラ座標を計算
	m_eyepos = m_refpos + cameraV;

	/*SetEyePos(m_eyepos);
	SetRefPos(m_refpos);*/

	//基底クラスの関数呼び出し
	Camera::Update();
}

void FollowCamera::SetTargetPos(const Vector3& targetpos)
{
	m_target_pos = targetpos;
}

void FollowCamera::SetTargetAngle(const float& targetdegree)
{
	m_target_deg = targetdegree;
}



