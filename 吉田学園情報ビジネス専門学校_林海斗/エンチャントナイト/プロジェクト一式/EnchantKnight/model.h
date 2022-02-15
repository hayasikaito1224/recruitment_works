//----------------------------------
//���f���̏���
//�쐬�ҁF�с@�C�l
//----------------------------------
#ifndef _MODEL_H
#define _MODEL_H
#include "main.h"
#include "scene.h"
#include "texture.h"
#include "xload.h"

#define MAX_MODEL_TEX (10)
#define MAX_MODEL_VTX (8)						//���f���̒��_��
//------------------------------------------------------------------------------
//�N���X�̒�`
//------------------------------------------------------------------------------
class CModel
{
public:
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_OBJECT,
		TYPE_MAX
	}CREATE_TYPE;
	CModel();
	~CModel();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,const int& nModelType,const int& nCreateType);//�t�@�C���l�[���̓t�@�C���̃p�X������
	void SetParent(CModel *pModel) {m_pParent = pModel;}//�e�̃��f������
	D3DXMATRIX GetMatrix(void) {return m_mtxWorld;}//���[���h�}�g���b�N�X�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	//void SetLayerPos(D3DXVECTOR3 pos) { m_layerpos = pos; }
	void SetLayerRot(D3DXVECTOR3 rot) { m_layerrot = rot; }
	D3DXVECTOR3 GetLayerPos() { return m_layerpos; }
	D3DXVECTOR3 GetLayerRot() { return m_layerrot; }
	void SetDiffuse(float DiffuseA);
	D3DXMATERIAL *GetMat() { return m_pSaveMat; }
	void SetNumParent(int Parent) { m_nNumParent = Parent; }
	int GetNumParent() { return m_nNumParent; }
	CXload::ModelVtx GetVtx(int nNumVtx) { return m_vtx[nNumVtx]; }//�e���f���̒��_���̎擾
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }//���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetMaxPos() { return m_pModelDataX.m_vtxMin; }
	D3DXVECTOR3 GetMinPos() { return m_pModelDataX.m_vtxMax; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	void SetSword(bool bSword) {m_bSwordEffect = bSword;}
private:
	CXload::ModelData		m_pModelDataX;
	D3DXVECTOR3				m_pos,m_layerpos,m_scale;				//���f���̈ʒu�i�I�t�Z�b�g�j
	D3DXVECTOR3				m_rot, m_layerrot;				//����
	CXload::ModelVtx		m_vtx[MAX_MODEL_VTX];			//�e���_�̏��i�[�\����
	
	D3DXMATRIX				m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXMATERIAL			*m_pMat;						//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL			*m_pSaveMat;					//�}�e���A���ւ̃|�C���^�̃Z�[�u
	int						m_nNumParent;					//���݂̐e���f���̔ԍ�
	CModel					*m_pParent;						//�e���f���̃|�C���^
	char					m_sFailName[256];
	bool					m_bDraw;						//�`�悷�邩
	bool					m_bSwordEffect;
	int						m_nModelType;
	int						m_nCreateType;
};


#endif // !_MODEL_H
