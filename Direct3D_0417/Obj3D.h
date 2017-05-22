/// <summary>
/// �RD�I�u�W�F�N�g�N���X�̃w�b�_
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Model.h>				//���f����\�����邽�߂̃w�b�_
#include <CommonStates.h>		//�R�����X�e�C�g���g�����߂̃w�b�_
#include <Effects.h>			//�G�t�F�N�g���g�����߂̃w�b�_
#include "ForrowCamera.h"		//�J�����w�b�_

class Obj3D
{
public:
	//�ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, 
		Camera* camera);

private:
	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>				m_d3dDevice;
	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
	//�ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates>			m_states;
	//�J����
	static Camera*											m_camera;
	//�G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory>			m_factory;

public:
	Obj3D();
	~Obj3D();

	//�����o�֐�
	
	//���f���̓ǂݍ���
	void LoadModel(const wchar_t* filename);
	//�X�V
	void Update();
	//�`��
	void Render();

	//setter, getter
	//�X�P�[�����O(XYZ)
	void SetScale(DirectX::SimpleMath::Vector3 scale);
	DirectX::SimpleMath::Vector3 GetScale();
	//��]�p(XYZ)
	void SetRotate(DirectX::SimpleMath::Vector3 rotate);
	DirectX::SimpleMath::Vector3 GetRotate();
	//���s�ړ�(XYZ)
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	DirectX::SimpleMath::Vector3 GetPosition();
	//���[���h�s��
	DirectX::SimpleMath::Matrix GetWorld();
	//�e��Obj3D
	void SetParent(Obj3D* parent);

private:
	//�����o�ϐ�

	//���f��
	std::unique_ptr<DirectX::Model> m_model;
	//�X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//��]�p(XYZ)
	DirectX::SimpleMath::Vector3 m_angle;
	//���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_pos;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e��Obj3D�ւ̃|�C���^
	Obj3D* m_obj_parent;

};

