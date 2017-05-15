/// <summary>
/// ���@�ɒǏ]����J�����̃N���X�̃\�[�X
/// </summary>

#include "ForrowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̏�����
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
	//�J�����̍X�V
	//���@�ɒǏ]
	//�Q�Ɠ_���W���v�Z
	m_refpos = m_target_pos + Vector3(0, 2, 0);
	//���@����J�������W�ւ̍���
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//�J�����ւ̃x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W���v�Z
	m_eyepos = m_refpos + cameraV;

	/*SetEyePos(m_eyepos);
	SetRefPos(m_refpos);*/

	//���N���X�̊֐��Ăяo��
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



