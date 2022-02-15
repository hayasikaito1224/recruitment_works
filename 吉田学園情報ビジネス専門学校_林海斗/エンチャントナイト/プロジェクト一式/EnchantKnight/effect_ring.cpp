#include "main.h"
#include "effectring.h"
#include "manager.h"
#include "renderer.h"
#include "Scene3D.h"
#define SIZE_SPEED (2.0)
CEffect_Ring::CEffect_Ring(OBJTYPE nPriority) : CEffect(nPriority)
{
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_bOnBillbord = false;
	m_bCulling = true;
	m_bAddColA = true;
	m_fSizeSpeed = SIZE_SPEED;
}

CEffect_Ring::~CEffect_Ring()
{

}

//---------------------------------------------------
//�C���X�^���X����
//---------------------------------------------------
CEffect_Ring * CEffect_Ring::Create(D3DXVECTOR3 pos,D3DXCOLOR col,float fDefSpeed, float fSizeSpeed, CTexture::Type type)
{//	�ʒu, �ړ���, �T�C�Y,�F, �������Ȃ�X�s�[�h
 //�C���X�^���X����
	CEffect_Ring *pEffect = new CEffect_Ring(OBJTYPE_EFFECT);
	pEffect->m_pos = pos;
	pEffect->m_col = col;
	pEffect->m_TexType = type;
	pEffect->m_fSizeSpeed = fSizeSpeed;
	if (pEffect != NULL)
	{
		//�摜�ǂݍ���
		pEffect->Init();
	}

	return pEffect;
}

//---------------------------------------------------
//�T�C�Y�̐ݒ�
//---------------------------------------------------

void CEffect_Ring::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);

	m_pVtxBuff->Unlock();

}

//----------------------------------------------------
//����������
//-----------------------------------------------------


HRESULT CEffect_Ring::Init()
{
	CEffect::Init();

	return S_OK;
}

//---------------------------------------------------------
//�I������
//---------------------------------------------------------
void CEffect_Ring::Uninit()
{
	CEffect::Uninit();


}

void CEffect_Ring::Update()
{
	CEffect::Update();
	if (m_bUninit == false)
	{
		m_size.x += m_fSizeSpeed;
		m_size.z += m_fSizeSpeed;
		SetSize(m_size);
	}
}

void CEffect_Ring::Draw()
{
	CEffect::Draw();

}

