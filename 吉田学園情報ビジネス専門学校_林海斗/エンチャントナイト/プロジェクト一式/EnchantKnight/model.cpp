//----------------------------------
//�|���S���̏���
//�쐬�ҁ@�ъC�l
//-----------------------------------
#include "model.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
#include "xload.h"
#include "swordeffect.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	memset(m_vtx, NULL, sizeof(m_vtx));
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					//���f���̈ʒu�i�I�t�Z�b�g�j
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//����
	m_bDraw = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{
}
//=============================================================================
// �|���S���̏���������
//=============================================================================
void CModel::Init()
{
	CXload *pModelDataX = CManager::GetXload();
	CXload::ModelData ModelData;
	switch (m_nCreateType)
	{
	case TYPE_PLAYER:
		m_pModelDataX = pModelDataX->GetPlayerModelData(m_nModelType);
		break;
	case TYPE_ENEMY:
		m_pModelDataX = pModelDataX->GetEnemy01ModelData(m_nModelType);
		break;
	case TYPE_OBJECT:
		m_pModelDataX = pModelDataX->GetObjectData(m_nModelType);
		break;
	case TYPE_FRAMEENEMY:
		m_pModelDataX = pModelDataX->GetEnemy02ModelData(m_nModelType);
		break;
	case TYPE_CRYSTALENEMY:
		m_pModelDataX = pModelDataX->GetEnemy03ModelData(m_nModelType);
		break;

	}

	for (int nVtx = 0; nVtx < MAX_MODEL_VTX; nVtx++)
	{
		m_vtx[nVtx] = m_pModelDataX.m_vtx[nVtx];
	}

}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CModel::Uninit(void)
{
	memset(&m_pModelDataX, NULL, sizeof(m_pModelDataX));
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CModel::Update(void)
{


}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^

	//���݂̃}�e���A�����擾
	D3DXMATRIX mtxRotModel, mtxTransModel,mtxScale;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X
	D3DMATERIAL9 Matdef;
						 //�e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	////�T�C�Y�ύX
	//D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	////�����𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);
	pDevice->GetMaterial(&Matdef);

	//�����p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//�ŐV�̃}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	if (m_bDraw == true)
	{

		//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//�}�e���A���f�[�^�҂̃|�C���^���擾
		m_pMat = (D3DXMATERIAL*)m_pModelDataX.m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModelDataX.m_nNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pModelDataX.m_pTexture[nCntMat]);
			//���f���i�p�[�c�j�̕`��
			m_pModelDataX.m_pMesh->DrawSubset(nCntMat);


		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&Matdef);

	}
}
//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CModel::SetDiffuse(float DiffuseA)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
																	 //�}�e���A���f�[�^�҂̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pModelDataX.m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelDataX.m_nNumMat; nCntMat++)
	{
		m_pMat[nCntMat].MatD3D.Diffuse.a = DiffuseA;
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);
	}

}
//---------------------------------------------------------------
//�C���X�^���X��������
//---------------------------------------------------------------
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int& nModelType,const int& nCreateType)
{
	//�C���X�^���X����
	CModel *pModel = new CModel;
	if (pModel != NULL)
	{
		pModel->m_nModelType = nModelType;
		pModel->m_nCreateType = nCreateType;
		pModel->Init();
	}
	pModel->m_pos = pos;
	pModel->m_rot = rot;
	pModel->m_layerpos = pos;
	pModel->m_layerrot = rot;


	return pModel;
}
