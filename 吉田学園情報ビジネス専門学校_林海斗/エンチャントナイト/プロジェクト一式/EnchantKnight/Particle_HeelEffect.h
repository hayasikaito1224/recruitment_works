//=====================================
//�񕜖��@�G�t�F�N�g�̏���
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
	�ʒu�̐ݒ�,
	�ړ���,
	�T�C�Y,
	�������Ȃ�X�s�[�h
	Texture
	*/

private:
	int						m_nEffectTime;//���o�p�̃^�C�}�[
	int						m_nEffectMaxTime;//���o�p�̃^�C�}�[
	bool					m_bStop;
	bool					m_bHeel;//�񕜂��s���^�C�~���O��m�点��
	float					m_fRadius;
	//��ŏ���

};


#endif // !_PARTICLE_HEELEFFECT_H_

