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
	,m_fps_flag(true)
{

}

FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	Vector3 eyepos, refpos;
	auto state = m_keyboard->GetState();
	m_tracker.Update(state);

	if (m_tracker.pressed.C)
	{
		m_fps_flag = m_fps_flag ? false : true;
	}

	m_fps_flag
		? SetFPS(eyepos, refpos)
		: SetTPS(eyepos, refpos);
	//カメラの更新
	//自機に追従
	//ゴム紐
	////参照点座標を計算
	//Vector3 refpos = m_target_pos + Vector3(0, 2, 0);
	////自機からカメラ座標への差分
	//Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	////自機の後ろに回り込むための回転行列
	//Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	////カメラへのベクトルを回転
	//cameraV = Vector3::TransformNormal(cameraV, rotmat);
	////カメラ座標を計算
	//Vector3 eyepos = m_refpos + cameraV;


	{
		//FPS
		//Vector3 position;
		////カメラ座標を計算
		//position = m_target_pos + Vector3(0, 0.2f, 0);
		////自機からカメラ座標への差分
		//Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		////自機の後ろに回り込むための回転行列
		//Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
		////カメラへのベクトルを回転
		//cameraV = Vector3::TransformNormal(cameraV, rotmat);
		////ちょっと進んだ位置が視点
		//eyepos = position + cameraV * 0.1f;
		////しっかり進んだ位置が注視点
		//refpos = position + cameraV;
	}
	

	//視点を現在地から補間
	//eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
	//注視点を現在地から補間
	//refpos = m_refpos + (refpos - m_refpos) * 0.05f;

	SetEyePos(eyepos);
	SetRefPos(refpos);

	//基底クラスの関数呼び出し
	Camera::Update();
}

void FollowCamera::SetTPS(Vector3& eyepos, Vector3& refpos)
{
	//自機に追従
	//ゴム紐
	//参照点座標を計算
	refpos = m_target_pos + Vector3(0, 2, 0);
	//自機からカメラ座標への差分
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//自機の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//カメラへのベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//カメラ座標を計算
	eyepos = refpos + cameraV;
}

void FollowCamera::SetFPS(Vector3& eyepos, Vector3& refpos)
{
	//FPS
	Vector3 position;
	//カメラ座標を計算
	position = m_target_pos + Vector3(0, 0.2f, 0);
	//自機からカメラ座標への差分
	Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
	//自機の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//カメラへのベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//ちょっと進んだ位置が視点
	eyepos = position + cameraV * 0.1f;
	//しっかり進んだ位置が注視点
	refpos = position + cameraV;
}

void FollowCamera::SetTargetPos(const Vector3& targetpos)
{
	m_target_pos = targetpos;
}

void FollowCamera::SetTargetAngle(const float& targetdegree)
{
	m_target_deg = targetdegree;
}

void FollowCamera::SetKeyboard(Keyboard* keyboard)
{
	m_keyboard = keyboard;
}



