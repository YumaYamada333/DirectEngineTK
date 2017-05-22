/// <summary>
/// �RD�I�u�W�F�N�g�̃N���X
/// </summary>

#include "Obj3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//�ÓI�����o�ϐ��̒�`
//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3D::m_d3dDevice;
//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>    Obj3D::m_d3dContext;
//�ėp�X�e�[�g�ݒ�
std::unique_ptr<DirectX::CommonStates> Obj3D::m_states;
//�J����
Camera* Obj3D::m_camera;
//�G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3D::m_factory;

//�R���X�g���N�^
Obj3D::Obj3D()
	:m_model(nullptr)
{

}

Obj3D::~Obj3D()
{
}

void Obj3D::InitializeStatic(
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
	Camera* camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");	//�e�N�X�`���̃p�X���w��(�t�H���_�����w��)
}

//���f���̓ǂݍ���
void Obj3D::LoadModel(const wchar_t* filename)
{
	m_model= Model::CreateFromCMO(m_d3dDevice.Get(), filename, *m_factory);
}

//�X�V
void Obj3D::Update()
{

}

//�`��
void Obj3D::Render()
{
	m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_camera->GetViewMatrix(), m_camera->GetProjMatrix());
}

//setter, getter
//�X�P�[�����O(XYZ)
void Obj3D::SetScale(DirectX::SimpleMath::Vector3 scale)
{
	m_scale = scale;
}
DirectX::SimpleMath::Vector3 Obj3D::GetScale()
{
	return m_scale;
}
//��]�p(XYZ)
void Obj3D::SetRotate(DirectX::SimpleMath::Vector3 rotate)
{
	m_angle = rotate;
}
DirectX::SimpleMath::Vector3 Obj3D::GetRotate()
{
	return m_angle;
}
//���s�ړ�(XYZ)
void Obj3D::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_pos = position;
}
DirectX::SimpleMath::Vector3 Obj3D::GetPosition()
{
	return m_pos;
}
//���[���h�s��
DirectX::SimpleMath::Matrix Obj3D::GetWorld()
{
	return m_world;
}
//�e��Obj3D
void Obj3D::SetParent(Obj3D* parent)
{
	m_obj_parent = parent;
}