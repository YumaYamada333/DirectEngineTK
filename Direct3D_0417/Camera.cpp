#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(float width, float height)
	: m_forY (XM_PI / 4.f)
	, m_aspect_ratio (width / height)
	, m_near_clip (0.1f)
	, m_far_clip (500.0f)
{
	//メンバ変数の初期化
	m_eyepos = Vector3(0.0f, 0.5f, 0.0f);
	m_refpos = Vector3(0.0f, 0.0f, 0.0f);
	m_upvec = Vector3(0.0f, 1.0f, 0.0f);

	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_forY,		//視野角(上下方向)	(カメラの直横str線からの角度)
		m_aspect_ratio,	//アスペクト比
		m_near_clip,			//ニアクリップ		-> 描画する範囲(カメラからの距離)
		m_far_clip);			//ファークリップ	-> 描画する範囲(カメラからの距離)
}

Camera::~Camera()
{

}

void Camera::Update()
{
	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_forY,		//視野角(上下方向)	(カメラの直横str線からの角度)
		m_aspect_ratio,	//アスペクト比
		m_near_clip,			//ニアクリップ		-> 描画する範囲(カメラからの距離)
		m_far_clip);			//ファークリップ	-> 描画する範囲(カメラからの距離)
}

void Camera::SetEyePos(Vector3 eyepos)
{
	m_eyepos = eyepos;
	if (m_eyepos == m_refpos)
	{
		m_refpos += Vector3(0.0f, 0.1f, 0.0f);
	}
}

void Camera::SetRefPos(Vector3 refpos)
{
	m_refpos = refpos;
}

const Matrix& Camera::GetProjMatrix()
{
	return m_proj;
}

const Matrix& Camera::GetViewMatrix()
{
	return m_view;
}
