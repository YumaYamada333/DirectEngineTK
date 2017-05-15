/// <summary>
/// �J�����𐧌䂷��N���X
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	Camera(float width, float height);
	~Camera();
	virtual void Update();
	
	const DirectX::SimpleMath::Matrix& GetViewMatrix();
	const DirectX::SimpleMath::Matrix& GetProjMatrix();
	void SetEyePos(DirectX::SimpleMath::Vector3 eyepos);
	void SetRefPos(DirectX::SimpleMath::Vector3 refpos);

protected:

	//�r���[�s��
	DirectX::SimpleMath::Matrix  m_view;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix  m_proj;

	//�J�����̈ʒu(���_���W)
	DirectX::SimpleMath::Vector3 m_eyepos;
	//�J���������Ă���ʒu(�����_/�Q�Ɠ_/���ړ_)
	DirectX::SimpleMath::Vector3 m_refpos;
	//�J�����̏�����x�N�g��(�P�ʃx�N�g���̂�)(�ȂȂ߂̎��͐��K������)
	DirectX::SimpleMath::Vector3 m_upvec;
	
	//������������p
	float m_forY;
	//�A�X�y�N�g��
	float m_aspect_ratio;
	//�j�A�N���b�v
	float m_near_clip;
	//�t�@�[�N���b�v
	float m_far_clip;
	
};
