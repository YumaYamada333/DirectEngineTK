﻿//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "ctime"



extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//グローバル変数＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	srand(static_cast<unsigned int> (time(nullptr)));

    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
	//初期化はここに書く

	//キーボードの生成
	m_keyboard = std::make_unique<Keyboard>();
	//カメラを生成
	m_camera = std::make_unique<FollowCamera>(static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_camera->SetKeyboard(m_keyboard.get());
	//３Dオブジェクトの静的メンバを初期化
	Obj3D::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	//引数としてそのままのポインタを要求されている場合、スマートポインタ.Get()と関数を呼び出すと隠されたポインタが返ってくる
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	//射影行列
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//デバッグカメラを生成
	m_debug_camera = std::make_unique<DebugCamera>(m_outputHeight, m_outputWidth);

	//エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");	//テクスチャのパスを指定(フォルダ名を指定)
	//モデルの生成(引数は　デバイス、読み込むcmo、エフェクトファクトリ(実体))
	m_model_ground = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\ground_200m.cmo", *m_factory);
	m_model_head = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\Head.cmo", *m_factory);
	m_obj.LoadModel(L"Resources\\Head.cmo");

	m_model_skydome = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\skydome.cmo", *m_factory);
	for (int i = 0; i < BALL_NUM; i++)
	{
		m_model_ball[i] = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\ball.cmo", *m_factory);
	}

	for (int i = 0; i < POT_NUM; i++)
	{
		m_model_pot[i] = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\tea_pot.cmo", *m_factory);
	}

	m_worldBall = Matrix::Identity;
	//ポットの初期位置設定
	for (int i = 0; i < POT_NUM; i++)
	{
		float pot_degX = static_cast<float>(rand() % 360);
		float pot_degZ = static_cast<float>(rand() % 360);
		float pot_desX = static_cast<float>(rand() % SKY_RUD);
		float pot_desZ = static_cast<float>(rand() % SKY_RUD);

		Matrix transmat = Matrix::CreateTranslation(cos(XMConvertToRadians(pot_degX)) * pot_desX, 0.0f, sin(XMConvertToRadians(pot_degZ)) * pot_desZ);
		pot_pos[i].x = cos(XMConvertToRadians(pot_degX)) * pot_desX;
		pot_pos[i].z = sin(XMConvertToRadians(pot_degZ)) * pot_desZ;
		pot_pos[i].y = 0.0f;

		m_transmat[i] = transmat;
	}

	m_head_pos = Vector3(0.0f, 0.0f, 30.0f);
	m_head_deg = 0.0f;
	count = 0;
	m_timestep = 0;
	m_movetime = 0;
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{

    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
	//毎フレーム処理を書く
	auto key = m_keyboard->GetState();	//キーボードの状態取得

	count++;
	m_movetime++;
	m_timestep = m_movetime / 600.0f;
	if (m_timestep > 1.0f)
	{
		m_timestep = 1.0f;
	}

	if (count > COUNT_NUM)
	{
		count = 0;
	}

	//ポットの位置を更新
	for (int i = 0; i < POT_NUM; i++)
	{
		//float deg = rand() % 360;
		//float pot_degZ = rand() % 360;
		//float pot_desX = rand() % SKY_RUD;
		//float pot_desZ = rand() % SKY_RUD;

		float val = (1.0f + sin(XMConvertToRadians(count % 360))) * 2.0f + 1.0f;

		//Matrix transmat = Matrix::CreateTranslation(XMConvertToRadians(pot_degX) + pot_desX, 0.0f, XMConvertToRadians(pot_degZ) + pot_desZ);
		Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(count % 360));
		Matrix scale = Matrix::CreateScale(val);
		m_transmat[i] = Matrix::CreateTranslation((1 - m_timestep) * pot_pos[i].x, 0.0f, (1 - m_timestep) * pot_pos[i].z);

		m_worldPot[i] = rotmatY * scale * m_transmat[i];
	}



	m_debug_camera->Update();

	//カメラの更新
	////時期に追従
	//const float CAMERA_DISTANCE = 5.0f;
	////視点、参照点
	//Vector3 eyepos, refpos;
	////参照点座標を計算
	//refpos = m_head_pos + Vector3(0, 2, 0);
	////自機からカメラ座標への差分
	//Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	////自機の後ろに回り込むための回転行列
	//Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_head_deg));
	////カメラへのベクトルを回転
	//cameraV = Vector3::TransformNormal(cameraV, rotmat);
	////カメラ座標を計算
	//eyepos = refpos + cameraV;


	//m_camera->SetEyePos(eyepos);
	//m_camera->SetRefPos(refpos);
	m_camera->SetTargetPos(/*m_obj.GetPosition()*/m_head_pos);
	m_camera->SetTargetAngle(/*m_obj.GetRotate().y*/m_head_deg);

	m_camera->Update();
	m_view = m_camera->GetViewMatrix();
	m_proj = m_camera->GetProjMatrix();

	//キーボード操作

	//Aキーで左旋回
	if (key.A)
	{
		////回転ベクトル(Y軸回転)
		//Vector3 Rot(0, 1, 0);
		////旋回
		//m_obj.SetRotate(Rot);
		m_head_deg++;
	}
	//Dキーで右旋回
	if (key.D)
	{
		////回転ベクトル(Y軸回転)
		//Vector3 Rot(0, -1, 0);
		////旋回
		//m_obj.SetRotate(Rot);
		m_head_deg--;
	}

	//自機のワールド行列を計算(回転)
	Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(m_head_deg));
	//Matrix rotmatY2 = Matrix::CreateRotationY(XMConvertToRadians(m_head_deg));
	Matrix rotmatY2 = Matrix::CreateRotationY(0);

	//Wキーで前進
	if (key.W)
	{
		//移動ベクトル(Z座標前進)
		Vector3 moveV(0, 0, -0.1f);
		//自機の座標を移動(回転角から移動量を計算)
		m_head_pos += SimpleMath::Vector3::TransformNormal(moveV, rotmatY);
	}
	//Sキーで後退
	if (key.S)
	{
		//移動ベクトル(Z座標後退)
		Vector3 moveV(0, 0, 0.1f);
		//自機の座標を移動(回転角から移動量を計算)
		m_head_pos += SimpleMath::Vector3::TransformNormal(moveV, rotmatY);
	}

	//ワールド行列を計算(回転 * 移動)
	Matrix transmat = Matrix::CreateTranslation(m_head_pos);
	//Matrix transmat2 = Matrix::CreateTranslation(m_head_pos);
	Matrix transmat2 = Matrix::CreateTranslation(Vector3(0, 0.5f, 0));
	m_head_world = rotmatY * transmat;
	//頭②のワールド行列を計算する(子供の行列＊親の行列)
	m_head_world2 = rotmatY2 * transmat2 * rotmatY * transmat;

	//ワールド行列を計算
	////スケーリング
	//Matrix scalemat = Matrix::CreateScale(2.0f);

	////ロール
	//Matrix rotmatZ = Matrix::CreateRotationZ(1.57f);
	////ピッチ（仰角）
	//Matrix rotmatX = Matrix::CreateRotationX(1.57f);
	////ヨー(方位角)
	//Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(15.0f));
	////回転行列の合成
	//Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	////平行移動
	//Matrix transmat = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);

	////ワールド行列の合成
	//m_worldBall = scalemat * transmat * rotmat;

	//m_worldBall = rotmatX;
}

// Draws the scene.
void Game::Render()
{
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3,
	};

	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)},
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
	};

    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	//描画処理を書く
	
	//オペーク -> 不透明度設定
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//DepthNone() -> 奥行情報(デプスバッファ)は使わない
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//CullNone() -> カリング(表裏)を反映しない
	m_d3dContext->RSSetState(m_states->CullNone());

	//ビュー行列を生成
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0.f, 0.f, 20.f),		//カメラの座標
	//	Vector3::Zero,				//カメラの注視点
	//	Vector3::UnitY);			//上方向ベクトル(画面の上方向がワールドのどこになるかを決定する)
	//m_view = m_debug_camera->GetCameraMatrix();
	
	//カメラの位置(視点座標)
	//Vector3 eyepos(0.0f, 0.0f, 5.0f);
	////カメラが見ている位置(注視点/参照点/注目点)
	//Vector3 refpos(0.0f, 0.0f, 0.0f);
	////カメラの上方向ベクトル(単位ベクトルのみ)(ななめの時は正規化する)
	//Vector3 upvec(0.0f, 1.0f, 0.0f);
	////ビュー行列の生成
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);

	//垂直方向視野角
	//float forY = XMConvertToRadians(60.0f);
	////アスペクト比
	////ニアクリップ
	////ファークリップ
	////プロジェクション行列を生成(遠近法を加味した画面の描画)
	//m_proj = Matrix::CreatePerspectiveFieldOfView(
	//	XM_PI / 4.f,		//視野角(上下方向)	(カメラの直横str線からの角度)
	//	float(m_outputWidth) / float(m_outputHeight),	//アスペクト比
	//	0.1f,			//ニアクリップ		-> 描画する範囲(カメラからの距離)
	//	500.0f);			//ファークリップ	-> 描画する範囲(カメラからの距離)

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);


	//上の設定を反映する
	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//モデルの描画(引数は　コンテキスト、コモンステート(実体)、ワールド行列、ビュー行列、射影行列)
	m_model_ground->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	m_model_skydome->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	//ボールの描画
	/*for (int i = 0; i < BALL_NUM; i++)
	{
		Matrix transmat = Matrix::CreateTranslation(20.0f * ((i / 10) + 1), 0.0f, 0.0f);
		Matrix rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(36.0f * i + (count / -10.0f)));
		if (i < 10)
		{
			rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(36.0f * i + (count / 10.0f)));
		}
		
		m_worldBall = transmat * rotmatZ;

		m_model_ball[i]->Draw(m_d3dContext.Get(), *m_states, m_worldBall, m_view, m_proj);
	}*/
	//ポットの描画
	/*for (int i = 0; i < POT_NUM; i++)
	{
		m_model_pot[i]->Draw(m_d3dContext.Get(), *m_states,m_worldPot[i], m_view, m_proj);
	}*/

	//自機の描画
	m_model_head->Draw(m_d3dContext.Get(), *m_states, m_head_world, m_view, m_proj);
	m_model_head->Draw(m_d3dContext.Get(), *m_states, m_head_world2, m_view, m_proj);

	m_batch->Begin();
	m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	/*m_batch->DrawLine(VertexPositionColor(Vector3(0, 0, 0), Color(1, 1, 1)), VertexPositionColor(Vector3(100, 100, 0), Color(1, 1, 1)));

	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);*/

	/*VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Yellow);
	VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Yellow);
	VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Yellow);*/

	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->End();

    Present();

	
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
//デバイスを作っている
//デバイス＝DirectXでの諸機能で使うもの
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
//描画先のキャンバスを作っている(イメージ)
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}