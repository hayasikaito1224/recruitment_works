//-------------------------------
//bumper�̃N���X�̏���
//�쐬�ҁF�ъC�l
//-------------------------------
#include "bumper.h"
#include "model_spawner.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#define SPEED_DEFF (1.5)//���x�̌����̑���
#define KNOCKBACK_SPEED (20.0)//�m�b�N�o�b�N�̑��x
#define CREATE_MODELTYPE (9)//���̃}�N���iallmodel.txt�ɏ����Ă���bumper�̃��f���̔ԍ����Q�Ƃ��āj
//----------------------------------------
//�R���X�g���N�^
//----------------------------------------
CBumper::CBumper(OBJTYPE nPriority) :CScene(nPriority)
{
}
//----------------------------------------
//�f�X�g���N�^
//----------------------------------------
CBumper::~CBumper()
{
}
//----------------------------------------
//������
//----------------------------------------

HRESULT CBumper::Init()
{
	//���f���̐���
	pModelSpawner = CModel_Spawner::Create(m_pos, m_rot, CREATE_MODELTYPE);
	m_fKnockBackSpeed = KNOCKBACK_SPEED;
	return S_OK;
}
//----------------------------------------
//�j��
//----------------------------------------

void CBumper::Uninit()
{
}
//----------------------------------------
//�X�V
//----------------------------------------

void CBumper::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

	D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = CManager::GetGame()->GetPlayer()->GetPos();				//�ʒu
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = pos - m_pos;			//�G�ƒe�̃x�N�g��
	float fLength = 0.0f;
	fLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	//���f���̑傫���͈̔͂Ƀv���C���[���������璵�˕Ԃ�
	if (fLength <= 100.0f)
	{
		m_bKnockBack = true;
	}

	//�m�b�N�o�b�N�̔��肪�I����������
	if (m_bKnockBack == true)
	{
		Knockback(pos, PlayerVec);

	}
	//�v���C���[�Ɉʒu�𔽉f
	pPlayer->SetPlayerPos(pos);

}
//----------------------------------------
//�`��
//----------------------------------------

void CBumper::Draw()
{
}
//----------------------------------------
//�C���X�^���X����
//----------------------------------------

CBumper * CBumper::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBumper *pBumper = new CBumper(OBJTYPE_MODEL);
	if (pBumper != nullptr)
	{
		pBumper->m_pos = pos;
		pBumper->m_rot = rot;
		pBumper->Init();
	}
	return nullptr;
}
//----------------------------------------
//�v���C���[�Ƀm�b�N�o�b�N�����鏈��
//----------------------------------------
void CBumper::Knockback(D3DXVECTOR3 & Playerpos,const D3DXVECTOR3& PlayerVec)
{
	float fAng = atan2(PlayerVec.x, PlayerVec.z);//�G�̌���
											 //�����o�����������߂�
	float fSpeed_x = sinf(fAng)*KNOCKBACK_SPEED;
	float fSpeed_z = cosf(fAng)*KNOCKBACK_SPEED;
	//�v���C���[�𓮂���
	Playerpos.x += fSpeed_x;
	Playerpos.z += fSpeed_z;
	m_fKnockBackSpeed -= SPEED_DEFF;
	//�m�b�N�o�b�N�X�s�[�h���Ȃ��Ȃ�����m�b�N�o�b�N���Ȃ��悤�ɂ���
	if (m_fKnockBackSpeed <= 0.0f)
	{
		m_fKnockBackSpeed = KNOCKBACK_SPEED;
		m_bKnockBack = false;
	}


}
