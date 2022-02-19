//===============================================-
//CP����X�L���i�t���[�W���O�V���b�g�j�̏���
//===============================================
#ifndef _MAGIC_SKILL01_H_
#define _MAGIC_SKILL01_H_
#include "magic.h"
#define MAGIC_SKILL01_MP (20)//MP�����

//class C_Magic_Ice;

class CModel;
class CMagicCircle;
class C_Magic_Skill01 : public C_Magic
{
public:
	C_Magic_Skill01(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Skill01();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Skill01 *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:
	int m_nTimer, m_nMaxTime;//�V���b�g�Ԋu�n
	D3DXMATRIX m_mtxWorld;											//���[���h�}�g���b�N�X
	bool m_bAttack;
	D3DXVECTOR3 ModelRot;
	CMagicCircle *m_pMagicCircle;
	//C_Magic_Ice *m_pIce;

};
#endif // !_MAGIC_SKILL01_H_
