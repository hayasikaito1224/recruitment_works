//----------------------------------------------
//�Q�[�W�̏���
//Author::�ъC�l
//--------------------------------------------

#ifndef _GAUGE_H_
#define _GAUGE_H_
#include "scene.h"
#include "Scene2D.h"
#include "texture.h"



class CGauge : public CScene2D
{
public:
	typedef enum
	{
		L_ADD=0,//�����Ɍ������ăQ�[�W��������
		R_ADD,//�E���Ɍ������ăQ�[�W��������
	}ADDTYPE;//�Q�[�W�̑�����

	CGauge(OBJTYPE nPriority = OBJTYPE_UI);
	~CGauge();
	static CGauge*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 scale, const D3DXCOLOR col, const float MaxGauge, const int nValue, const ADDTYPE type);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetGauge();//�Q�[�W�̑���

	void SetGauge(float Set);//�Q�[�W�̑���
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale,ADDTYPE AddType);
	int GetGaugeValue() { return m_fValue; }
	void SetGaugeValue(int nValue) { m_fValue = nValue; }

private:
	D3DXVECTOR3			m_Scale;		//�Q�[�W�̑傫��
	ADDTYPE				m_AddType;		//�Q�[�W�̑�����^�C�v
	float				m_fMaxGauge;	//gauge�̍ő吔
	float				m_fValueMax;	//�Q�[�W�̒l�̍ő吔
	float				m_fValue;		//�Q�[�W�̐��l

};

#endif // !_TIME_H_*/#pragma once
