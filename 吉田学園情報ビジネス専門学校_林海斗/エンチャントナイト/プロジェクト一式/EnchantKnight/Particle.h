#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "texture.h"
class CParticle
{
public:
	typedef struct
	{
		D3DXVECTOR3	size, move, CircleRot;					//大きさ/移動量
		D3DXCOLOR	col;//色
		int			nEffectStack;//エフェクトを重ねる数
		float		fSpeed;
		float		fGravity;
		int			nMaxParticle;//円のエフェクトに使う一度に出す最大のパーティクルの数
		float		fDefSpeedColorA;
		bool		bGravity;

	}ParticleState;
	typedef enum
	{
		RANDOMPARTICLE = 0,
		CIRCLEPARTICLE,
		MAGICPARTICLE,//散るエフェクト
		RANDOMMAGICPARTICLE,
		CIRCLEMOVE,
		MAGICCIRCLE,
		HEELEFFECT,//回復魔法の処理
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
	D3DXVECTOR3				m_size, m_move, m_pos, m_rot, m_CircleRot;					//大きさ/移動量
	D3DXMATRIX				m_mtxWorld;				//ワールドマトリックス
	D3DXCOLOR				m_col;//色
	int						m_nMax_Y, m_nMax_X;
	int						m_nType;//パーティクルのタイプ
	int						m_nEffectStack;//エフェクトを重ねる数
	int						m_nCntPushTime;//キーを押した時間
	int						m_nNumParticle;//いまパーティクルを重ねている数
	int						m_nPosVolumeType;//数を増やす量のタイプ
	int						m_nMaxParticle;//円のエフェクトに使う一度に出す最大のパーティクルの数
	float					m_fCircleMove;
	float					m_fCircleMoveSpeed;
	bool					m_bPush;//キーの押下判定
	bool					m_bGravity;
	bool					m_bShiftLock;
	bool					m_bAltLock;
	bool					m_bCtrlLock;
	bool					m_bMagicCircleSwitch;
	bool					m_bHeelEffect;//回復魔法エフェクトを行うか
	float					m_fDefSpeedColorA;
	float					m_fVolume;//数を増やす量
	float					m_fSpeed;
	float					m_fAng;//角度
	float					m_fPosAng;
	float					m_fCircleAng;
	float					m_fGravity;
	float					m_fRadius;
	int						m_nTime;
	int						m_nMaxTime;
	static ParticleState			m_State[MaxParticle];

};


#endif // !_PARTICLE_H_

