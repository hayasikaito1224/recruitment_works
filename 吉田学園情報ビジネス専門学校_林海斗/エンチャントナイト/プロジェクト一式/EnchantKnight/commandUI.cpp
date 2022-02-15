//=============================================
//�v���C���[�����삷��R�}���h�֌W�̏���
//=============================================
#include "commandUI.h"
#define NEXTMOVE (50.0)
#define MOVESPEED (8.0)
#define COLSPEED (0.1)
#define MAX_COL (0.7)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCommandUI::CCommandUI(OBJTYPE nPriority) : CScene2D(nPriority)
{
	m_bSelect = false;
	m_bDraw = true;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCommandUI::~CCommandUI()
{
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CCommandUI::Init()
{
	//�|���S���̐���
	CScene2D::Init(m_TexType);
	CScene2D::SetPos(m_pos,m_size);
	CScene2D::SetCol(D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
	m_col = { 1.0,1.0,1.0,1.0 };
	m_InitPos = m_pos;//�����̈ʒu��ۑ�
	return S_OK;
}
//=============================================================================
// �j��
//=============================================================================
void CCommandUI::Uninit()
{
	CScene2D::Uninit();
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CCommandUI::Update()
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
		m_pos.x-= MOVESPEED;
		m_fMove = 0.0;
		if (m_InitPos.x >= m_pos.x)
		{
			m_pos.x = m_InitPos.x;
		}

	}
	if (m_bPush)
	{
		m_col.r -= COLSPEED;
		m_col.g -= COLSPEED;
		m_col.b -= COLSPEED;
		if (m_col.r <= MAX_COL)
		{
			m_col.r = MAX_COL;
			m_col.g = MAX_COL;
			m_col.b = MAX_COL;
		}
	}
	else
	{
		m_col.r += COLSPEED;
		m_col.g += COLSPEED;
		m_col.b += COLSPEED;
		if (m_col.r >= 1.0)
		{
			m_col.r = 1.0;
			m_col.g = 1.0;
			m_col.b = 1.0;

		}

	}
	CScene2D::SetCol(m_col);
	CScene2D::SetPos(m_pos, m_size);

}
//=============================================================================
// �`�揈��
//=============================================================================
void CCommandUI::Draw()
{
	if (m_bDraw)
	{
		CScene2D::Draw();

	}
}
//=============================================================================
//�C���X�^���X��������
//=============================================================================
CCommandUI * CCommandUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType)
{
	//�C���X�^���X����
	CCommandUI *pCommand = new CCommandUI(CScene::OBJTYPE_UI);
	if (pCommand)
	{
		pCommand->m_TexType = (CTexture::Type)nTexType;
		pCommand->m_pos = pos;
		pCommand->m_size = size;
		pCommand->Init();
	}
	return pCommand;
}
