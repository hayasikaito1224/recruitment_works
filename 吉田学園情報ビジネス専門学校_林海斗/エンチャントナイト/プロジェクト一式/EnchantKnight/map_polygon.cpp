//----------------------------------
//�|���S���̏���
//-----------------------------------
#include "texture.h"
#include "map_polygon.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "stage.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMap_Polygon::CMap_Polygon(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_bDraw = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMap_Polygon::~CMap_Polygon()
{

}
void CMap_Polygon::BindTexture(CTexture::Type type)
{
	m_pTexture = CManager::GetTexture()->GetTexture(type);
}
//---------------------------------------------------------------
//�C���X�^���X��������
//---------------------------------------------------------------
CMap_Polygon *CMap_Polygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, CTexture::Type texture)
{
	//�C���X�^���X����
	CMap_Polygon *pMap_Polygon = new CMap_Polygon(OBJTYPE_MAP_UI);

	if (pMap_Polygon != NULL)
	{
		pMap_Polygon->BindTexture(texture);
		pMap_Polygon->m_Scale = scale;
		pMap_Polygon->Init();
		pMap_Polygon->m_pos.y = 50.0f;
		pMap_Polygon->SetPos(pos.x,pos.z);
	}

	return pMap_Polygon;

}

HRESULT CMap_Polygon::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^

																	 // ���_����ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(-m_Scale.x, 0.0f, m_Scale.z);
	pVtx[1].pos = D3DXVECTOR3(m_Scale.x, 0.0f, m_Scale.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Scale.x, 0.0f, -m_Scale.z);
	pVtx[3].pos = D3DXVECTOR3(m_Scale.x, 0.0f, -m_Scale.z);

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CMap_Polygon::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================

void CMap_Polygon::Update(void)
{
	
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CMap_Polygon::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X
	if (m_bDraw == true)
	{
									//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);
		m_mtxWorld._11 = m_Scale.x;
		m_mtxWorld._33 = m_Scale.z;
		////���e�X�g
		//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		//���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
		pDevice->SetFVF(FVF_VERTEX_3D);//���_�t�H�[�}�b�g�̐ݒ�

		pDevice->SetTexture(0, m_pTexture);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				2);

		//���e�X�g
		//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
		//���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}


}

//------------------------------------------------------------
//���_���W�̐ݒ�
//------------------------------------------------------------

void CMap_Polygon::SetPos(const float PosX, const float PosZ)
{
	CScene::SetScale(D3DXVECTOR3(PosX, m_pos.y, PosZ));
	m_pos.x = PosX;
	m_pos.z = PosZ;

}

void CMap_Polygon::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}
