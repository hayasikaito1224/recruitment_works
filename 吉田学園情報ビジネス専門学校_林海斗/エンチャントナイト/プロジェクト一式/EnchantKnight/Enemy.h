#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "main.h"

class CSound;
class CMap_Polygon;
class CShadow;

#define MAX_KEYSET (10) //最大キーセット数
#define MAX_DRAWRADIUS (1000.0f)
#define MAX_COLRADIUS (400.0f)
#define MAX_ATTACKSTART_RADIUS (150.0f)//攻撃開始範囲
#define ENEMY_ADD_SPEED (0.2)//ゲージを増やす量
#define INVINCIBLE_TIME (30)//無敵時間
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
		NONE =0,
		FIRE,
		BULLIZAD,
	}MAGIC;
	CEnemy(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	virtual~CEnemy();
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void Colision();
	virtual void AIAttack();
	virtual void AIMove();
	virtual void AddLife(int nPower, int nType);
	virtual void AddLife(int nLife);//体力の増減

	bool bColision();
	//静的メンバー関数
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void AIBehavior();//敵AIの挙動の処理
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
	int GetLife(void) { return m_nLife; }//体力の増減
	void SetbDamage(bool bDamage) { m_bDamage = bDamage; }//ダメージを受けた状態にする
	bool GetbDamage(void) { return m_bDamage; }//ダメージを受けている状態か取得
	bool GetAIMove() { return m_bAIMove; }
	bool GetAIMoveStop() { return m_bAIMoveStop; }
	void SetHit(const bool& bHit) { m_bHit = bHit; }
	bool bHitAttack();//攻撃が当たってもいいかの判定
	bool GetHit() { return m_bHit; }
	bool GetbInvincible() { return m_bInvincible; }
	void Knockback(D3DXVECTOR3& Playerpos);
	void SetbHitCollision(bool bHitCollision) { m_bHitCollision = bHitCollision; }

private:
protected:
	std::random_device s_random;	// 非決定的な乱数生成器
	CMap_Polygon		*m_pMapPolygon;
	CShadow *m_pShadow;//影
	bool  m_bKnockback;//ノックバックするかどうかの判定

	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	bool m_bUninit;
	bool m_bOnMissile;
	bool m_bDraw;
	bool m_bHit;//攻撃を受けたかどうか
	int  m_nTimer;
	float	m_fMovingRange;//敵が自動で動く距離
	int	m_nDefense;
	bool	m_bNeutral;
	bool	m_bMotionStop;//モーションが止まったかどうか
	bool	m_bDamage;//今攻撃を受けているか
	bool	m_bInvincible;//無敵時間の判定
	bool    m_bEffect;

	bool m_bHitCollision;//プレイヤーからの攻撃が当たるようにする判定

	D3DXVECTOR3 m_MoveSpeed; //位置
	float m_fAng;

	float s_CntTimer;//カウンター
	float s_MaxTimer;//カウンター
	float s_MovingCnt;//カウンター
	float s_MoveRandAng;//ランダムな向き
	float m_fWalkSoundInterval;

	bool  s_bCntStop;//ランダムな向き
	bool	m_bAIMoveStop;//敵が追従するときにAI動作を止める判定

	int	 m_nCntHitInterval;//プレイヤーからの攻撃が当たれるようになるまでカウンター
	float m_fSoundInterval;
	bool	m_bAttack;//今攻撃してるかどうか
	int		m_nAttackStartCnt;//攻撃開始までのカウント
	bool	m_bAIMove;	//敵が自動的に動く判定
	bool m_bMove;//移動判定
	D3DXVECTOR3 m_pos, m_rot, m_lastPos, m_Lotrot;
	CMotion				*m_pMotion;
	vector<CModel*>		m_pPartsModel;
	float m_fPlayerVecLength;
	int	 m_MotionType,	m_MotionLastType;
	bool	m_bDeth;//死ぬ判定
	vector<CModel*>		m_pAttackModel;//攻撃判定用のモデル
	int					m_nPower;//攻撃力
	int					m_nLife;			//ライフ
	int	m_nType;//敵の種類
};


#endif // !_PLAYER_H_

