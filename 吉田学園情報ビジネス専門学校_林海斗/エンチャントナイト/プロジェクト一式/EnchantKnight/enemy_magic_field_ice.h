//===============================================-
//CP����X�L���i�t���[�W���O�V���b�g�j�̏���
//===============================================
#ifndef _ENEMY_MAGIC_FIELD_ICE_H_
#define _ENEMY_MAGIC_FIELD_ICE_H_
#include "magic.h"
#define ENEMY_MAGIC_FIELD_ICE_MP (20)//MP�����
#define SKILL02_COLLISIONVTX (4)
#define ENEMY_MAGIC_FIELD_ICE_SIZE_Z (30)//�X�̃T�C�Y
#define ENEMY_MAGIC_FIELD_ICE_SIZE_X (30)//�X�̃T�C�Y

//class C_Magic_Ice;

class CModel;
class CCol_Area;
class C_Enemy_Magic_Field_Ice : public C_Magic
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}Vtx;//�e���_�̕ۑ��p�\����

	C_Enemy_Magic_Field_Ice(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Enemy_Magic_Field_Ice();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Enemy_Magic_Field_Ice *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const float fSpeed,const int nMaxCnt);
	bool Collision(D3DXVECTOR3 *pPos,float fRadius);
private:
	int m_nTimer, m_nMaxTime;//�V���b�g�Ԋu�n
	D3DXMATRIX m_mtxWorld;											//���[���h�}�g���b�N�X
	bool m_bAttack;
	D3DXVECTOR3 ModelRot;
	int m_nCnt;//���o���Ă���X�̉�̌�
	int m_nMaxCnt;//�o���X�̉�̍ő吔
	float m_fSpeed;//�����o�鑬�x
	Vtx m_vtx[SKILL02_COLLISIONVTX];
	int m_nAttackTimer;
	CCol_Area *m_pArea;
	//C_Magic_Ice *m_pIce;

};
#endif // !_ENEMY_MAGIC_FIELD_ICE_H_
