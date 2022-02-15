//----------------------------------
//�|���S���̏���
//�쐬�ҁ@�ъC�l
//-----------------------------------
#include "xload.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
std::vector<CXload::ModelData> CXload::m_PlayerModelData;
std::vector<CXload::ModelData> CXload::m_ObjectModelData;
std::vector<CXload::ModelData> CXload::m_Enemy01ModelData;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXload::CXload()
{
	m_PlayerModelData.clear();
	m_ObjectModelData.clear();
	m_Enemy01ModelData.clear();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXload::~CXload()
{
}
//=============================================================================
// �|���S���̏���������
//=============================================================================
void CXload::Init(void)
{
	//���f���f�[�^�̓ǂݍ���
	Load();
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CXload::Uninit(void)
{
}


//=============================================================================
// ���f����ǂݍ��ޏ���
//=============================================================================
void CXload::Load(void)
{
	char sString[2][255];	// �ǂݍ��ݗp�̕ϐ�
	int nCnt = 0;
	int nCnt2 = 0;
	int nCnt3 = 0;

	FILE *File = fopen("data/TEXT/AllModel.txt", "r");
	char sFileName[124];
	if (File != NULL)
	{
		while (true)
		{
			fscanf(File, "%s", &sString[0]);


			//�v���C���[���f���̃t�@�C���l�[����ǂݍ���
			if (strcmp(sString[0], "PLAYERMODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_PlayerModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);
				//���f�����̓ǂݍ���
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_PlayerModelData[nCnt].m_pBuffMat,
					NULL,
					&m_PlayerModelData[nCnt].m_nNumMat,
					&m_PlayerModelData[nCnt].m_pMesh);

				//�}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL *)m_PlayerModelData[nCnt].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_PlayerModelData[nCnt].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;
						//�e�N�X�`���̓ǂݍ���
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_PlayerModelData[nCnt].m_pTexture.push_back(Securement);
					}
					else
					{
						m_PlayerModelData[nCnt].m_pTexture.push_back(NULL);

					}
				}
				//���_�����擾
				m_PlayerModelData[nCnt].m_nNumVtx = m_PlayerModelData[nCnt].m_pMesh->GetNumVertices();
				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				m_PlayerModelData[nCnt].m_sizeFVF = D3DXGetFVFVertexSize(m_PlayerModelData[nCnt].m_pMesh->GetFVF());
				//���_�o�b�t�@���A�����b�N
				m_PlayerModelData[nCnt].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_PlayerModelData[nCnt].m_pVtxBuff);
				//���f���̑傫���𑪂�
				for (int nCnt1 = 0; nCnt1 < m_PlayerModelData[nCnt].m_nNumVtx; nCnt1++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_PlayerModelData[nCnt].m_pVtxBuff;//���_���W�̑��
																				  //x�̍ő�l�̔�r
					if (m_PlayerModelData[nCnt].m_vtxMax.x >= vtx.x)
					{
						m_PlayerModelData[nCnt].m_vtxMax.x = vtx.x;
					}
					//���̍ŏ��l�̔�r
					else if (m_PlayerModelData[nCnt].m_vtxMin.x <= vtx.x)
					{
						m_PlayerModelData[nCnt].m_vtxMin.x = vtx.x;
					}
					//y�̍ő�l�̔�r
					if (m_PlayerModelData[nCnt].m_vtxMax.y >= vtx.y)
					{
						m_PlayerModelData[nCnt].m_vtxMax.y = vtx.y;
					}
					//y�̍ŏ��l�̔�r
					else if (m_PlayerModelData[nCnt].m_vtxMin.y <= vtx.y)
					{
						m_PlayerModelData[nCnt].m_vtxMin.y = vtx.y;
					}
					//z�̍ő�l�̔�r
					if (m_PlayerModelData[nCnt].m_vtxMax.z >= vtx.z)
					{
						m_PlayerModelData[nCnt].m_vtxMax.z = vtx.z;
					}
					//z�̍ŏ��l�̔�r
					else if (m_PlayerModelData[nCnt].m_vtxMin.z <= vtx.z)
					{
						m_PlayerModelData[nCnt].m_vtxMin.z = vtx.z;
					}
					m_PlayerModelData[nCnt].m_pVtxBuff += m_PlayerModelData[nCnt].m_sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				}
				//���_�o�b�t�@���A�����b�N
				m_PlayerModelData[nCnt].m_pMesh->UnlockVertexBuffer();
				//8�̒��_���̕ۑ�
				//�����i��ʁj
				m_PlayerModelData[nCnt].m_vtx[0].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//�E���i��ʁj
				m_PlayerModelData[nCnt].m_vtx[1].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//���O�i��ʁj
				m_PlayerModelData[nCnt].m_vtx[2].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//�E�O�i��ʁj
				m_PlayerModelData[nCnt].m_vtx[3].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//�����i���ʁj
				m_PlayerModelData[nCnt].m_vtx[4].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//�E���i���ʁj
				m_PlayerModelData[nCnt].m_vtx[5].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//���O�i���ʁj
				m_PlayerModelData[nCnt].m_vtx[6].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//�E�O�i���ʁj
				m_PlayerModelData[nCnt].m_vtx[7].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				nCnt++;
			}
			//�G���f����ǂݍ���
			if (strcmp(sString[0], "ENEMY01MODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_Enemy01ModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);

				//���f�����̓ǂݍ���
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_Enemy01ModelData[nCnt3].m_pBuffMat,
					NULL,
					&m_Enemy01ModelData[nCnt3].m_nNumMat,
					&m_Enemy01ModelData[nCnt3].m_pMesh);

				//�}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL *)m_Enemy01ModelData[nCnt3].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_Enemy01ModelData[nCnt3].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;

						//�e�N�X�`���̓ǂݍ���
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_Enemy01ModelData[nCnt3].m_pTexture.push_back(Securement);

					}
					else
					{
						m_Enemy01ModelData[nCnt3].m_pTexture.push_back(NULL);
					}

				}
				//���_�����擾
				m_Enemy01ModelData[nCnt3].m_nNumVtx = m_Enemy01ModelData[nCnt3].m_pMesh->GetNumVertices();
				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				m_Enemy01ModelData[nCnt3].m_sizeFVF = D3DXGetFVFVertexSize(m_Enemy01ModelData[nCnt3].m_pMesh->GetFVF());
				//���_�o�b�t�@���A�����b�N
				m_Enemy01ModelData[nCnt3].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_Enemy01ModelData[nCnt3].m_pVtxBuff);
				//���f���̑傫���𑪂�
				for (int nCnt = 0; nCnt < m_Enemy01ModelData[nCnt3].m_nNumVtx; nCnt++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_Enemy01ModelData[nCnt3].m_pVtxBuff;//���_���W�̑��
																						 //x�̍ő�l�̔�r
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.x >= vtx.x)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.x = vtx.x;
					}
					//���̍ŏ��l�̔�r
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.x <= vtx.x)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.x = vtx.x;
					}
					//y�̍ő�l�̔�r
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.y >= vtx.y)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.y = vtx.y;
					}
					//y�̍ŏ��l�̔�r
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.y <= vtx.y)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.y = vtx.y;
					}
					//z�̍ő�l�̔�r
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.z >= vtx.z)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.z = vtx.z;
					}
					//z�̍ŏ��l�̔�r
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.z <= vtx.z)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.z = vtx.z;
					}
					m_Enemy01ModelData[nCnt3].m_pVtxBuff += m_Enemy01ModelData[nCnt3].m_sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				}
				//���_�o�b�t�@���A�����b�N
				m_Enemy01ModelData[nCnt3].m_pMesh->UnlockVertexBuffer();
				//8�̒��_���̕ۑ�
				//�����i��ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[0].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//�E���i��ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[1].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//���O�i��ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[2].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//�E�O�i��ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[3].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//�����i���ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[4].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//�E���i���ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[5].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//���O�i���ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[6].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//�E�O�i���ʁj
				m_Enemy01ModelData[nCnt3].m_vtx[7].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				nCnt3++;
			}

			//�Q�[���ɔz�u���郂�f����ǂݍ���
			if (strcmp(sString[0], "OBJECTMODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_ObjectModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);

				//���f�����̓ǂݍ���
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_ObjectModelData[nCnt2].m_pBuffMat,
					NULL,
					&m_ObjectModelData[nCnt2].m_nNumMat,
					&m_ObjectModelData[nCnt2].m_pMesh);

				//�}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL *)m_ObjectModelData[nCnt2].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_ObjectModelData[nCnt2].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;

						//�e�N�X�`���̓ǂݍ���
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_ObjectModelData[nCnt2].m_pTexture.push_back(Securement);

					}
					else
					{
						m_ObjectModelData[nCnt2].m_pTexture.push_back(NULL);
					}

				}
				//���_�����擾
				m_ObjectModelData[nCnt2].m_nNumVtx = m_ObjectModelData[nCnt2].m_pMesh->GetNumVertices();
				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				m_ObjectModelData[nCnt2].m_sizeFVF = D3DXGetFVFVertexSize(m_ObjectModelData[nCnt2].m_pMesh->GetFVF());
				//���_�o�b�t�@���A�����b�N
				m_ObjectModelData[nCnt2].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_ObjectModelData[nCnt2].m_pVtxBuff);
				//���f���̑傫���𑪂�
				for (int nCnt = 0; nCnt < m_ObjectModelData[nCnt2].m_nNumVtx; nCnt++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_ObjectModelData[nCnt2].m_pVtxBuff;//���_���W�̑��
																				  //x�̍ő�l�̔�r
					if (m_ObjectModelData[nCnt2].m_vtxMax.x >= vtx.x)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.x = vtx.x;
					}
					//���̍ŏ��l�̔�r
					else if (m_ObjectModelData[nCnt2].m_vtxMin.x <= vtx.x)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.x = vtx.x;
					}
					//y�̍ő�l�̔�r
					if (m_ObjectModelData[nCnt2].m_vtxMax.y >= vtx.y)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.y = vtx.y;
					}
					//y�̍ŏ��l�̔�r
					else if (m_ObjectModelData[nCnt2].m_vtxMin.y <= vtx.y)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.y = vtx.y;
					}
					//z�̍ő�l�̔�r
					if (m_ObjectModelData[nCnt2].m_vtxMax.z >= vtx.z)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.z = vtx.z;
					}
					//z�̍ŏ��l�̔�r
					else if (m_ObjectModelData[nCnt2].m_vtxMin.z <= vtx.z)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.z = vtx.z;
					}
					m_ObjectModelData[nCnt2].m_pVtxBuff += m_ObjectModelData[nCnt2].m_sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				}
				//���_�o�b�t�@���A�����b�N
				m_ObjectModelData[nCnt2].m_pMesh->UnlockVertexBuffer();
				//8�̒��_���̕ۑ�
				//�����i��ʁj
				m_ObjectModelData[nCnt2].m_vtx[0].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//�E���i��ʁj
				m_ObjectModelData[nCnt2].m_vtx[1].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//���O�i��ʁj
				m_ObjectModelData[nCnt2].m_vtx[2].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//�E�O�i��ʁj
				m_ObjectModelData[nCnt2].m_vtx[3].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//�����i���ʁj
				m_ObjectModelData[nCnt2].m_vtx[4].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//�E���i���ʁj
				m_ObjectModelData[nCnt2].m_vtx[5].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//���O�i���ʁj
				m_ObjectModelData[nCnt2].m_vtx[6].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//�E�O�i���ʁj
				m_ObjectModelData[nCnt2].m_vtx[7].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				nCnt2++;
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}

	}
	fclose(File);
}