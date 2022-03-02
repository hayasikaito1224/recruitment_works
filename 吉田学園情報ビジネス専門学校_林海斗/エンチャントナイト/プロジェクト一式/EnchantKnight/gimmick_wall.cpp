//----------------------------------
//�M�~�b�N�ǂ̏���
//----------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "gimmick_wall.h"

//==================================
//�R���X�g���N�^
//==================================
CGimmickWall::CGimmickWall(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
	{
		m_vtx[nCnt].rot = { 0.0f,0.0f,0.0f };

	}
	m_bUninit = false;
}
//==================================
//�f�X�g���N�^
//==================================
CGimmickWall::~CGimmickWall()
{

}

//------------------------------------------------------------------------------------------------
//�C���X�^���X��������(�ʒu�A�傫���A�p�x�A�ǂ̃i���o�[�A�\��e�N�X�`���̎��)
//-------------------------------------------------------------------------------------------------
CGimmickWall *CGimmickWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nNumber, CTexture::Type Tex)
{
	//�C���X�^���X����
	CGimmickWall *pField = new CGimmickWall(OBJTYPE_GIMMICKWALL);

	pField->m_pos = pos;
	pField->m_rot.y = rot.y;
	pField->m_texType = Tex;
	pField->m_nNumber = nNumber;
	pField->m_size = size;
	if (pField != NULL)
	{
		pField->Init();
	}

	return pField;
}
////=============================================================================
// �ǂ̓����蔻��
//=============================================================================
bool CGimmickWall::TestCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius)
{
	bool bHit = false;//�����������ǂ�����Ԃ�
					  //���݂̃f�[�^
	D3DXVECTOR3 NorAsk;//�@��
	D3DXVECTOR3 NorNormalization;//�@���̐��K��
	D3DXVECTOR3 VecNor;//�@���x�N�g��
	D3DXVECTOR3 VecVtx[MAX_GIMMICK_WALL_VTX];//���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecPair;//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 PairPos;//����̈ʒu
	D3DXVECTOR3 PairOldPos;//����̑O�̈ʒu
	D3DXVECTOR3 VecVtxWhile[MAX_GIMMICK_WALL_VTX];////���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecVtxPair[MAX_GIMMICK_WALL_VTX];//����̈ʒu���玩���̒��_�ʒu�܂ł̃x�N�g��

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
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
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
	for (int nNumVtx = 0; nNumVtx < MAX_GIMMICK_WALL_VTX; nNumVtx++)
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

	float fVecClossX[MAX_GIMMICK_WALL_VTX];//�O��
	float fVecClossZ[MAX_GIMMICK_WALL_VTX];//�O��

								   //�ǂ̊O�ς����߂�
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
	{
		fVecClossX[nCnt] = (VecVtxWhile[nCnt].z*VecVtxPair[nCnt].y) -
			(VecVtxPair[nCnt].z * VecVtxWhile[nCnt].y);

		fVecClossZ[nCnt] = (VecVtxWhile[nCnt].x*VecVtxPair[nCnt].y) -
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
	D3DXVECTOR3 NorMoveVec = { MoveVec.x*-1,MoveVec.y*-1,MoveVec.z*-1 };


	//�����Ɩ@���̓��ς����߂�
	float fNorMoveDot = D3DXVec3Dot(&NorMoveVec, &NorNormalization);

	//�@���x�N�g�������߂�
	D3DXVECTOR3 NorMoveDotVec = { NorNormalization.x*fNorMoveDot ,
		NorNormalization.y*fNorMoveDot,
		NorNormalization.z*fNorMoveDot };
	if (NorNormalization.x <= -0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
		{
			fVecClossX[nCnt] *= -1.0f;
		}
	}
	if (NorNormalization.z >= 0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
		{
			fVecClossZ[nCnt] *= -1.0f;
		}
	}
	//�@���x�N�g��������̔��a�ȉ��ɂȂ��Ă�&&�O�̖@���x�N�g��������̔��a�ȏゾ������
	if (fVecClossX[0] < 0.00f && fVecClossX[1] < 0.00f && fVecClossX[2] < 0.00f && fVecClossX[3] < 0.00f ||
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
//�M�~�b�N�ŕ��Ă����ǂ̊J������
//==================================
void CGimmickWall::GimmickUnlock(int nNumber)
{
	//�����̐��l�Ƃ��̃I�u�W�F�N�g�������Ă��鐔�l�������Ȃ�
	if (nNumber == m_nNumber)
	{
		//�폜
		m_bUninit = true;
	}
}

//==================================
//������
//==================================
HRESULT CGimmickWall::Init(void)
{
	CScene3D::Init(CScene3D::TYPE_WALL,m_texType);

	//�����i��ʁj
	m_vtx[0].vtx = D3DXVECTOR3(
		-m_size.x,
		m_size.y,
		0.0f);

	//�E���i��ʁj
	m_vtx[1].vtx = D3DXVECTOR3(
		m_size.x,
		m_size.y,
		0.0f);

	//���O�i��ʁj
	m_vtx[2].vtx = D3DXVECTOR3(
		-m_size.x,
		-m_size.y,
		0.0f);

	//�E�O�i��ʁj
	m_vtx[3].vtx = D3DXVECTOR3(
		m_size.x,
		-m_size.y,
		0.0f);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGimmickWall::Uninit(void)
{

	CScene3D::Uninit();
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGimmickWall::Update(void)
{
	if (m_bUninit)
	{
		Uninit();
	}
}
//=======================================================================
//�`�揈��
//=======================================================================
void CGimmickWall::Draw()
{
	CScene3D::Draw();
}

