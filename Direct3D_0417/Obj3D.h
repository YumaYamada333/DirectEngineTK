/// <summary>
/// ３Dオブジェクトクラスのヘッダ
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Model.h>				//モデルを表示するためのヘッダ
#include <CommonStates.h>		//コモンステイトを使うためのヘッダ
#include <Effects.h>			//エフェクトを使うためのヘッダ
#include "ForrowCamera.h"		//カメラヘッダ

class Obj3D
{
public:
	//静的メンバ関数
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, 
		Camera* camera);

private:
	//静的メンバ変数
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>				m_d3dDevice;
	//デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
	//汎用ステート設定
	static std::unique_ptr<DirectX::CommonStates>			m_states;
	//カメラ
	static Camera*											m_camera;
	//エフェクトファクトリ
	static std::unique_ptr<DirectX::EffectFactory>			m_factory;

public:
	Obj3D();
	~Obj3D();

	//メンバ関数
	
	//モデルの読み込み
	void LoadModel(const wchar_t* filename);
	//更新
	void Update();
	//描画
	void Render();

	//setter, getter
	//スケーリング(XYZ)
	void SetScale(DirectX::SimpleMath::Vector3 scale);
	DirectX::SimpleMath::Vector3 GetScale();
	//回転角(XYZ)
	void SetRotate(DirectX::SimpleMath::Vector3 rotate);
	DirectX::SimpleMath::Vector3 GetRotate();
	//平行移動(XYZ)
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	DirectX::SimpleMath::Vector3 GetPosition();
	//ワールド行列
	DirectX::SimpleMath::Matrix GetWorld();
	//親のObj3D
	void SetParent(Obj3D* parent);

private:
	//メンバ変数

	//モデル
	std::unique_ptr<DirectX::Model> m_model;
	//スケーリング(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//回転角(XYZ)
	DirectX::SimpleMath::Vector3 m_angle;
	//平行移動(XYZ)
	DirectX::SimpleMath::Vector3 m_pos;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//親のObj3Dへのポインタ
	Obj3D* m_obj_parent;

};

