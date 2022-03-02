//----------------------------------
//�����蔻�肪��������G���A�̕\��
//-----------------------------------
#include "col_area.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"

#include "keyboard.h"
#include "Scene3D.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCol_Area::CCol_Area(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bDraw = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_fTime = 0.0f;
	m_col = { 0.7f,0.0,0.0,1.0 };
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCol_Area::~CCol_Area()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CCol_Area::Init()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^

																	 // ���_����ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//�e�N�X�`���摜�̐ݒ�
	m_pTexture = CManager::GetTexture()->GetTexture(m_Tex);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�o�b�t�@�̐���
	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.01f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.01f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.01f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.01f);
	m_vtx[0].vtx = pVtx[0].pos;
	m_vtx[1].vtx = pVtx[1].pos;
	m_vtx[2].vtx = pVtx[2].pos;
	m_vtx[3].vtx = pVtx[3].pos;

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
void CCol_Area::Uninit(void)
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
void CCol_Area::Update(void)
{
	MoveVtx();
	
	if (m_bUninit)
	{
		Uninit();
	}

}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CCol_Area::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

								//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//m_mtxWorld._11 = m_size.x;
	//m_mtxWorld._33 = m_size.z;
	////���e�X�g
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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

}
//---------------------------------------------------------------
//�C���X�^���X��������
//---------------------------------------------------------------
CCol_Area * CCol_Area::Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fAlpha, const float MaxGauge,
	const int nValueMax, const CTexture::Type texture)
{
	//�C���X�^���X����
	CCol_Area *pCol_Area = new CCol_Area(OBJTYPE_EFFECT);
	if (pCol_Area != NULL)
	{
		pCol_Area->m_pos = pos;
		pCol_Area->m_size.x = fSizeX;
		pCol_Area->m_col.a = fAlpha;
		pCol_Area->m_Tex = texture;
		pCol_Area->m_rot.y = fRotY;
		pCol_Area->m_fMaxGauge = MaxGauge;
		pCol_Area->m_fTimeMax = nValueMax;
		pCol_Area->Init();
	}
	return pCol_Area;
}
//---------------------------------------------------------------
//���_�̈ړ�
//---------------------------------------------------------------
void CCol_Area::MoveVtx()
{
	m_fTime += 1.0f;
	//���_���ړ�����ʂ��v�Z
	float fAdd = (m_fMaxGauge * 1.0f) / m_fTimeMax;
	m_size.z += fAdd;
	//���߂�ꂽ���ԂɂȂ�����
	if (m_fTime >= m_fTimeMax)
	{
		//���Ԃ��ő�̎��Ԃɍ��킹��
		m_fTime = m_fTimeMax;
		//Z���Ɍ��������_�̍ő吔�����킹��
		m_size.z = m_fMaxGauge;
		//����������I���ɂ���
		m_bUninit = true;
	}
	//���_���̍X�V
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();

}
