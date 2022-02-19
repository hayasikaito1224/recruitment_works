#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "scene.h"
#include "model.h"
#include "motion.h"
class CSwordEffect;
class CLayer_Structure;
class CShadow;
class CPlayer_Controller;
class CMap_Polygon;
class CBillboard;
class CGauge;
class CPolygon;
//------------------------------------
//�}�N����`
//------------------------------------
#define PLAYER_POS_X (0.0)
#define PLAYER_POS_Y (0.0)
#define PLAYER_POS_Z (0.0)
#define PLAYER_FALL (-100.0)//��������ɓ���܂ł̈ʒu
#define PLAYER_ROCK_LENGTH (500.0)//��������ɓ���܂ł̈ʒu

class CPlayer : public CScene
{
public:
	typedef struct
	{
		bool m_bMagic;//���@����
		bool m_bRetryMagic;//���@��A�����ďo��
	}Magic;

	typedef enum 
	{
		TYPE_SWORD=0,
		TYPE_HAMMER,
		TYPE_MAX
	}WEAPON_TYPE;

	typedef enum 
	{
		N_NEUTRAL=0,
		N_MOVE,
		N_ATTACK_1,
		N_ATTACK_2,
		N_ATTACK_3,
		N_MAGIC_ATTACK,//���@�U��
		N_SKILL002,
		N_DETH,
		N_DODGE,
		N_MAX
	}N_MOTION;
	typedef enum 
	{
		STATE_NOWN = 0,
		STATE_MAGIC,
	}STATE;

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	//�ÓI�����o�[�֐�
	static CPlayer *Create();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Drawtext();
	void Dodge();//����̏���
	void PlayerMagic();//���@�Ɋւ��鏈��
	void PlayerAttack();//�U���Ɋւ��鏈��
	void NearEnemySearch(D3DXVECTOR3 Enemy);
	void SetState(int nState) { m_State = (STATE)nState; }//�v���C���[�̏�Ԃ̐ݒ�
	void PlayerHit();//�v���C���[���G�̍U���ɓ���������
	void SetHit(bool bHit) { m_bHit = bHit; }//�v���C���[���G�̍U���ɓ���������
	void SetbAttack(bool bAttack) { m_bAttack = bAttack; }
	void SetbSkill(bool bSkill) { m_bSkill = bSkill; }
	void SetDeth(bool bDeth) { m_bDeth = bDeth; }
	void SetGameStop(bool bStop) { m_bGameStop = bStop; }
	void SetPlayerPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMotionType(int nType) { m_nMotionType[0] = nType; }
	void SetbMove(bool bMove) {}
	int GetWeaponType(void) { return m_nWeaponType; }
	bool bColision();
	bool GetCommandMagic() { return m_bSelectMagic; }
	bool GetHit() { return m_bHitStop; }
	bool GetGameStop() { return m_bGameStop; }
	bool GetDeth() { return m_bDeth; }
	bool GetDodge() { return m_bDodge; }
	D3DXVECTOR3 GetLastPos(void) { return m_lastpos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	CBillboard *GetNearEnemyPos() { return m_pRockOnPolygon; }
	CModel *GetParts(int nNumParts) { return m_pModel[nNumParts]; }

private:
	STATE m_State;//�v���C���[�̏��

	CPlayer_Controller	*m_pController;//����n�̃N���X
	CMap_Polygon		*m_pMapPolygon;
	CSwordEffect		*m_pSwordLocus;//���̋O��
	D3DXVECTOR3 m_pos,m_lastpos,m_Swordpos[2],m_LastSwordpos[2]; //�ʒu
	float m_Vec_x, m_Vec_y, m_Vec_z;
	float m_fLength;
	float m_move = 2.0f;
	D3DXVECTOR3 m_rot;												//����
	D3DXVECTOR3 m_DodgePos;//���������
	D3DXMATRIX m_mtxWorld;											//���[���h�}�g���b�N�X
	CModel *m_pModel[MAX_PARTS];
	CModel *m_pCollision;									//�����蔻��p�̕���̌��_�����߂���
	CModel *m_pSwordEffect[2];									//�����蔻��p�̕���̌��_�����߂���
	Magic   m_Magic;
	CShadow *pShadow;
	CMotion*m_pMotion[TYPE_MAX];
	CBillboard *m_pRockOnPolygon;
	CLayer_Structure *m_pLayer;
	bool m_bGameStop;
	bool	m_bDeth;												//���񂾂��̔���
	bool	m_bDodge;//����̔���
	bool	m_bBeginDodge;
	bool	m_bEndDodge;

	bool	m_bland;												//�n�ʂɂ��Ă邩����
	bool	m_bColloison;											//�����蔻��
	bool	m_bNeutral;
	bool	m_bAttack;												//���U�����Ă邩�ǂ���
	bool	m_bAttackNext;											//���̍U���ɓ��邽�߂̔���
	bool	m_bMotionStop;											//���[�V�������~�܂������ǂ���
	bool	m_bFall;												//��������
	bool	m_bMove;												// �ړ�����
	bool	m_bDelay;												//���̏����ɍs�����߂̊Ԋu
	bool	m_bMagicShot;											//���@�����łĂ邩�̔���
	bool	m_bNearEnemy;											//�G�Ƌ߂����̔���
	bool	m_bSelectMagic;											//���@���I������Ă��邩�̔���
	bool	m_bDraw;
	bool	m_bHit;//���G�̍U�����������Ă���̂��̃q�b�g����
	bool	m_bHitStop;//�G�̍U���𓖂���Ȃ��悤�ɂ��锻��
	bool	m_bSkill;//�X�L�����o���̓���
	bool m_bSearchStop;
	bool m_bLockOn;
	float	m_fMagicShotDelayCnt;									//���@���o��܂ł̃J�E���g
	float	m_fMagicShotDelayMax;									//���@���o��ő�̎���
	float	m_fSoundInterval;										//�T�E���h�C���^�[�o��
	int		m_nDelayTimer;											//�x���J�E���g
	int		m_nMaxDelatTime;										//�x�������鎞��
	int		m_nLife;												//���C�t
	int		m_nMP;													//�}�W�b�N�|�C���g
	int		m_nWeaponType;											//����̃^�C�v
	int		m_nMotionType[TYPE_MAX],m_nMotionLastType[TYPE_MAX];
	int		m_nAttackType[TYPE_MAX];								//���̍U���^�C�v
	int     m_nTimer;
	int		m_nEffectTimer;
	int		m_nMagicPoint;											//���@��ł��߂̃|�C���g
	int		m_nCommandType;											//�R�}���h��I�����Ă���^�C�v
	int		m_nMagicCommandType;											//�R�}���h��I�����Ă���^�C�v
	int		m_nHitTime;
	float  m_fDodgeAddSpeed;//��𒆂̑�����
	float m_fDodgeTimer;
	float m_fDodgerot;//����������
};


#endif // !_PLAYER_H_
