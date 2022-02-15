//-----------------------------------------
//プレイヤーの操作
//-----------------------------------------
#include "main.h"
#define PLAYER_MOVE_SPEED (6.0f)//移動量
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
	void Attack(const D3DXVECTOR3& pos, float& frotY, int& nComboType,bool& bAttack,bool& bAttackNext, const bool& bNearEnemy);		//プレイヤーの攻撃
	void Gravity(D3DXVECTOR3& pos);		//重力
	void CameraControl(const D3DXVECTOR3 pos);
	void Rotate(float& rot_y, float fAngle);//かくどを指定してその方向に向かせる関数
	int MagicAttack(D3DXVECTOR3 & pos, float& frotY,int& nMotionType ,bool& bMagic,const bool& bNearEnemy,const int& nMagicType);
	void DrawRockOn(D3DXVECTOR3& pos);//ロックオン用の画像の描画
	void Jump(float& fGravity,bool& bJump);
	void Gravity(D3DXVECTOR3& pos, float& fGravity,const float& fGravitySpeed, bool& bJump);		//重力
	bool RockOn(D3DXVECTOR3& pos, D3DXVECTOR3& rot);//ロックオン

private:
	CEnemy *m_pNearEnemy;
	bool	m_bCommandMagic;//まほうコマンドを選択したかの判定
	void LengthNearEnemy(D3DXVECTOR3 & pos);//一番近い敵を求める
	void RotNearEnemy();//一番プレイヤーが向いている方向に近い敵を求める
	bool	m_bRockOn;												//現在ロックオンしているかの判定
	D3DXVECTOR3 m_NearEnemyVec;
	float m_rot_y, m_fAngle, m_fAngleScece,m_LastAngle;
	bool	m_bJumpLock;											//ジャンプの制限
	bool	m_bPorpose;
	bool	m_bCrossBoLock;
	bool	m_bCrossUpLock;
	bool	m_bCrossDownLock;
	bool m_bPush;//ボタンを押したか
	float	m_fGravity;												//重力
	float m_fSoundInterval;
	int		m_nCntJump;												//ジャンプのカウント
	float	m_fGravitySpeed;										//重力
	int		m_nCommandType;//現在選択しているコマンドのタイプ
	int		m_nMagicCommandType;//現在選択しているコマンドのタイプ
};

