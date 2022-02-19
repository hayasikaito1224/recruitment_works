//===============================================-
//CP����X�L���i�t���[�W���O�V���b�g�j�̏���
//===============================================
#ifndef _MAGIC_SKILL02_H_
#define _MAGIC_SKILL02_H_
#include "magic.h"
#define MAGIC_SKILL02_MP (20)//MP�����
#define SKILL02_COLLISIONVTX (4)

//class C_Magic_Ice;

class CModel;
class CCol_Area;
class C_Magic_Skill02 : public C_Magic
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}Vtx;//�e���_�̕ۑ��p�\����

	C_Magic_Skill02(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Skill02();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Skill02 *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const float fSpeed);
	bool Collision(D3DXVECTOR3 *pPos,float fRadius);
private:
	int m_nTimer, m_nMaxTime;//�V���b�g�Ԋu�n
	D3DXMATRIX m_mtxWorld;											//���[���h�}�g���b�N�X
	bool m_bAttack;
	D3DXVECTOR3 ModelRot;
	int m_nCnt;//���o���Ă���X�̉�̌�
	float m_fSpeed;//�����o�鑬�x
	Vtx m_vtx[SKILL02_COLLISIONVTX];
	int m_nAttackTimer;
	CCol_Area *m_pArea;
	//C_Magic_Ice *m_pIce;

};
#endif // !_MAGIC_SKILL02_H_
