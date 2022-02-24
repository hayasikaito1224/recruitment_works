//===============================================-
//�����@�̃N���X
//===============================================
#ifndef _ENEMY_MAGIC_FIRE_H_
#define _ENEMY_MAGIC_FIRE_H_
#include "magic.h"

//�O���錾
class CModel;
class CPlayer;
class CShadow;

class C_Enemy_Magic_Fire : public C_Magic
{
public:
	C_Enemy_Magic_Fire(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Enemy_Magic_Fire();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Enemy_Magic_Fire *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const bool bBezier = false,
		const float Shotrot = 0.0f);
	void BezierCurve();//�x�W�F�Ȑ��̏���
private:
	CShadow *m_pShadow;//�e

	CModel *m_pFireModel;//�t�@�C�A�[�{�[���p�̃|���S��.
	D3DXVECTOR3 ModelRot;
	D3DXMATRIX m_mtxWorld;											//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_StartingPoint;//�n�_
	D3DXVECTOR3 m_EndPoint;//�I�_
	D3DXVECTOR3 m_ControlPoint;//����_
	float  m_nTime;//�n�_����I�_�܂Ői�ގ���
	float  m_nTimeAddSpeed;//�n�_����I�_�܂Ői�ގ���
	CPlayer *m_pPlayer;
	bool m_bAttack;
	bool m_bBezier;//�x�W�F�Ȑ��̃I���I�t(true=�I��)
};
#endif // !_ENEMY_MAGIC_FIRE_H_
