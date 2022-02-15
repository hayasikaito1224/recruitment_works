//===============================================-
//���@�Ɋւ���x�[�X�N���X
//===============================================
#ifndef _MAGIC_H_
#define _MAGIC_H_
#include "main.h"
#include "scene.h"
class C_Magic : public CScene
{
public:
	C_Magic(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic();

	virtual HRESULT Init()=0;
	virtual void Uninit()=0;
	virtual void Update()=0;
	virtual void Draw()=0;

	int Sub(int& nMP) { return nMP - m_ConsumingMP; }//���͂̏����(�v���C���[�������Ă��閂�̓|�C���g�������ɓ����)

private:
protected:
	D3DXVECTOR3 m_pos, m_move, m_col,m_rot,m_MoveSpeed;
	bool		m_bUninit;//��������
	int			m_nDeleteTimer;
	int			m_ConsumingMP;//�e���@�̑������ƂɎ����Ă��閂�͂̏���|�C���g
	int			m_nPower;//�U����

};
#endif // !_MAGIC_H_
