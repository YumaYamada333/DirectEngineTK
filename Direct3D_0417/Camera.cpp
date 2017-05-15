#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(float width, float height)
	: m_forY (XM_PI / 4.f)
	, m_aspect_ratio (width / height)
	, m_near_clip (0.1f)
	, m_far_clip (500.0f)
{
	//�����o�ϐ��̏�����
	m_eyepos = Vector3(0.0f, 0.5f, 0.0f);
	m_refpos = Vector3(0.0f, 0.0f, 0.0f);
	m_upvec = Vector3(0.0f, 1.0f, 0.0f);

	//�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_forY,		//����p(�㉺����)	(�J�����̒���str������̊p�x)
		m_aspect_ratio,	//�A�X�y�N�g��
		m_near_clip,			//�j�A�N���b�v		-> �`�悷��͈�(�J��������̋���)
		m_far_clip);			//�t�@�[�N���b�v	-> �`�悷��͈�(�J��������̋���)
}

Camera::~Camera()
{

}

void Camera::Update()
{
	//�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_forY,		//����p(�㉺����)	(�J�����̒���str������̊p�x)
		m_aspect_ratio,	//�A�X�y�N�g��
		m_near_clip,			//�j�A�N���b�v		-> �`�悷��͈�(�J��������̋���)
		m_far_clip);			//�t�@�[�N���b�v	-> �`�悷��͈�(�J��������̋���)
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
