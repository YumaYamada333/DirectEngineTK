//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <PrimitiveBatch.h>		//プリミティブバッチクラスのためのヘッダ
#include <VertexTypes.h>		//Vertex系を使うためのヘッダ
#include <Effects.h>			//エフェクトを使うためのヘッダ
#include <CommonStates.h>		//コモンステイとを使うためのヘッダ
#include <SimpleMath.h>			//Vector,Colorを使うためのヘッダ
#include <Model.h>				//モデルを表示するためのヘッダ
#include "DebugCamera.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

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

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//VertexPositonColor -> 頂点に位置と色を持たせる
	//PrimitiveBatch -> 指定した頂点のデータを使って図形を描く
	//unique_ptr -> スマートポインタの一つ、メモリの所有者が一人(複数の人が使いまわさない)
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debug_camera;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	//地面モデル
	std::unique_ptr<DirectX::Model> m_model_ground;
	//天球モデル
	std::unique_ptr<DirectX::Model> m_model_skydome;
};