#ifndef _ENEMY_POYO_H_
#define _ENEMY_POYO_H_

#include "Enemy.h"

//#define MAX_ENEMY_POYO_PARTS (10)	//�ő�p�[�c��
//#define MAX_KEYSET (10) //�ő�L�[�Z�b�g��
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//�U�f�˒��͈�
//#define ENEMY_POYO_ADD_SPEED (0.2)//�Q�[�W�𑝂₷��
//#define INVINCIBLE_TIME (30)//���G����
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
	//�ÓI�����o�[�֐�
	static CEnemy_Poyo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
};


#endif // !_PLAYER_H_

