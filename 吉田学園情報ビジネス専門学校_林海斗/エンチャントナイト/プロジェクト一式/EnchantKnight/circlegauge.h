//----------------------------------------------
//�Q�[�W�̏���
//Author::�ъC�l
//--------------------------------------------
#ifndef _CIRCLEGAUGE_H_
#define _CIRCLEGAUGE_H_
#include "scene.h"
#include "Scene2D.h"
#include "texture.h"

class CCircleGauge : public CScene2D
{
public:

	CCircleGauge(OBJTYPE nPriority = OBJTYPE_UI);
	~CCircleGauge();
	static CCircleGauge*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 scale, const D3DXCOLOR col,
		const float MaxCircleGauge, const int nValue);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetGauge();//�Q�[�W�̑���

	void SetGauge(float Set);//�Q�[�W�̑���
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	int GetGaugeValue() { return m_fValue; }
	void SetGaugeValue(int nValue) { m_fValue = nValue; }

private:
	D3DXVECTOR3			m_Scale;		//�Q�[�W�̑傫��
	D3DXCOLOR m_Initcol;//�����̐F
	float				m_fMaxGauge;	//gauge�̍ő吔
	float				m_fValueMax;	//�Q�[�W�̒l�̍ő吔
	float				m_fValue;		//�Q�[�W�̐��l

};

#endif // !_TIME_H_*/#pragma once
