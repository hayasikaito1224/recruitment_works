//----------------------------------------------
//Q[WÌ
//Author::ÑCl
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
	void ResetGauge();//Q[WÌ¸

	void SetGauge(float Set);//Q[WÌ¸
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	int GetGaugeValue() { return m_fValue; }
	void SetGaugeValue(int nValue) { m_fValue = nValue; }

private:
	D3DXVECTOR3			m_Scale;		//Q[WÌå«³
	D3DXCOLOR m_Initcol;//úÌF
	float				m_fMaxGauge;	//gaugeÌÅå
	float				m_fValueMax;	//Q[WÌlÌÅå
	float				m_fValue;		//Q[WÌl

};

#endif // !_TIME_H_*/#pragma once
