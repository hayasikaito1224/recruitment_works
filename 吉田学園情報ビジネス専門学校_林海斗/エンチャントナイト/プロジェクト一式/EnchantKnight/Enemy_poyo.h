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
	 void AIAttack();//�����ōU�����鏈��
	 void AIMove();
	 void AddLife(int nPower, int nType);
	 void AddLife(int nLife);//�̗͂̑���
	//�ÓI�����o�[�֐�
	static CEnemy_Poyo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
};


#endif // !_PLAYER_H_

