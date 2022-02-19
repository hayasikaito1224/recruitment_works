#ifndef _ENEMY_POYO_H_
#define _ENEMY_POYO_H_

#include "Enemy.h"


class CEnemy_Poyo : public CEnemy
{
public:
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
	 void Colision();
	 void AIAttack();//自動で攻撃する処理
	 void AIMove();
	//静的メンバー関数
	static CEnemy_Poyo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
};


#endif // !_PLAYER_H_

