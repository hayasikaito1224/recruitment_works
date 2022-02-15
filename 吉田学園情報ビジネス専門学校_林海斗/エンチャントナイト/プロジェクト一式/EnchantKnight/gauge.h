//----------------------------------------------
//ゲージの処理
//Author::林海斗
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
		L_ADD=0,//左側に向かってゲージが増える
		R_ADD,//右側に向かってゲージが増える
	}ADDTYPE;//ゲージの増え方

	CGauge(OBJTYPE nPriority = OBJTYPE_UI);
	~CGauge();
	static CGauge*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 scale, const D3DXCOLOR col, const float MaxGauge, const int nValue, const ADDTYPE type);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetGauge();//ゲージの増減

	void SetGauge(float Set);//ゲージの増減
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale,ADDTYPE AddType);
	int GetGaugeValue() { return m_fValue; }
	void SetGaugeValue(int nValue) { m_fValue = nValue; }

private:
	D3DXVECTOR3			m_Scale;		//ゲージの大きさ
	ADDTYPE				m_AddType;		//ゲージの増えるタイプ
	float				m_fMaxGauge;	//gaugeの最大数
	float				m_fValueMax;	//ゲージの値の最大数
	float				m_fValue;		//ゲージの数値

};

#endif // !_TIME_H_*/#pragma once
