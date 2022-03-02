//=====================================
//�񕜖��@�G�t�F�N�g�̏���
//=====================================
#include "Particle_HeelEffect.h"
#include "effect.h"
#include "MagicCircle.h"
#include "effectring.h"
//=========================================
//�R���X�g���N�^
//=========================================
CParticle_HeelEffect::CParticle_HeelEffect()
{
	m_fRadius = 50.0f;
	m_nEffectMaxTime = 30;
	m_bStop = false;
	m_nEffectTime = 0;
}

//=========================================
//�f�X�g���N�^
//=========================================
CParticle_HeelEffect::~CParticle_HeelEffect()
{

}

//-----------------------------------
//�񕜖��@�G�t�F�N�g
//-----------------------------------
void CParticle_HeelEffect::PlayHeelEffect(D3DXVECTOR3 pos)
{
	//HEELEFFECT�̃X�e�[�^�X���擾
	ParticleState state = m_State[HEELEFFECT];

	std::random_device random;	// �񌈒�I�ȗ���������
	std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
	std::uniform_real_distribution<> randSpeed(state.fSpeed / 2, state.fSpeed);
	std::uniform_real_distribution<> randMagicCircle(0.0f, m_fRadius);

	m_nEffectTime++;
	if (m_bStop == false)
	{
		//���@�w�𐶐�
		CMagicCircle::Create(pos, { D3DXToRadian(90.0f),0.0f,0.0f }, m_fRadius, m_nEffectMaxTime, false, true, state.col, CTexture::MagicCircle_TypeB);
		for (int nStack = 0; nStack <state.nEffectStack; nStack++)
		{
			//ring�G�t�F�N�g�𐶐�
			CEffect_Ring::Create(pos, state.col, state.fDefSpeedColorA, state.fSpeed, CTexture::RingEffect);
		}

		//���@�w�̏������g���ăG�t�F�N�g
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
