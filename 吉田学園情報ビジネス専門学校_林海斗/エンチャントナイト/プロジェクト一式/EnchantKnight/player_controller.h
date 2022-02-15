//-----------------------------------------
//�v���C���[�̑���
//-----------------------------------------
#include "main.h"
#define PLAYER_MOVE_SPEED (6.0f)//�ړ���
class CCamera;
class CEnemy;

class CPlayer_Controller
{
public:
	typedef enum
	{
		NOWN = 0,
		COMBO_1,
		COMBO_2,
		COMBO_3,
		COMBO_MAX
	}COMBO;
	CPlayer_Controller();
	~CPlayer_Controller();
	void Drawtext();

	int CommandSelect();
	int MagicCommandSelect();

	bool bPorpose();
	bool GetMagicCommandSelect() { return m_bCommandMagic; }
	CEnemy *GetNearEnemy() { 
		if (m_pNearEnemy != nullptr)
		{
			return m_pNearEnemy;
		}
		return nullptr;
	}

	void TestMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot);
	void TestPadMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot, bool& bMove);
	void SkillPadMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot, bool& bMove, const bool& bNearEnemy);
	void PlayerFace(D3DXVECTOR3& pos, float& frotY, const bool& bNearEnemy);
	void Attack(const D3DXVECTOR3& pos, float& frotY, int& nComboType,bool& bAttack,bool& bAttackNext, const bool& bNearEnemy);		//�v���C���[�̍U��
	void Gravity(D3DXVECTOR3& pos);		//�d��
	void CameraControl(const D3DXVECTOR3 pos);
	void Rotate(float& rot_y, float fAngle);//�����ǂ��w�肵�Ă��̕����Ɍ�������֐�
	int MagicAttack(D3DXVECTOR3 & pos, float& frotY,int& nMotionType ,bool& bMagic,const bool& bNearEnemy,const int& nMagicType);
	void DrawRockOn(D3DXVECTOR3& pos);//���b�N�I���p�̉摜�̕`��
	void Jump(float& fGravity,bool& bJump);
	void Gravity(D3DXVECTOR3& pos, float& fGravity,const float& fGravitySpeed, bool& bJump);		//�d��
	bool RockOn(D3DXVECTOR3& pos, D3DXVECTOR3& rot);//���b�N�I��

private:
	CEnemy *m_pNearEnemy;
	bool	m_bCommandMagic;//�܂ق��R�}���h��I���������̔���
	void LengthNearEnemy(D3DXVECTOR3 & pos);//��ԋ߂��G�����߂�
	void RotNearEnemy();//��ԃv���C���[�������Ă�������ɋ߂��G�����߂�
	bool	m_bRockOn;												//���݃��b�N�I�����Ă��邩�̔���
	D3DXVECTOR3 m_NearEnemyVec;
	float m_rot_y, m_fAngle, m_fAngleScece,m_LastAngle;
	bool	m_bJumpLock;											//�W�����v�̐���
	bool	m_bPorpose;
	bool	m_bCrossBoLock;
	bool	m_bCrossUpLock;
	bool	m_bCrossDownLock;
	bool m_bPush;//�{�^������������
	float	m_fGravity;												//�d��
	float m_fSoundInterval;
	int		m_nCntJump;												//�W�����v�̃J�E���g
	float	m_fGravitySpeed;										//�d��
	int		m_nCommandType;//���ݑI�����Ă���R�}���h�̃^�C�v
	int		m_nMagicCommandType;//���ݑI�����Ă���R�}���h�̃^�C�v
};

