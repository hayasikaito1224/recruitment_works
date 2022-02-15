#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "main.h"

class CSound;
class CMap_Polygon;

#define MAX_KEYSET (10) //�ő�L�[�Z�b�g��
#define MAX_DRAWRADIUS (1000.0f)
#define MAX_COLRADIUS (400.0f)
#define MAX_ATTACKSTART_RADIUS (150.0f)//�U���J�n�͈�
#define ENEMY_ADD_SPEED (0.2)//�Q�[�W�𑝂₷��
#define INVINCIBLE_TIME (30)//���G����
class CEnemy : public CScene
{
public:
	typedef enum
	{
		NEUTRAL = 0,
		MOVE,
		ATTACKWAIT,
		ATTACK,
		DETH,
		MAX
	}MOTION;
	typedef enum
	{
		FIRE = 0,
		BULLIZAD,
	}MAGIC;
	CEnemy(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	virtual~CEnemy();
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	bool bColision();
	//�ÓI�����o�[�֐�
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void AIMove();//�GAI�̋����̏���
	CModel *GetModel(int nParts) {
		if (m_bUninit == false)
		{
			return m_pPartsModel[nParts];
		}
		return nullptr;
	}
	vector<CModel*> GetModel() { return m_pPartsModel; }
	CMotion *GetMotion() { return m_pMotion; }
	D3DXVECTOR3 GetEnemyPos(void) { return m_pos; }
	void SetUninit(bool Uninit) { m_bUninit = Uninit; }
	bool GetUninit(void) { return m_bUninit; }
	void AddLife(int nLife);//�̗͂̑���
	int GetLife(void) { return m_nLife; }//�̗͂̑���
	void SetbDamage(bool bDamage) { m_bDamage = bDamage; }//�_���[�W���󂯂���Ԃɂ���
	bool GetbDamage(void) { return m_bDamage; }//�_���[�W���󂯂Ă����Ԃ��擾
	bool GetAIMove() { return m_bAIMove; }
	bool GetAIMoveStop() { return m_bAIMoveStop; }
	void SetHit(const bool& bHit) { m_bHit = bHit; }
	bool bHitAttack();//�U�����������Ă��������̔���
	bool GetHit() { return m_bHit; }
	bool GetbInvincible() { return m_bInvincible; }
	void Knockback(D3DXVECTOR3& Playerpos);
	void AddLife(int nPower, int nType);

private:
	std::random_device s_random;	// �񌈒�I�ȗ���������

	CMap_Polygon		*m_pMapPolygon;

	float s_CntTimer;//�J�E���^�[
	float s_MaxTimer;//�J�E���^�[
	float s_MovingCnt;//�J�E���^�[
	float s_MoveRandAng;//�����_���Ȍ���
	bool  s_bCntStop;//�����_���Ȍ���
	bool  m_bKnockback;//�m�b�N�o�b�N���邩�ǂ����̔���
	int		m_nAttackStartCnt;//�U���J�n�܂ł̃J�E���g

	D3DXVECTOR3 m_MoveSpeed; //�ʒu
	D3DXMATRIX m_mtxWorld; //���[���h�}�g���b�N�X
	bool m_bUninit;
	bool m_bOnMissile;
	bool m_bDraw;
	bool m_bHit;//�U�����󂯂����ǂ���
	float m_fAng;
	int  m_nTimer;
	int	 m_nCntHitInterval;//�v���C���[����̍U�����������悤�ɂȂ�܂ŃJ�E���^�[
	float m_fSoundInterval;
	float m_fWalkSoundInterval;
	float	m_fMovingRange;//�G�������œ�������
	int	m_nDefense;
	bool	m_bNeutral;
	bool	m_bAttack;//���U�����Ă邩�ǂ���
	bool	m_bMotionStop;//���[�V�������~�܂������ǂ���
	bool	m_bDamage;//���U�����󂯂Ă��邩
	bool	m_bInvincible;//���G���Ԃ̔���
	bool    m_bEffect;
	bool	m_bAIMove;	//�G�������I�ɓ�������
	bool	m_bAIMoveStop;//�G���Ǐ]����Ƃ���AI������~�߂锻��
protected:
	D3DXVECTOR3 m_pos, m_rot, m_lastPos, m_Lotrot;
	CMotion				*m_pMotion;
	vector<CModel*>		m_pPartsModel;
	int	 m_MotionType,	m_MotionLastType;
	bool	m_bDeth;//���ʔ���
	vector<CModel*>		m_pAttackModel;//�U������p�̃��f��
	int					m_nPower;//�U����
	int					m_nLife;			//���C�t

};


#endif // !_PLAYER_H_

