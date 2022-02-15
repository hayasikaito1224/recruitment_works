#ifndef _ENEMY_POYO_H_
#define _ENEMY_POYO_H_

#include "Enemy.h"

//#define MAX_ENEMY_POYO_PARTS (10)	//最大パーツ数
//#define MAX_KEYSET (10) //最大キーセット数
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//誘惑射程範囲
//#define ENEMY_POYO_ADD_SPEED (0.2)//ゲージを増やす量
//#define INVINCIBLE_TIME (30)//無敵時間
class CEnemy_Poyo : public CEnemy
{
public:
	typedef enum
	{
		NEUTRAL = 0,
		MOVE,
		ATTACK,
		MAX
	}MOTION;
	typedef enum
	{
		FIRE = 0,
		BULLIZAD,
	}MAGIC;
	CEnemy_Poyo(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CEnemy_Poyo();
	 HRESULT Init();
	 void Uninit();
	 void Update();
	 void Draw();
	//静的メンバー関数
	static CEnemy_Poyo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
};


#endif // !_PLAYER_H_

