//=====================================
//回復魔法エフェクトの処理
//=====================================
#ifndef _PARTICLE_HEELEFFECT_H_
#define _PARTICLE_HEELEFFECT_H_
#include "Particle.h"

class CParticle_HeelEffect : public CParticle
{
public:
	CParticle_HeelEffect();
	~CParticle_HeelEffect();
	void PlayHeelEffect(D3DXVECTOR3 pos);
	bool GetbHeel() { return m_bHeel; }
	/*
	位置の設定,
	移動量,
	サイズ,
	小さくなるスピード
	Texture
	*/

private:
	int						m_nEffectTime;//演出用のタイマー
	int						m_nEffectMaxTime;//演出用のタイマー
	bool					m_bStop;
	bool					m_bHeel;//回復を行うタイミングを知らせる
	float					m_fRadius;
	//後で消す

};


#endif // !_PARTICLE_HEELEFFECT_H_

