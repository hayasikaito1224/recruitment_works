//----------------------------------
//�|���S���̏���
//----------------------------------
#ifndef _CAMERA_H
#define _CAMERA_H
#include "main.h"
#include "scene.h"
#include "texture.h"
#define MAX_PLAYER_RANGE (400.0f)//�v���C���[����̋���
#define PLAYER_CAMERA_ANGLE_OF_VIEW (80.0f)//�v���C���[�̉�p
#define MAP_ANGLE_OF_VIEW (10.0f)//�}�b�v�̉�p
#define MAP_RANGE (6000.0f)//�}�b�v�̉�p

class CPlayer;
//------------------------------------------------------------------------------
//�N���X�̒�`
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
	void SetPosV(D3DXVECTOR3 pos_v) { m_posV = pos_v; }//���݂̎��_
	void SetPosR(D3DXVECTOR3 pos_r) { m_posR = pos_r; }//���݂̒����_
	void SetLong(float flong) { m_fLong += flong; }
	void SetPlayerCamera(D3DXVECTOR3 pos);
	void SetMapCamera(D3DXVECTOR3 pos);
	void SetAngleView(float fAngleView) {m_fAngleView = fAngleView;}
	HRESULT SetViewport(DWORD Xpos, DWORD Ypos, DWORD Width, DWORD Height,int nCnt);
	D3DVIEWPORT9 GetViewport(void) { return m_Viewport; }
private:
	D3DXVECTOR3 m_posV;//���݂̎��_
	D3DXVECTOR3 m_posR;//���݂̒����_
	D3DXVECTOR3 m_vecU;//������x�N�g��
	D3DXVECTOR3 m_rot;//�J�����̌���
	float m_fLong;//����
	D3DXMATRIX m_mtxProjection;//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;//�r���[�}�g���b�N�X
	POINT m_Cursol;
	D3DVIEWPORT9 m_Viewport;
	float m_fAngleView;//��p

};


#endif // !_CAMERA_H
#pragma once
