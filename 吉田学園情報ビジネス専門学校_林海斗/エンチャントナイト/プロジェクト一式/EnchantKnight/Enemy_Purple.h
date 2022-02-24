#ifndef _ENEMY_PURPLE_H_
#define _ENEMY_PURPLE_H_

#include "Enemy.h"

//#define MAX_ENEMY_PURPLE_PARTS (10)	//最大パーツ数
//#define MAX_KEYSET (10) //最大キーセット数
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//誘惑射程範囲
//#define ENEMY_PURPLE_ADD_SPEED (0.2)//ゲージを増やす量
//#define INVINCIBLE_TIME (30)//無敵時間
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
		int nCnt;//火の玉を打つ数のカウント
		int nNumCnt;//火の玉を打つ数のカウント
		int nAttackCnt;//火の玉を打つまでの時間のカウント
		float fMoveAng;
		bool bAttackStart;
		bool bEffectStop;
		bool bAttackEffectStop;
	}FireState;
	typedef struct
	{
		int nAttackCnt;//火の玉を打つまでの時間のカウント
		int nEndCnt;//攻撃が終わるまでのカウント
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
	void FireAttack();//火属性の攻撃
	void IceAttack();//氷属性
	void AttackWait();//攻撃待機
	void AIMove();
	void Discovery();//敵がプレイヤーを発見したときの処理
	void AddLife(int nLife);//体力の増減
	//静的メンバー関数
	static CEnemy_Purple *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife);
private:
	D3DXCOLOR m_col;//エフェクトの色
	bool m_bAttackStart;
	bool m_bDiscovery;//敵がプレイヤーを発見したか
	int m_nTimer;
	int m_nBreakTime;//攻撃したあとのクールタイム
	bool m_bBreakTime;//攻撃したあとのクールタイム
	bool m_bAttackWait;//待機中かどうか(オンだと待機中)
	int m_nAttackType;//現在の攻撃の種類
	FireState m_FireState;
	IceState m_IceState;

};


#endif // !_PLAYER_H_

