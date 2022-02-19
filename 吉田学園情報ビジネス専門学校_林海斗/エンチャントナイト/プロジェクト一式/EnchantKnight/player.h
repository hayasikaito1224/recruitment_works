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
//マクロ定義
//------------------------------------
#define PLAYER_POS_X (0.0)
#define PLAYER_POS_Y (0.0)
#define PLAYER_POS_Z (0.0)
#define PLAYER_FALL (-100.0)//落下判定に入るまでの位置
#define PLAYER_ROCK_LENGTH (500.0)//落下判定に入るまでの位置

class CPlayer : public CScene
{
public:
	typedef struct
	{
		bool m_bMagic;//魔法判定
		bool m_bRetryMagic;//魔法を連続して出す
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
		N_MAGIC_ATTACK,//魔法攻撃
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
	//静的メンバー関数
	static CPlayer *Create();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Drawtext();
	void Dodge();//回避の処理
	void PlayerMagic();//魔法に関する処理
	void PlayerAttack();//攻撃に関する処理
	void NearEnemySearch(D3DXVECTOR3 Enemy);
	void SetState(int nState) { m_State = (STATE)nState; }//プレイヤーの状態の設定
	void PlayerHit();//プレイヤーが敵の攻撃に当たった時
	void SetHit(bool bHit) { m_bHit = bHit; }//プレイヤーが敵の攻撃に当たった時
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
	STATE m_State;//プレイヤーの状態

	CPlayer_Controller	*m_pController;//操作系のクラス
	CMap_Polygon		*m_pMapPolygon;
	CSwordEffect		*m_pSwordLocus;//剣の軌跡
	D3DXVECTOR3 m_pos,m_lastpos,m_Swordpos[2],m_LastSwordpos[2]; //位置
	float m_Vec_x, m_Vec_y, m_Vec_z;
	float m_fLength;
	float m_move = 2.0f;
	D3DXVECTOR3 m_rot;												//向き
	D3DXVECTOR3 m_DodgePos;//回避した先
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	CModel *m_pModel[MAX_PARTS];
	CModel *m_pCollision;									//当たり判定用の武器の原点を決めるやつ
	CModel *m_pSwordEffect[2];									//当たり判定用の武器の原点を決めるやつ
	Magic   m_Magic;
	CShadow *pShadow;
	CMotion*m_pMotion[TYPE_MAX];
	CBillboard *m_pRockOnPolygon;
	CLayer_Structure *m_pLayer;
	bool m_bGameStop;
	bool	m_bDeth;												//死んだかの判定
	bool	m_bDodge;//回避の判定
	bool	m_bBeginDodge;
	bool	m_bEndDodge;

	bool	m_bland;												//地面についてるか判定
	bool	m_bColloison;											//当たり判定
	bool	m_bNeutral;
	bool	m_bAttack;												//今攻撃してるかどうか
	bool	m_bAttackNext;											//次の攻撃に入るための判定
	bool	m_bMotionStop;											//モーションが止まったかどうか
	bool	m_bFall;												//落下判定
	bool	m_bMove;												// 移動判定
	bool	m_bDelay;												//次の処理に行くための間隔
	bool	m_bMagicShot;											//魔法を今打てるかの判定
	bool	m_bNearEnemy;											//敵と近いかの判定
	bool	m_bSelectMagic;											//魔法が選択されているかの判定
	bool	m_bDraw;
	bool	m_bHit;//今敵の攻撃が当たっているのかのヒット判定
	bool	m_bHitStop;//敵の攻撃を当たらないようにする判定
	bool	m_bSkill;//スキル演出中の動作
	bool m_bSearchStop;
	bool m_bLockOn;
	float	m_fMagicShotDelayCnt;									//魔法が出るまでのカウント
	float	m_fMagicShotDelayMax;									//魔法が出る最大の時間
	float	m_fSoundInterval;										//サウンドインターバル
	int		m_nDelayTimer;											//遅延カウント
	int		m_nMaxDelatTime;										//遅延させる時間
	int		m_nLife;												//ライフ
	int		m_nMP;													//マジックポイント
	int		m_nWeaponType;											//武器のタイプ
	int		m_nMotionType[TYPE_MAX],m_nMotionLastType[TYPE_MAX];
	int		m_nAttackType[TYPE_MAX];								//今の攻撃タイプ
	int     m_nTimer;
	int		m_nEffectTimer;
	int		m_nMagicPoint;											//魔法を打つためのポイント
	int		m_nCommandType;											//コマンドを選択しているタイプ
	int		m_nMagicCommandType;											//コマンドを選択しているタイプ
	int		m_nHitTime;
	float  m_fDodgeAddSpeed;//回避中の増加量
	float m_fDodgeTimer;
	float m_fDodgerot;//回避する方向
};


#endif // !_PLAYER_H_
