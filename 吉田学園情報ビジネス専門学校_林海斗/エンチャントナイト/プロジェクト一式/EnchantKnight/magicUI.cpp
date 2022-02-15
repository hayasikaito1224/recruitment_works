//=============================================
//�v���C���[�����삷��R�}���h�֌W�̏���
//=============================================
#include "magicUI.h"
#include "Polygon.h"
#define NEXTMOVE (50.0)
#define MOVESPEED (8.0)
#define COLSPEED (0.1)
#define MAX_COL (0.7)
#define ALPHA_MIN (0.4)
#define ALPHA_MAX (1.0)
#define BUTTON_POSX (160.0)
#define BUTTON_SIZE (30.0)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMagicUI::CMagicUI(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bSelect = false;
	m_bDraw = true;
	m_pCommandBG = nullptr;
	m_pMagic = nullptr;
	m_bAlphaFadeIn = false;
	m_pButton = nullptr;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMagicUI::~CMagicUI()
{
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMagicUI::Init()
{
	if (m_pCommandBG == nullptr)
	{
		m_pCommandBG = CPolygon::Create(m_pos, m_size, CTexture::COMMAND_BG);
	}
	if (m_pMagic == nullptr)
	{
		m_pMagic = CPolygon::Create(m_pos, m_size, (CTexture::Type)m_nTexType);
	}
	if (m_pButton == nullptr)
	{
		CTexture::Type ButtonTexType = CTexture::None;
		switch (m_nTexType)
		{
		case CTexture::BLAZE:
			ButtonTexType = CTexture::B_BUTTON;
			break;
		case CTexture::ICECLE:
			ButtonTexType = CTexture::X_BUTTON;
			break;
		case CTexture::HEEL:
			ButtonTexType = CTexture::A_BUTTON;
			break;
		}
		m_pButton = CPolygon::Create({ m_pos.x - (float)BUTTON_POSX,m_pos.y,m_pos.z }, { BUTTON_SIZE ,BUTTON_SIZE ,0.0f}, ButtonTexType);

	}
	//�|���S���̐���
	m_col = { 1.0,1.0,1.0,1.0 };
	m_InitPos = m_pos;//�����̈ʒu��ۑ�
	return S_OK;
}
//=============================================================================
// �j��
//=============================================================================
void CMagicUI::Uninit()
{
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->Uninit();
		m_pCommandBG = nullptr;
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->Uninit();
		m_pMagic = nullptr;
	}
	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		m_pButton = nullptr;
	}
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CMagicUI::Update()
{
	//�I�����Ă�����
	if (m_bSelect)
	{
		//�ړ��̉��Z
		m_fMove += MOVESPEED;
		//�ړ��ʂ����̈ړ���ɍs������
		if (m_fMove >= NEXTMOVE)
		{
			m_fMove = NEXTMOVE;
		}
		//�ʒu�Ɉړ��ʂ����Z
		m_pos.x = m_InitPos.x + m_fMove;
	}
	else
	{
		m_pos.x -= MOVESPEED;
		m_fMove = 0.0;
		if (m_InitPos.x >= m_pos.x)
		{
			m_pos.x = m_InitPos.x;
		}

	}
	if (m_bPush)
	{
		m_col.r -= (float)COLSPEED;
		m_col.g -= (float)COLSPEED;
		m_col.b -= (float)COLSPEED;
		if (m_col.r <= MAX_COL)
		{
			m_col.r = MAX_COL;
			m_col.g = MAX_COL;
			m_col.b = MAX_COL;
		}
	}
	else
	{
		m_col.r += (float)COLSPEED;
		m_col.g += (float)COLSPEED;
		m_col.b += (float)COLSPEED;
		if (m_col.r >= 1.0)
		{
			m_col.r = 1.0;
			m_col.g = 1.0;
			m_col.b = 1.0;

		}

	}
	//���l�̃t�F�[�h����
	AlphaFade();
	//�ʒu���̍X�V
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetPos(m_pos);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetPos(m_pos);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetPos({ m_pos.x - (float)BUTTON_POSX,m_pos.y,m_pos.z });
	}

	//�F�̍X�V
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetCol(m_col);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetCol(m_col);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetCol(m_col);
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMagicUI::Draw()
{
	//�`��̐؂�ւ�
	if (m_pCommandBG != nullptr)
	{
		m_pCommandBG->SetDraw(m_bDraw);
	}
	if (m_pMagic != nullptr)
	{
		m_pMagic->SetDraw(m_bDraw);
	}
	if (m_pButton != nullptr)
	{
		m_pButton->SetDraw(m_bDraw);
	}
}
//=============================================================================
//�C���X�^���X��������
//=============================================================================
CMagicUI * CMagicUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType)
{
	//�C���X�^���X����
	CMagicUI *pCommand = new CMagicUI(CScene::OBJTYPE_UI);
	if (pCommand)
	{
		pCommand->m_nTexType = nTexType;
		pCommand->m_pos = pos;
		pCommand->m_size = size;
		pCommand->Init();
	}
	return pCommand;
}
//=============================================================================
//���l�̃t�F�[�h����
//=============================================================================

void CMagicUI::AlphaFade()
{
	if (m_bAlphaFadeIn)
	{
		m_col.a -= 0.05f;
		if (m_col.a < ALPHA_MIN)
		{
			m_col.a = ALPHA_MIN;
		}
	}
	else
	{
		m_col.a += 0.05f;
		if (m_col.a >= ALPHA_MAX)
		{
			m_col.a = ALPHA_MAX;
		}
	}
}
