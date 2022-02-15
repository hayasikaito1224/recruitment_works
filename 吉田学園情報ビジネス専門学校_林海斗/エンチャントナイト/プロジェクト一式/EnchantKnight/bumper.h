//-------------------------------
//bumper�̃N���X�̏���
//�쐬�ҁF�ъC�l
//-------------------------------
#ifndef _BUMPER_H_
#define _BUMPER_H_
#include "scene.h"
class CModel_Spawner;

class CBumper : public CScene
{
public:
	CBumper(OBJTYPE nPriority = CScene::OBJTYPE_MODEL);
	~CBumper();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CBumper *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Knockback(D3DXVECTOR3& Playerpos, const D3DXVECTOR3& PlayerVec);//�v���C���[�Ƀm�b�N�o�b�N�����鏈��

private:
	CModel_Spawner *pModelSpawner;//�o���p�[�̃��f��
	D3DXVECTOR3	m_pos, m_rot;//����֌W
	float m_fKnockBackSpeed;//�m�b�N�o�b�N�̃X�s�[�h
	bool m_bKnockBack;//�m�b�N�o�b�N���邩�̔���
};
#endif