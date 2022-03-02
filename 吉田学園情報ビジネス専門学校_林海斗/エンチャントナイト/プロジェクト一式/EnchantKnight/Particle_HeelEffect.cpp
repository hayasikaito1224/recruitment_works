//=====================================
//回復魔法エフェクトの処理
//=====================================
#include "Particle_HeelEffect.h"
#include "effect.h"
#include "MagicCircle.h"
#include "effectring.h"
//=========================================
//コンストラクタ
//=========================================
CParticle_HeelEffect::CParticle_HeelEffect()
{
	m_fRadius = 50.0f;
	m_nEffectMaxTime = 30;
	m_bStop = false;
	m_nEffectTime = 0;
}

//=========================================
//デストラクタ
//=========================================
CParticle_HeelEffect::~CParticle_HeelEffect()
{

}

//-----------------------------------
//回復魔法エフェクト
//-----------------------------------
void CParticle_HeelEffect::PlayHeelEffect(D3DXVECTOR3 pos)
{
	//HEELEFFECTのステータスを取得
	ParticleState state = m_State[HEELEFFECT];

	std::random_device random;	// 非決定的な乱数生成器
	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);
	std::uniform_real_distribution<> randMagicCircle(0.0f, m_fRadius);

	m_nEffectTime++;
	if (m_bStop == false)
	{
		//魔法陣を生成
		CMagicCircle::Create(pos, { D3DXToRadian(90.0f),0.0f,0.0f }, m_fRadius, m_nEffectMaxTime, false, true, state.col, CTexture::MagicCircle_TypeB);
		for (int nStack = 0; nStack <state.nEffectStack; nStack++)
		{
			//ringエフェクトを生成
			CEffect_Ring::Create(pos, state.col, state.fDefSpeedColorA, state.fSpeed, CTexture::RingEffect);
		}

		//魔法陣の処理を使ってエフェクト
		CMagicCircle::Create(pos, { D3DXToRadian(90.0f),0.0f,0.0f }, m_fRadius, m_nEffectMaxTime, false, true, state.col, CTexture::Effect);

		m_bStop = true;

	}
	if (m_nEffectTime >= m_nEffectMaxTime)
	{
		m_nEffectTime = 0;
		m_bHeel = true;
		PlayRandomCircle({ pos.x, pos.y + 50.0f , pos.z }, HEELEFFECT);
		for (int nStack = 0; nStack < state.nEffectStack; nStack++)
		{
			CEffect_Ring::Create(pos, state.col, state.fDefSpeedColorA, state.fSpeed, CTexture::RingEffect);
		}

		m_bStop = false;
	}

	if (m_nEffectTime % 20 == 0)
	{
	}
	else if (m_nEffectTime % 6 == 0)
	{
		D3DXVECTOR3 posEffect;
		float fAng = randAng(mt);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
			randSpeed(mt),
			0.0f);
		posEffect = D3DXVECTOR3(
			cosf(fAng)*randMagicCircle(mt) + pos.x,
			pos.y,
			sinf(fAng)*randMagicCircle(mt) + pos.z
		);

		CEffect::Create(posEffect, move, state.size, state.col, false, state.fGravity, state.fDefSpeedColorA,true, CTexture::LifeEffect);

	}

}
