//=========================================
//���@�w�̏���
//=========================================
#include "main.h"
#include "MagicCircle.h"
#include "manager.h"
#include "renderer.h"
#include "Scene3D.h"
#include "Particle.h"
#include "shadow.h"
#define MAX_ERASE_TIME (60)
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMagicCircle::CMagicCircle(OBJTYPE nPriority) : CEffect(nPriority)
{
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_size = { 1.0f,1.0f,1.0f };
	m_fRotSpeed = 0.5f;
	m_fEraseMaxTime = MAX_ERASE_TIME;
	m_fEraseTime = 0.0f;
	m_fScaleSpeed = 1.5f;
	m_bErase = false;
	m_pParticle = nullptr;
	m_bSizeAdd = false;
	m_bSizeDef = false;
	m_rot = { D3DXToRadian(90.0f),0.0f,0.0f };
	pShadow = nullptr;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMagicCircle::~CMagicCircle()
{

}

//---------------------------------------------------
//�C���X�^���X����
//---------------------------------------------------
CMagicCircle * CMagicCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot, bool bEraseStart,
	D3DXCOLOR col, CTexture::Type type, const float fSpeed)
{//	�ʒu, �ړ���, �T�C�Y,�F, �������Ȃ�X�s�[�h
 //�C���X�^���X����
	CMagicCircle *pEffect = new CMagicCircle(OBJTYPE_EFFECT);
	pEffect->m_pos = pos;
	pEffect->m_rot = rot;
	pEffect->m_fMaxSize = fsize;
	pEffect->m_col = col;
	pEffect->m_TexType = type;
	pEffect->m_bEraseStart = bEraseStart;
	pEffect->m_fEraseMaxTime = fMaxTime;
	pEffect->m_bSizeAdd = bEraseStart;
	pEffect->m_fScaleSpeed = fSpeed;
	pEffect->bOnBill(false);
	pEffect->m_bRotX = bRot;
	if (pEffect != NULL)
	{
		//�摜�ǂݍ���
		pEffect->Init();

	}

	return pEffect;
}

//----------------------------------------------------
//����������
//-----------------------------------------------------
HRESULT CMagicCircle::Init()
{
	CEffect::Init();
	//�J�����O������悤�ɂ���
	SetCulling(true);
	if (m_pParticle == nullptr)
	{
		m_pParticle = new CParticle();
	}
	
	return S_OK;
}

//---------------------------------------------------------
//�I������
//---------------------------------------------------------
void CMagicCircle::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (pShadow != nullptr)
	{
		pShadow->Uninit();
		pShadow = nullptr;
	}
	Release();
}
//---------------------------------------------------------
//�X�V����
//---------------------------------------------------------
void CMagicCircle::Update()
{
	Setpos(m_pos, m_size);//���_���̐ݒ�

	SetCol_E(m_col);
	//pShadow->SetCol(m_col);
	//�ړ�����
	m_pos += m_move;
	if (m_bRotX == false)
	{
		m_rot.y += D3DXToRadian(m_fRotSpeed);

	}
	else
	{
		m_rot.z += D3DXToRadian(m_fRotSpeed);

	}
	//���@�w�������Ƃ�
	if (m_bEraseStart == true)
	{
		m_fEraseTime++;
		if (m_fEraseTime >= m_fEraseMaxTime)
		{
			//�����ɂ��鏈�����n�߂�
			m_bColDef = true;
			//����ɃT�C�Y��傫���������n�߂�
			m_bEraseSizeStart = true;
		}
	}

	//�T�C�Y���傫���Ȃ鏈��
	if (m_bSizeAdd == true)
	{
		m_bSizeDef = false;
		SizeAdd();
	}
	//�T�C�Y���������Ȃ鏈��
	if (m_bSizeDef == true)
	{
		m_bSizeAdd = false;
		SizeDef();
	}
	//�T�C�Y�����ł������鏈��
	if (m_bEraseSizeStart == true)
	{
		ColSizeAdd();
	}
	//�F�̒l�����������锻�肪�I���Ȃ�
	if (m_bColDef == true)
	{
		//���邳��������
		m_col.a -= 0.03f;
	}
	
	//���l��0�ȉ��ɂȂ����������
	if (m_col.a <= 0.0)
	{
		m_bUninit = true;
	}
	//�I�����肪�I���Ȃ�
	if (m_bUninit == true)
	{
		//�I���������Ă�
		Uninit();
	}

}
//---------------------------------------------------------
//�`�揈��
//---------------------------------------------------------
void CMagicCircle::Draw()
{
	CEffect::Draw();
}

//---------------------------------------------------------
//�T�C�Y���傫���Ȃ鏈��
//---------------------------------------------------------
void CMagicCircle::SizeAdd()
{
	//�T�C�Y�𑝂₷
	m_size.x += m_fScaleSpeed;
	m_size.y += m_fScaleSpeed;
	if (m_size.x >= m_fMaxSize)
	{
		m_size.x = m_fMaxSize;
		m_size.y = m_fMaxSize;
		//�T�C�Y���傫���Ȃ鏈�����~
		m_bSizeAdd = false;
		//�����������s�̔��肪�I����������
		if (m_bErase == true)
		{
			//���������̎��s
			m_bEraseStart = true;
		}
	}
}
//---------------------------------------------------------
//�T�C�Y���������Ȃ鏈��
//---------------------------------------------------------
void CMagicCircle::SizeDef()
{
	m_size.x -= m_fScaleSpeed;
	m_size.y -= m_fScaleSpeed;
	if (m_size.x < 0.0f)
	{
		m_size.x = 0.0f;
		m_size.y = 0.0f;
		m_bSizeDef = false;
	}
}
//---------------------------------------------------------
//�T�C�Y���傫�����邾���̏���
//---------------------------------------------------------
void CMagicCircle::ColSizeAdd()
{
	m_size.x += m_fScaleSpeed;
	m_size.y += m_fScaleSpeed;

}
//---------------------------------------------------------
//�T�C�Y��ݒ�
//---------------------------------------------------------
void CMagicCircle::SetSize()
{

	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	m_pVtxBuff->Unlock();

}
