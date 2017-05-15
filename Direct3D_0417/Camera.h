/// <summary>
/// カメラを制御するクラス
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

	//ビュー行列
	DirectX::SimpleMath::Matrix  m_view;
	//射影行列
	DirectX::SimpleMath::Matrix  m_proj;

	//カメラの位置(視点座標)
	DirectX::SimpleMath::Vector3 m_eyepos;
	//カメラが見ている位置(注視点/参照点/注目点)
	DirectX::SimpleMath::Vector3 m_refpos;
	//カメラの上方向ベクトル(単位ベクトルのみ)(ななめの時は正規化する)
	DirectX::SimpleMath::Vector3 m_upvec;
	
	//垂直方向視野角
	float m_forY;
	//アスペクト比
	float m_aspect_ratio;
	//ニアクリップ
	float m_near_clip;
	//ファークリップ
	float m_far_clip;
	
};
