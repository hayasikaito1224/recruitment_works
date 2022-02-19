//----------------------------------------------
//ゲージの処理
//Author::林海斗
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
	void ResetGauge();//ゲージの増減

	void SetGauge(float Set);//ゲージの増減
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	int GetGaugeValue() { return m_fValue; }
	void SetGaugeValue(int nValue) { m_fValue = nValue; }

private:
	D3DXVECTOR3			m_Scale;		//ゲージの大きさ
	D3DXCOLOR m_Initcol;//初期の色
	float				m_fMaxGauge;	//gaugeの最大数
	float				m_fValueMax;	//ゲージの値の最大数
	float				m_fValue;		//ゲージの数値

};

#endif // !_TIME_H_*/#pragma once
