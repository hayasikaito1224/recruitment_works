#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "Scene3D.h"

CEffect::CEffect(OBJTYPE nPriority) : CScene(nPriority)
{
	m_col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	m_fDefSpeedA = 0.01f;
	m_bOnBillbord = true;
	m_bCulling = false;
	m_bAddColA = true;
	m_bUninit = false;
	m_bScale = false;
	m_fScaleSpeed = 1.0f;
	m_bDefScale = false;
	m_bScaleX = false;
}

CEffect::~CEffect()
{

}

//---------------------------------------------------
//�C���X�^���X����
//---------------------------------------------------
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 size, D3DXCOLOR col, bool bGravity,
	float fGravity, float fDefSpeed, bool bIsBill, CTexture::Type type, float fAng, bool bScele, bool bScaleX, bool bDef)
{//	�ʒu, �ړ���, �T�C�Y,�F, �������Ȃ�X�s�[�h
	//�C���X�^���X����
	CEffect *pEffect = new CEffect(OBJTYPE_EFFECT);
	pEffect->m_pos = pos;
	pEffect->m_size = { size.x*5,size.y*5,size.z*5};
	pEffect->m_initsize = { size.x * 5,size.y * 5,size.z * 5 };

	pEffect->m_fDefSpeedA = fDefSpeed;
	pEffect->m_move = m_move;
	pEffect->m_col = col;
	pEffect->m_TexType = type;
	pEffect->m_bGravity = bGravity;
	pEffect->m_fGravity = fGravity;
	pEffect->m_fAng = fAng;
	pEffect->m_bOnBillbord = bIsBill;
	pEffect->m_bScale = bScele;
	pEffect->m_bScaleX = bScaleX;
	pEffect->m_bDefScale = bDef;
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
HRESULT CEffect::Init(CTexture::Type type)
{
	m_fRand = float(rand() %314/100.0f) - float(rand() % 314 + 314/100.0f);//z�������_���ړ��p
	return S_OK;
}

HRESULT CEffect::Init()
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
	m_pTexture = CManager::GetTexture()->GetTexture(m_TexType);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

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
	Setpos(m_pos, m_size);//���_���̐ݒ�

	return S_OK;
}

//---------------------------------------------------------
//�I������
//---------------------------------------------------------
void CEffect::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

void CEffect::Update()
{
	SetCol_E(m_col);
	if (m_bScaleX)
	{
		//�t���[���񃋂��ƂɃT�C�Y��傫���Ȃ�
		m_size.x += m_fScaleSpeed*3.0f;
		//m_size.y +=m_fScaleSpeed/2.0f;
		if (m_size.x >= m_initsize.x*20.0f)
		{
			m_bScale = false;
		}

	}
	if (m_bScale==true)
	{
		//�t���[���񃋂��ƂɃT�C�Y��傫���Ȃ�
		m_size.x +=m_fScaleSpeed;
		m_size.y +=m_fScaleSpeed;
		if (m_size.x >= m_initsize.x*5.0f)
		{
			//�k���̊J�n
			m_bDefScale = true;
			m_bScale = false;
		}
	}
	//�k��
	if (m_bDefScale)
	{
		m_size.x -= m_fScaleSpeed*1.2f;
		m_size.y -= m_fScaleSpeed*1.2f;
		if (m_size.y <= 0.0f)
		{
			m_size.y = 0.0f;
		}
		if (m_size.x <= 0.0f)
		{
			m_size.x = 0.0f;
		}
	}
	Setpos(m_pos, m_size);//���_���̐ݒ�

	//�ړ�����
	m_pos += m_move;
	//�s�����ɂȂ鏈��
	if (m_bAddColA == true)
	{
		AddColA();
	}

	//���邳��������
	m_col.a -= m_fDefSpeedA;
	//m_col.g += 0.005;
	if (m_pos.y < 0.0f)
	{
		//m_move.y *= -1;
		//m_move.y /= 1.2f;
	}
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}
	//���l��0�ȉ��ɂȂ����������
	if (m_col.a <= 0.0)
	{
		m_bUninit = true;
	}
	if (m_bUninit == true)
	{
		Uninit();

	}
}

void CEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxTrans, mtxRot;				//�ʒu�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxScale;				//�X�P�[���v�Z�p�}�g���b�N�X
										//���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//�ォ�珑�����ނ��Ƃ����Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//���e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	if (m_bCulling == true)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O������
	}
	D3DXMatrixIdentity(&m_mtxWorld);//���[���h�}�g���b�N�X�̏�����

	if (m_bOnBillbord == true)
	{
		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);//�r���[�}�g���b�N�X���擾

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		//�t�s������߂�	
		m_mtxWorld._11 = m_mtxView._11;
		m_mtxWorld._12 = m_mtxView._21;
		m_mtxWorld._13 = m_mtxView._31;
		m_mtxWorld._21 = m_mtxView._12;
		m_mtxWorld._22 = m_mtxView._22;
		m_mtxWorld._23 = m_mtxView._32;
		//m_mtxWorld._31 = m_mtxView._13;
		//m_mtxWorld._32 = m_mtxView._23;
		//m_mtxWorld._33 = m_mtxView._33;

	}
	else
	{
		//m_mtxWorld._11 = m_size.x;
		//m_mtxWorld._22 = m_size.y;


		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	}


	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);			//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, m_pTexture);	//�e�N�X�`���̐ݒ�

										//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,					//�J�n���钸�_�̃C���f�b�N�X
		2);					//�`�悷��v���~�e�B�u��
	if (m_bCulling == true)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O���s��Ȃ�
	}
	//���e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//�ʏ퍇���ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


}

void CEffect::SetCol_E(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}

void CEffect::Setpos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	m_size = scale;
	D3DXVECTOR3 posOrigin[4];
	D3DXVECTOR3 Rotatepos[4];

	//�X�P�[���T�C�Y���݂̂S���_
	posOrigin[0].x = -m_size.x;
	posOrigin[1].x = m_size.x;
	posOrigin[2].x = -m_size.x;
	posOrigin[3].x = m_size.x;

	posOrigin[0].y = -m_size.y;
	posOrigin[1].y = -m_size.y;
	posOrigin[2].y = m_size.y;
	posOrigin[3].y = m_size.y;

	//��]�̌v�Z
	Rotatepos[0].x = (posOrigin[0].x * cosf(m_fAng)) + (posOrigin[0].y * sinf(m_fAng));
	Rotatepos[1].x = (posOrigin[1].x * cosf(m_fAng)) + (posOrigin[1].y * sinf(m_fAng));
	Rotatepos[2].x = (posOrigin[2].x * cosf(m_fAng)) + (posOrigin[2].y * sinf(m_fAng));
	Rotatepos[3].x = (posOrigin[3].x * cosf(m_fAng)) + (posOrigin[3].y * sinf(m_fAng));

	Rotatepos[0].y = (posOrigin[0].x * sinf(m_fAng)) - (posOrigin[0].y * cosf(m_fAng));
	Rotatepos[1].y = (posOrigin[1].x * sinf(m_fAng)) - (posOrigin[1].y * cosf(m_fAng));
	Rotatepos[2].y = (posOrigin[2].x * sinf(m_fAng)) - (posOrigin[2].y * cosf(m_fAng));
	Rotatepos[3].y = (posOrigin[3].x * sinf(m_fAng)) - (posOrigin[3].y * cosf(m_fAng));

	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(Rotatepos[0].x, Rotatepos[0].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Rotatepos[1].x, Rotatepos[1].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Rotatepos[2].x, Rotatepos[2].y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Rotatepos[3].x, Rotatepos[3].y, 0.0f);

	m_pVtxBuff->Unlock();

}
//�o�����ɕs�����ɂȂ��Ă�������
void CEffect::AddColA()
{
	m_fAddA += 0.1f;
	if (m_fAddA >= 1.0f)
	{
		m_fAddA = 1.0f;
		m_bAddColA = false;
	}
	SetCol_E({ m_col.r,m_col.g,m_col.b,m_fAddA });
}



