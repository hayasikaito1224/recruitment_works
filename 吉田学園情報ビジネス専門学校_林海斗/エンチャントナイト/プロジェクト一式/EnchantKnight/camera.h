//----------------------------------
//ポリゴンの処理
//----------------------------------
#ifndef _CAMERA_H
#define _CAMERA_H
#include "main.h"
#include "scene.h"
#include "texture.h"
#define MAX_PLAYER_RANGE (400.0f)//プレイヤーからの距離
#define PLAYER_CAMERA_ANGLE_OF_VIEW (80.0f)//プレイヤーの画角
#define MAP_ANGLE_OF_VIEW (10.0f)//マップの画角
#define MAP_RANGE (6000.0f)//マップの画角

class CPlayer;
//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCamera(void);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	float GetLong(void) { return m_fLong; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosV(D3DXVECTOR3 pos_v) { m_posV = pos_v; }//現在の視点
	void SetPosR(D3DXVECTOR3 pos_r) { m_posR = pos_r; }//現在の注視点
	void SetLong(float flong) { m_fLong += flong; }
	void SetPlayerCamera(D3DXVECTOR3 pos);
	void SetMapCamera(D3DXVECTOR3 pos);
	void SetAngleView(float fAngleView) {m_fAngleView = fAngleView;}
	HRESULT SetViewport(DWORD Xpos, DWORD Ypos, DWORD Width, DWORD Height,int nCnt);
	D3DVIEWPORT9 GetViewport(void) { return m_Viewport; }
private:
	D3DXVECTOR3 m_posV;//現在の視点
	D3DXVECTOR3 m_posR;//現在の注視点
	D3DXVECTOR3 m_vecU;//上方向ベクトル
	D3DXVECTOR3 m_rot;//カメラの向き
	float m_fLong;//長さ
	D3DXMATRIX m_mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;//ビューマトリックス
	POINT m_Cursol;
	D3DVIEWPORT9 m_Viewport;
	float m_fAngleView;//画角

};


#endif // !_CAMERA_H
#pragma once
