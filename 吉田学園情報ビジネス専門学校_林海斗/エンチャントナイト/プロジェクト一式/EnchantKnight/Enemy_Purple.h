#ifndef _ENEMY_PURPLE_H_
#define _ENEMY_PURPLE_H_

#include "Enemy.h"

//#define MAX_ENEMY_PURPLE_PARTS (10)	//�ő�p�[�c��
//#define MAX_KEYSET (10) //�ő�L�[�Z�b�g��
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//�U�f�˒��͈�
//#define ENEMY_PURPLE_ADD_SPEED (0.2)//�Q�[�W�𑝂₷��
//#define INVINCIBLE_TIME (30)//���G����
class CEnemy_Purple : public CEnemy
{
public:
	typedef enum
	{
		FIRE = 0,
		BULLIZAD,
	}ATTACKTYPE;
	typedef struct 
	{
		int nCnt;//�΂̋ʂ�ł��̃J�E���g
		int nNumCnt;//�΂̋ʂ�ł��̃J�E���g
		int nAttackCnt;//�΂̋ʂ�ł܂ł̎��Ԃ̃J�E���g
		float fMoveAng;
		bool bAttackStart;
		bool bEffectStop;
		bool bAttackEffectStop;
	}FireState;
	typedef struct
	{
		int nAttackCnt;//�΂̋ʂ�ł܂ł̎��Ԃ̃J�E���g
		int nEndCnt;//�U�����I���܂ł̃J�E���g
		bool bAttackStart;
		bool bAttack;
		bool bEffectStop;

	}IceState;
	CEnemy_Purple(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CEnemy_Purple();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void AddLife(int nPower, int nType);
	void Colision();
	void AIAttack();
	void FireAttack();//�Α����̍U��
	void IceAttack();//�X����
	void AttackWait();//�U���ҋ@
	void AIMove();
	void Discovery();//�G���v���C���[�𔭌������Ƃ��̏���
	void AddLife(int nLife);//�̗͂̑���
	//�ÓI�����o�[�֐�
	static CEnemy_Purple *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife);
private:
	D3DXCOLOR m_col;//�G�t�F�N�g�̐F
	bool m_bAttackStart;
	bool m_bDiscovery;//�G���v���C���[�𔭌�������
	int m_nTimer;
	int m_nBreakTime;//�U���������Ƃ̃N�[���^�C��
	bool m_bBreakTime;//�U���������Ƃ̃N�[���^�C��
	bool m_bAttackWait;//�ҋ@�����ǂ���(�I�����Ƒҋ@��)
	int m_nAttackType;//���݂̍U���̎��
	FireState m_FireState;
	IceState m_IceState;

};


#endif // !_PLAYER_H_

