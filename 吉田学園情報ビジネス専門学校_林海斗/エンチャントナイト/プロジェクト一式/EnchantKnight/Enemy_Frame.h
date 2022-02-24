#ifndef _ENEMY_FRAME_H_
#define _ENEMY_FRAME_H_

#include "Enemy.h"

//#define MAX_ENEMY_FRAME_PARTS (10)	//�ő�p�[�c��
//#define MAX_KEYSET (10) //�ő�L�[�Z�b�g��
//#define MAX_DRAWRADIUS (800.0f)
//#define MAX_COLRADIUS (400.0f)
//#define MAX_TEMPTATION_RADIUS (150.0f)//�U�f�˒��͈�
//#define ENEMY_FRAME_ADD_SPEED (0.2)//�Q�[�W�𑝂₷��
//#define INVINCIBLE_TIME (30)//���G����
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
	void AddLife(int nLife);//�̗͂̑���
	//�ÓI�����o�[�֐�
	static CEnemy_Frame *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	bool m_bDiscovery;//�G���v���C���[�𔭌�������
	int m_nTimer;
	int m_nBreakTime;//�U���������Ƃ̃N�[���^�C��
	bool m_bBreakTime;//�U���������Ƃ̃N�[���^�C��

};


#endif // !_PLAYER_H_

