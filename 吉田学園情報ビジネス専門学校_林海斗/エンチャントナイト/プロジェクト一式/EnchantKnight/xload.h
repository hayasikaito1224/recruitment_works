//----------------------------------
//���f���̏���
//�쐬�ҁF�с@�C�l
//----------------------------------
#ifndef _XLOAD_H
#define _XLOAD_H
#include "main.h"
#include "scene.h"
#include "texture.h"
#define MAX_XLOAD_TEX (10)
#define MAX_XLOAD_VTX (8)						//���f���̒��_��

//------------------------------------------------------------------------------
//�N���X�̒�`
//------------------------------------------------------------------------------
class CXload
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}ModelVtx;//���f���̊e���_�̕ۑ��p�\����
	typedef struct
	{
		LPD3DXMESH				m_pMesh;						//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER			m_pBuffMat;						//�}�e���A���i�ގ����j�ւ̃|�C���^
		DWORD					m_nNumMat;						//�}�e���A���̐�
		D3DXVECTOR3				m_vtxMin, m_vtxMax;				//���f���̍ŏ��l�A�ő�l
		std::vector<LPDIRECT3DTEXTURE9>		m_pTexture;		//�e�N�X�`��
		int						m_nNumVtx;						//���_�̐�
		DWORD					m_sizeFVF;						//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE					*m_pVtxBuff;					//���_�o�b�t�@�̃|�C���^
		ModelVtx				m_vtx[MAX_XLOAD_VTX];			//�e���_�̏��i�[�\����

	}ModelData;
	CXload();
	~CXload();
	void Init(void);
	void Uninit(void);
	static void Load(void);
	static ModelData GetPlayerModelData(int nType) { return m_PlayerModelData[nType]; }
	static ModelData GetEnemy01ModelData(int nType) { return m_Enemy01ModelData[nType]; }

	static ModelData GetObjectData(int nType){ return m_ObjectModelData[nType]; }
private:
	static vector<ModelData>		m_PlayerModelData;//���f�����̍\����
	static vector<ModelData>		m_Enemy01ModelData;//���f�����̍\����

	static vector<ModelData>		m_ObjectModelData;//���f�����̍\����

};


#endif // !_XLOAD_H
