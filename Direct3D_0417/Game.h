//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <PrimitiveBatch.h>		//�v���~�e�B�u�o�b�`�N���X�̂��߂̃w�b�_
#include <VertexTypes.h>		//Vertex�n���g�����߂̃w�b�_
#include <Effects.h>			//�G�t�F�N�g���g�����߂̃w�b�_
#include <CommonStates.h>		//�R�����X�e�C�Ƃ��g�����߂̃w�b�_
#include <SimpleMath.h>			//Vector,Color���g�����߂̃w�b�_
#include <Model.h>				//���f����\�����邽�߂̃w�b�_
#include <Keyboard.h>			//�L�[�{�[�h�w�b�_
#include "DebugCamera.h"
#include "Camera.h"
#include "ForrowCamera.h"
#include "Obj3D.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	static const int POT_NUM = 20;
	const int SKY_RUD = 100;

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:
	const int BALL_NUM = 20;
	const int GROUND_NUM = 40000;
	const int COUNT_NUM = 360 * 60;



    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	//�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<FollowCamera> m_camera;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//VertexPositonColor -> ���_�Ɉʒu�ƐF����������
	//PrimitiveBatch -> �w�肵�����_�̃f�[�^���g���Đ}�`��`��
	//unique_ptr -> �X�}�[�g�|�C���^�̈�A�������̏��L�҂���l(�����̐l���g���܂킳�Ȃ�)
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�ėp�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debug_camera;

	//�G�t�F�N�g�t�@�N�g��
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	//�n�ʃ��f��
	std::unique_ptr<DirectX::Model> m_model_ground;
	//�V�����f��
	std::unique_ptr<DirectX::Model> m_model_skydome;
	//�{�[�����f��
	std::unique_ptr<DirectX::Model> m_model_ball[20];
	//�|�b�g���f��
	std::unique_ptr<DirectX::Model> m_model_pot[Game::POT_NUM];
	//�����f��
	std::unique_ptr<DirectX::Model> m_model_head;
	//���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldBall;
	DirectX::SimpleMath::Matrix m_worldPot[POT_NUM];
	DirectX::SimpleMath::Matrix m_transmat[POT_NUM];
	DirectX::SimpleMath::Matrix m_worldGround;
	//���̈ʒu
	DirectX::SimpleMath::Vector3 m_head_pos;
	float m_head_deg;
	DirectX::SimpleMath::Matrix m_head_world;
	//���A�̈ʒu
	DirectX::SimpleMath::Vector3 m_head_pos2;
	float m_head_deg2;
	DirectX::SimpleMath::Matrix m_head_world2;

	//3D�I�u�W�F�N�g
	Obj3D m_obj;

	//std::vector<Obj3D> m_ObjPlayer;

	int count;
	float m_movetime;
	float m_timestep;
	DirectX::SimpleMath::Vector3 pot_pos[POT_NUM];
	
};