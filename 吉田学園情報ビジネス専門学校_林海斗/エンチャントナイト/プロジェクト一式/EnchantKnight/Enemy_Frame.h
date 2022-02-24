#ifndef _ENEMY_FRAME_H_
#define _ENEMY_FRAME_H_

#include "Enemy.h"

//#define MAX_ENEMY_FRAME_PARTS (10)	//最大パーツ数
//#define MAX_KEYSET (10) //最大キーセット数
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//誘惑射程範囲
//#define ENEMY_FRAME_ADD_SPEED (0.2)//ゲージを増やす量
//#define INVINCIBLE_TIME (30)//無敵時間
class CEnemy_Frame : public CEnemy
{
public:
	typedef enum
	{
		FIRE = 0,
		BULLIZAD,
	}MAGIC;
	CEnemy_Frame(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CEnemy_Frame();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Colision();
	void AIAttack();
	void AIMove();
	void AddLife(int nPower, int nType);
	void AddLife(int nLife);//体力の増減
	//静的メンバー関数
	static CEnemy_Frame *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	bool m_bDiscovery;//敵がプレイヤーを発見したか
	int m_nTimer;
	int m_nBreakTime;//攻撃したあとのクールタイム
	bool m_bBreakTime;//攻撃したあとのクールタイム

};


#endif // !_PLAYER_H_

