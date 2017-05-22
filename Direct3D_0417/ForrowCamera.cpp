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
	//�J�����̍X�V
	//���@�ɒǏ]
	//�S���R
	////�Q�Ɠ_���W���v�Z
	//Vector3 refpos = m_target_pos + Vector3(0, 2, 0);
	////���@����J�������W�ւ̍���
	//Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	////���@�̌��ɉ�荞�ނ��߂̉�]�s��
	//Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	////�J�����ւ̃x�N�g������]
	//cameraV = Vector3::TransformNormal(cameraV, rotmat);
	////�J�������W���v�Z
	//Vector3 eyepos = m_refpos + cameraV;


	{
		//FPS
		//Vector3 position;
		////�J�������W���v�Z
		//position = m_target_pos + Vector3(0, 0.2f, 0);
		////���@����J�������W�ւ̍���
		//Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		////���@�̌��ɉ�荞�ނ��߂̉�]�s��
		//Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
		////�J�����ւ̃x�N�g������]
		//cameraV = Vector3::TransformNormal(cameraV, rotmat);
		////������Ɛi�񂾈ʒu�����_
		//eyepos = position + cameraV * 0.1f;
		////��������i�񂾈ʒu�������_
		//refpos = position + cameraV;
	}
	

	//���_�����ݒn������
	//eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
	//�����_�����ݒn������
	//refpos = m_refpos + (refpos - m_refpos) * 0.05f;

	SetEyePos(eyepos);
	SetRefPos(refpos);

	//���N���X�̊֐��Ăяo��
	Camera::Update();
}

void FollowCamera::SetTPS(Vector3& eyepos, Vector3& refpos)
{
	//���@�ɒǏ]
	//�S���R
	//�Q�Ɠ_���W���v�Z
	refpos = m_target_pos + Vector3(0, 2, 0);
	//���@����J�������W�ւ̍���
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//�J�����ւ̃x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W���v�Z
	eyepos = refpos + cameraV;
}

void FollowCamera::SetFPS(Vector3& eyepos, Vector3& refpos)
{
	//FPS
	Vector3 position;
	//�J�������W���v�Z
	position = m_target_pos + Vector3(0, 0.2f, 0);
	//���@����J�������W�ւ̍���
	Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
	//���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_target_deg));
	//�J�����ւ̃x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//������Ɛi�񂾈ʒu�����_
	eyepos = position + cameraV * 0.1f;
	//��������i�񂾈ʒu�������_
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



