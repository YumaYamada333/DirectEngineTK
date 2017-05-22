/// <summary>
/// ３Dオブジェクトのクラス
/// </summary>

#include "Obj3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//静的メンバ変数の定義
//デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3D::m_d3dDevice;
//デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>    Obj3D::m_d3dContext;
//汎用ステート設定
std::unique_ptr<DirectX::CommonStates> Obj3D::m_states;
//カメラ
Camera* Obj3D::m_camera;
//エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3D::m_factory;

//コンストラクタ
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
	m_factory->SetDirectory(L"Resources");	//テクスチャのパスを指定(フォルダ名を指定)
}

//モデルの読み込み
void Obj3D::LoadModel(const wchar_t* filename)
{
	m_model= Model::CreateFromCMO(m_d3dDevice.Get(), filename, *m_factory);
}

//更新
void Obj3D::Update()
{

}

//描画
void Obj3D::Render()
{
	m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_camera->GetViewMatrix(), m_camera->GetProjMatrix());
}

//setter, getter
//スケーリング(XYZ)
void Obj3D::SetScale(DirectX::SimpleMath::Vector3 scale)
{
	m_scale = scale;
}
DirectX::SimpleMath::Vector3 Obj3D::GetScale()
{
	return m_scale;
}
//回転角(XYZ)
void Obj3D::SetRotate(DirectX::SimpleMath::Vector3 rotate)
{
	m_angle = rotate;
}
DirectX::SimpleMath::Vector3 Obj3D::GetRotate()
{
	return m_angle;
}
//平行移動(XYZ)
void Obj3D::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_pos = position;
}
DirectX::SimpleMath::Vector3 Obj3D::GetPosition()
{
	return m_pos;
}
//ワールド行列
DirectX::SimpleMath::Matrix Obj3D::GetWorld()
{
	return m_world;
}
//親のObj3D
void Obj3D::SetParent(Obj3D* parent)
{
	m_obj_parent = parent;
}