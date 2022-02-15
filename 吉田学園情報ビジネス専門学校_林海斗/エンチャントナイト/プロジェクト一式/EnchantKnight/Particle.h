#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "texture.h"
class CParticle
{
public:
	typedef struct
	{
		D3DXVECTOR3	size, move, CircleRot;					//�傫��/�ړ���
		D3DXCOLOR	col;//�F
		int			nEffectStack;//�G�t�F�N�g���d�˂鐔
		float		fSpeed;
		float		fGravity;
		int			nMaxParticle;//�~�̃G�t�F�N�g�Ɏg����x�ɏo���ő�̃p�[�e�B�N���̐�
		float		fDefSpeedColorA;
		bool		bGravity;

	}ParticleState;
	typedef enum
	{
		RANDOMPARTICLE = 0,
		CIRCLEPARTICLE,
		MAGICPARTICLE,//�U��G�t�F�N�g
		RANDOMMAGICPARTICLE,
		CIRCLEMOVE,
		MAGICCIRCLE,
		HEELEFFECT,//�񕜖��@�̏���
		MaxParticle
	}ParticleType;

	CParticle();
	~CParticle();
	void PlayCircleParticle(bool bFixedRadius, float fRadius, D3DXCOLOR col);
	void RandomCircleParticle(D3DXVECTOR3 pos, D3DXCOLOR col, bool bStop);
	void PlayRandomCircle(D3DXVECTOR3 pos,int nType,int nTexType = (int)CTexture::GlitterEffect);
	static ParticleState GetState(int nType) { return m_State[nType]; }
	static void Load(const char* cFileName);
private:

protected:
	D3DXVECTOR3				m_size, m_move, m_pos, m_rot, m_CircleRot;					//�傫��/�ړ���
	D3DXMATRIX				m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXCOLOR				m_col;//�F
	int						m_nMax_Y, m_nMax_X;
	int						m_nType;//�p�[�e�B�N���̃^�C�v
	int						m_nEffectStack;//�G�t�F�N�g���d�˂鐔
	int						m_nCntPushTime;//�L�[������������
	int						m_nNumParticle;//���܃p�[�e�B�N�����d�˂Ă��鐔
	int						m_nPosVolumeType;//���𑝂₷�ʂ̃^�C�v
	int						m_nMaxParticle;//�~�̃G�t�F�N�g�Ɏg����x�ɏo���ő�̃p�[�e�B�N���̐�
	float					m_fCircleMove;
	float					m_fCircleMoveSpeed;
	bool					m_bPush;//�L�[�̉�������
	bool					m_bGravity;
	bool					m_bShiftLock;
	bool					m_bAltLock;
	bool					m_bCtrlLock;
	bool					m_bMagicCircleSwitch;
	bool					m_bHeelEffect;//�񕜖��@�G�t�F�N�g���s����
	float					m_fDefSpeedColorA;
	float					m_fVolume;//���𑝂₷��
	float					m_fSpeed;
	float					m_fAng;//�p�x
	float					m_fPosAng;
	float					m_fCircleAng;
	float					m_fGravity;
	float					m_fRadius;
	int						m_nTime;
	int						m_nMaxTime;
	static ParticleState			m_State[MaxParticle];

};


#endif // !_PARTICLE_H_

