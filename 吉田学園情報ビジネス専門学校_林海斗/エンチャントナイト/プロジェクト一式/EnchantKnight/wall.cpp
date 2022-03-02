#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "Scene3D.h"
#include "wall.h"
//==================================
//�R���X�g���N�^
//==================================

CWall::CWall(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_WallData.m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pIdxBuffWall = NULL;
	m_WallData.m_nMax_Vtx_X = 0;
	m_WallData.m_nMax_Vtx_Y = 0;
	m_nVtx_Num_Max = 0;
	m_nIdx_Num_Max = 0;
	m_nPoly_Num_Max = 0;
	m_fHalfWidth = 0.0f;
	m_fHalfDepth = 0.0f;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	for (int nCnt = 0; nCnt < MAX_WALL_VTX; nCnt++)
	{
		m_vtx[nCnt].rot = { 0.0f,0.0f,0.0f };

	}
}
//==================================
//�f�X�g���N�^
//==================================

CWall::~CWall()
{

}

//------------------------------------------------------------------------------------------------
//�C���X�^���X��������(�ʒu�A�傫���A�p�x�AX���̃u���b�N���AZ���̃u���b�N���A�\��e�N�X�`���̎��)
//-------------------------------------------------------------------------------------------------
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nNum_X, int nNum_Y, CTexture::Type Tex)
{
	//�C���X�^���X����
	CWall *pField = new CWall(OBJTYPE_WALL);

	pField->m_pos = pos;
	pField->m_WallData.m_InitSize = size;
	pField->m_WallData.m_size = D3DXVECTOR3(size.x*nNum_X, size.y*nNum_Y, size.z);
	pField->m_rot.y = rot.y;
	pField->m_fHalfWidth = pField->m_WallData.m_size.x / 2.0f;
	pField->m_fHalfDepth = pField->m_WallData.m_size.y / 2.0f;
	pField->m_WallData.m_nMax_Vtx_X = nNum_X;
	pField->m_WallData.m_nMax_Vtx_Y = nNum_Y;
	pField->m_pTexture = CManager::GetTexture()->GetTexture(Tex);

	if (pField != NULL)
	{
		pField->Init();
	}

	return pField;
}
////=============================================================================
// �ǂ̓����蔻��
//=============================================================================

bool CWall::TestCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius)
{
	bool bHit = false;//�����������ǂ�����Ԃ�
	//���݂̃f�[�^
	D3DXVECTOR3 NorAsk;//�@��
	D3DXVECTOR3 NorNormalization;//�@���̐��K��
	D3DXVECTOR3 VecNor;//�@���x�N�g��
	D3DXVECTOR3 VecVtx[MAX_WALL_VTX];//���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecPair;//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 PairPos;//����̈ʒu
	D3DXVECTOR3 PairOldPos;//����̑O�̈ʒu
	D3DXVECTOR3 VecVtxWhile[MAX_WALL_VTX];////���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecVtxPair[MAX_WALL_VTX];//����̈ʒu���玩���̒��_�ʒu�܂ł̃x�N�g��

	//�O�̃f�[�^
	D3DXVECTOR3 OldVecNor;//�@���x�N�g��
	D3DXVECTOR3 OldVecPair;//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g��
	NorNormalization = { 0.0f,0.0f,0.0f };
	NorAsk = { 0.0f,0.0f,0.0f };
	VecNor = { 0.0f,0.0f,0.0f };
	VecPair = { 0.0f,0.0f,0.0f };
	OldVecNor = { 0.0f,0.0f,0.0f };
	OldVecPair = { 0.0f,0.0f,0.0f };
	PairPos = *pPos;
	PairOldPos = *pLastpos;

	//���[�J���ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_WALL_VTX; nCnt++)
	{
		VecVtx[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxPair[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxWhile[nCnt] = { 0.0f,0.0f,0.0f };

	}
	//�q��pos�𑊎�̕������₷
	//��
	m_vtx[0].vtx.x -= fRadius;
	m_vtx[0].vtx.z -= fRadius;

	m_vtx[1].vtx.x += fRadius;
	m_vtx[1].vtx.z -= fRadius;

	m_vtx[2].vtx.x -= fRadius;
	m_vtx[2].vtx.z -= fRadius;

	m_vtx[3].vtx.x += fRadius;
	m_vtx[3].vtx.z -= fRadius;

	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���̂���}�g���b�N�X�ɔ��f������
	for (int nNumVtx = 0; nNumVtx < MAX_WALL_VTX; nNumVtx++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

											  //�e���_�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_vtx[nNumVtx].mtxWorld);
		////�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_vtx[nNumVtx].rot.y, m_vtx[nNumVtx].rot.x, m_vtx[nNumVtx].rot.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxRotModel);
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel, m_vtx[nNumVtx].vtx.x, m_vtx[nNumVtx].vtx.y, m_vtx[nNumVtx].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxTransModel);
		mtxParent = m_mtxWorld;
		//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld,
			&m_vtx[nNumVtx].mtxWorld,
			&m_mtxWorld);

		//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nNumVtx].mtxWorld);
	}

	//���_�ƒ��_�̊Ԃ̃x�N�g�������߂�
	VecVtxWhile[0] = { m_vtx[1].mtxWorld._41 - m_vtx[0].mtxWorld._41,
					   m_vtx[1].mtxWorld._42 - m_vtx[0].mtxWorld._42,
					   m_vtx[1].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	VecVtxWhile[1] = { m_vtx[3].mtxWorld._41 - m_vtx[1].mtxWorld._41,
					   m_vtx[3].mtxWorld._42 - m_vtx[1].mtxWorld._42,
					   m_vtx[3].mtxWorld._43 - m_vtx[1].mtxWorld._43 };

	VecVtxWhile[2] = { m_vtx[2].mtxWorld._41 - m_vtx[3].mtxWorld._41,
					   m_vtx[2].mtxWorld._42 - m_vtx[3].mtxWorld._42,
					   m_vtx[2].mtxWorld._43 - m_vtx[3].mtxWorld._43 };

	VecVtxWhile[3] = { m_vtx[0].mtxWorld._41 - m_vtx[2].mtxWorld._41,
					   m_vtx[0].mtxWorld._42 - m_vtx[2].mtxWorld._42,
					   m_vtx[0].mtxWorld._43 - m_vtx[2].mtxWorld._43 };

	//����̈ʒu���玩���̒��_�ʒu�܂ł̃x�N�g��
	VecVtxPair[0] = { PairPos.x - m_vtx[0].mtxWorld._41 ,
					  PairPos.y - m_vtx[0].mtxWorld._42 ,
					  PairPos.z - m_vtx[0].mtxWorld._43 };

	VecVtxPair[1] = { PairPos.x - m_vtx[1].mtxWorld._41 ,
					  PairPos.y - m_vtx[1].mtxWorld._42 ,
					  PairPos.z - m_vtx[1].mtxWorld._43 };

	VecVtxPair[2] = { PairPos.x - m_vtx[3].mtxWorld._41 ,
					  PairPos.y - m_vtx[3].mtxWorld._42 ,
					  PairPos.z - m_vtx[3].mtxWorld._43 };

	VecVtxPair[3] = { PairPos.x - m_vtx[2].mtxWorld._41 ,
					  PairPos.y - m_vtx[2].mtxWorld._42 ,
					  PairPos.z - m_vtx[2].mtxWorld._43 };

	float fVecClossX[MAX_WALL_VTX];//�O��
	float fVecClossZ[MAX_WALL_VTX];//�O��

	//�ǂ̊O�ς����߂�
	for (int nCnt = 0; nCnt < MAX_WALL_VTX; nCnt++)
	{
		fVecClossX[nCnt] = (VecVtxWhile[nCnt].z*VecVtxPair[nCnt].y) -
							(VecVtxPair[nCnt].z * VecVtxWhile[nCnt].y);

		fVecClossZ[nCnt] = (VecVtxWhile[nCnt].x*VecVtxPair[nCnt].y)-
							(VecVtxPair[nCnt].x * VecVtxWhile[nCnt].y);
	}


	//���_�O�ƒ��_�P�̃x�N�g��
	VecVtx[0] = { m_vtx[1].mtxWorld._41 - m_vtx[0].mtxWorld._41,
				m_vtx[1].mtxWorld._42 - m_vtx[0].mtxWorld._42,
				m_vtx[1].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	//���_�O�ƒ��_2�̃x�N�g��
	VecVtx[1] = { m_vtx[2].mtxWorld._41 - m_vtx[0].mtxWorld._41,
				m_vtx[2].mtxWorld._42 - m_vtx[0].mtxWorld._42,
				m_vtx[2].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	//�����̖@�������߂�
	D3DXVec3Cross(&NorAsk, &VecVtx[0], &VecVtx[1]);

	//���K��
	D3DXVec3Normalize(&NorNormalization, &NorAsk);

	//********���݂̈ʒu�̃f�[�^********
	//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g�������߂�
	VecPair = { PairPos.x - m_pos.x,0.0f,PairPos.z - m_pos.z };

	//���ς����߂�

	float fDot;//����
	fDot = D3DXVec3Dot(&VecPair, &NorNormalization);

	//�@���x�N�g�������߂�
	VecNor = NorNormalization*fDot;

	//*********�O�̈ʒu�̃f�[�^**********
	//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g�������߂�
	OldVecPair = { PairOldPos.x - m_pos.x,0.0f,PairOldPos.z - m_pos.z };

	//���ς����߂�
	float fOldDot;//����
	fOldDot = D3DXVec3Dot(&OldVecPair, &NorNormalization);

	//�@���x�N�g�������߂�
	OldVecNor = NorNormalization*fOldDot;

	//�@���x�N�g���̒��������߂�
	float fLength = sqrtf((VecNor.x * VecNor.x) + (VecNor.z * VecNor.z));
	float fOldLength = sqrtf((OldVecNor.x * OldVecNor.x) + (OldVecNor.z * OldVecNor.z));

	//����̉ߋ��̈ʒu�ƌ��݂̈ʒu�̍��������߂�
	D3DXVECTOR3 MoveVec = PairPos - PairOldPos;
	D3DXVECTOR3 NorMoveVec = { MoveVec.x*-1,MoveVec.y*-1,MoveVec.z*-1};


	//�����Ɩ@���̓��ς����߂�
	float fNorMoveDot = D3DXVec3Dot(&NorMoveVec, &NorNormalization);

	//�@���x�N�g�������߂�
	D3DXVECTOR3 NorMoveDotVec = { NorNormalization.x*fNorMoveDot ,
				NorNormalization.y*fNorMoveDot,
				NorNormalization.z*fNorMoveDot };
	if (NorNormalization.x <= -0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_WALL_VTX; nCnt++)
		{
			fVecClossX[nCnt] *= -1.0f;
		}
	}
	if (NorNormalization.z >= 0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_WALL_VTX; nCnt++)
		{
			fVecClossZ[nCnt] *= -1.0f;
		}
	}
	//�@���x�N�g��������̔��a�ȉ��ɂȂ��Ă�&&�O�̖@���x�N�g��������̔��a�ȏゾ������
	if (fVecClossX[0] < 0.00f && fVecClossX[1] < 0.00f && fVecClossX[2] < 0.00f && fVecClossX[3] < 0.00f||
		fVecClossZ[0] < 0.00f && fVecClossZ[1] < 0.00f && fVecClossZ[2] < 0.00f && fVecClossZ[3] < 0.00f)
	{
		if (fLength <= fRadius && fOldLength >= -10.0f)
		{
			D3DXVECTOR3 push = NorMoveDotVec + NorMoveVec;
			//*pPos = *pLastpos;
			pPos->x += NorMoveDotVec.x;
			pPos->z += NorMoveDotVec.z;

			bHit = true;
		}

	}
	//��
	m_vtx[0].vtx.x -= -fRadius;
	m_vtx[0].vtx.z -= -fRadius;
	m_vtx[1].vtx.x += -fRadius;
	m_vtx[1].vtx.z -= -fRadius;
	m_vtx[2].vtx.x -= -fRadius;
	m_vtx[2].vtx.z -= -fRadius;
	m_vtx[3].vtx.x += -fRadius;
	m_vtx[3].vtx.z -= -fRadius;

	return bHit;
}

//==================================
//�T�C�Y��ݒ�
//==================================

void CWall::SetSize(D3DXVECTOR3 size)
{
	m_WallData.m_size = size;
}
//==================================
//�e�N�X�`���𓮂���
//==================================

void CWall::SetMoveTex(float fSpeedX, float fSpeedY)
{
	m_MoveTexX = fSpeedX;
	m_MoveTexY = fSpeedY;
	//-----------------------------
	//���_���
	//-----------------------------
	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntY = 0; nCntY < m_WallData.m_nMax_Vtx_Y + 1; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_WallData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX) + m_MoveTexX, 0.0f + (1.0f * nCntY) + m_MoveTexX);
		}
	}
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//==================================
//�F��ݒ�
//==================================

void CWall::SetCol(float fAlpha)
{
	//-----------------------------
	//���_���
	//-----------------------------
	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntY = 0; nCntY < m_WallData.m_nMax_Vtx_Y + 1; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_WallData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{
			pVtx[nVtxNum].col = D3DXCOLOR(1.0, 1.0, 1.0, fAlpha);
		}
	}
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//==================================
//������
//==================================

HRESULT CWall::Init(void)
{
	m_nVtx_Num_Max = (m_WallData.m_nMax_Vtx_X + 1)*(m_WallData.m_nMax_Vtx_Y + 1);
	m_nIdx_Num_Max = (m_WallData.m_nMax_Vtx_X + 1)*(m_WallData.m_nMax_Vtx_Y + 1) + (m_WallData.m_nMax_Vtx_Y - 1)*(m_WallData.m_nMax_Vtx_X + 3);
	m_nPoly_Num_Max = 2 * m_WallData.m_nMax_Vtx_X * m_WallData.m_nMax_Vtx_Y + (m_WallData.m_nMax_Vtx_Y * 4) - 4;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
																	 //�e�N�X�`���摜�̐ݒ�

																	 //�p�x����~�����ɕϊ�����
																	 //���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVtx_Num_Max,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD)* m_nIdx_Num_Max,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffWall,
		NULL);
	//-----------------------------
	//���_���
	//-----------------------------
	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nVtxNum = 0;
	for (int nCntY = 0; nCntY < m_WallData.m_nMax_Vtx_Y + 1; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_WallData.m_nMax_Vtx_X + 1; nCntX++, nVtxNum++)
		{

			pVtx[nVtxNum].pos = D3DXVECTOR3(
				-m_fHalfWidth + (((m_fHalfWidth * 2) / m_WallData.m_nMax_Vtx_X)*nCntX),
				m_fHalfDepth - (((m_fHalfDepth * 2) / m_WallData.m_nMax_Vtx_Y)*nCntY),
				0.0f);

			pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[nVtxNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX) + m_MoveTexX, 0.0f + (1.0f * nCntY) + m_MoveTexX);
			if (m_vtxMax.x <= pVtx[nVtxNum].pos.x)
			{
				m_vtxMax.x = pVtx[nVtxNum].pos.x;
			}
			//���̍ŏ��l�̔�r
			else if (m_vtxMin.x >= pVtx[nVtxNum].pos.x)
			{
				m_vtxMin.x = pVtx[nVtxNum].pos.x;
			}
			//y�̍ő�l�̔�r
			if (m_vtxMax.y <= pVtx[nVtxNum].pos.y)
			{
				m_vtxMax.y = pVtx[nVtxNum].pos.y;
			}
			//y�̍ŏ��l�̔�r
			else if (m_vtxMin.y >= pVtx[nVtxNum].pos.y)
			{
				m_vtxMin.y = pVtx[nVtxNum].pos.y;
			}
			//z�̍ő�l�̔�r
			if (m_vtxMax.z <= pVtx[nVtxNum].pos.z)
			{
				m_vtxMax.z = pVtx[nVtxNum].pos.z;
			}
			//z�̍ŏ��l�̔�r
			else if (m_vtxMin.z >= pVtx[nVtxNum].pos.z)
			{
				m_vtxMin.z = pVtx[nVtxNum].pos.z;
			}

		}
	}
	nVtxNum = 0;
	//�����i��ʁj
	m_vtx[0].vtx = D3DXVECTOR3(
		m_vtxMin.x,
		m_vtxMax.y,
		0.0f);

	//�E���i��ʁj
	m_vtx[1].vtx = D3DXVECTOR3(
		m_vtxMax.x,
		m_vtxMax.y,
		0.0f);

	//���O�i��ʁj
	m_vtx[2].vtx = D3DXVECTOR3(
		m_vtxMin.x,
		m_vtxMin.y,
		0.0f);

	//�E�O�i��ʁj
	m_vtx[3].vtx = D3DXVECTOR3(
		m_vtxMax.x,
		m_vtxMin.y,
		0.0f);


	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
	//----------------------------
	//�C���f�b�N�X���
	//----------------------------
	WORD *pIdx;			//�C���f�b�N�X���ւ̃|�C���^

						//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);
	//�p���ȊO�̒��_
	for (int nCntY = 0; nCntY < m_WallData.m_nMax_Vtx_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_WallData.m_nMax_Vtx_X + 1; nCntX++)
		{
			pIdx[(nCntX * 2) + 0 + ((m_WallData.m_nMax_Vtx_X + 2) * 2) * nCntY] = ((m_WallData.m_nMax_Vtx_X + 1) + nCntX) + ((m_WallData.m_nMax_Vtx_X + 1) * nCntY);
			pIdx[(nCntX * 2) + 1 + ((m_WallData.m_nMax_Vtx_X + 2) * 2) * nCntY] = (0 + nCntX) + ((m_WallData.m_nMax_Vtx_X + 1) * nCntY);
		}
	}
	//�p���̒��_
	for (int nCntY = 0; nCntY < m_WallData.m_nMax_Vtx_Y - 1; nCntY++)
	{
		pIdx[(((m_WallData.m_nMax_Vtx_X + 1) * 2 + 0) * (nCntY + 1)) + (2 * nCntY)] = m_WallData.m_nMax_Vtx_X + ((m_WallData.m_nMax_Vtx_X + 1) * nCntY);
		pIdx[(((m_WallData.m_nMax_Vtx_X + 1) * 2 + 1) * (nCntY + 1)) + (1 * nCntY)] = (m_WallData.m_nMax_Vtx_X * 2 + 2) + ((m_WallData.m_nMax_Vtx_X + 1) * nCntY);
	}
	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuffWall->Unlock();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWall::Uninit(void)
{

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuffWall != NULL)
	{
		m_pIdxBuffWall->Release();
		m_pIdxBuffWall = NULL;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWall::Update(void)
{
	
}
//=======================================================================
//�`�揈��
//=======================================================================
void CWall::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

								//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	////�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���Ńo�X�̂Ł[�����Ƃ�[�ނɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffWall);

	pDevice->SetFVF(FVF_VERTEX_3D);//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, m_pTexture);
	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,					//�v���~�e�B�u�̎��
		0,									//�C���f�b�N�X�o�b�t�@�̊J�n�n�_����ŏ��̃C���f�b�N�X�܂ł̃I�t�Z�b�g
		m_nVtx_Num_Max,	//�`��Ŏg�p�����ŏ��̒��_�ԍ�
		(m_WallData.m_nMax_Vtx_X + 1)*(m_WallData.m_nMax_Vtx_Y + 1),		//���_�̐�
		0,	//�C���f�b�N�X�z��̓ǂݎ����J�n����ʒu
		m_nPoly_Num_Max);	//�O�p�`�|���S���̐�

}

