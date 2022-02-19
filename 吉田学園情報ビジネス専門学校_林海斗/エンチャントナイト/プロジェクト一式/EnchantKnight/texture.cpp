//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : �H�������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "texture.h"
#include "Renderer.h"
#include "manager.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����

	D3DXCreateTextureFromFile(pDevice,
		"",	// ����
		&m_pTexture[None]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/text_texture.png",//���e�N�X�`��
		&m_pTexture[Text]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Flag.png",//���e�N�X�`��
		&m_pTexture[GOAL]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",//����
		&m_pTexture[TIME]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/fade.png",//����
		&m_pTexture[FADE]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky_loop001.png",//��
		&m_pTexture[SKY]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//�e
		&m_pTexture[SHADOW]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Fire.png",//��
		&m_pTexture[FIRE]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MagicCircle_TypeB.png",//���@�w
		&m_pTexture[MagicCircle_TypeB]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ring001.png",//ring
		&m_pTexture[RingEffect]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/HeelEffect.jpg",//�񕜌n�̉摜
		&m_pTexture[LifeEffect]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//�G�t�F�N�g
		&m_pTexture[Effect]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GlitterEffect000.png",//�G�t�F�N�g
		&m_pTexture[GlitterEffect]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/hiteffect000.jpg",//�G�t�F�N�g
		&m_pTexture[HitEffect]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Trajectory000.png",//�G�t�F�N�g
		&m_pTexture[SwordEffect]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GaugeBer.png",//�Q�[�W
		&m_pTexture[GAUGEBER]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Field000.jpg",//��	
		&m_pTexture[FIELD]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Map_Player000.png",//��	
		&m_pTexture[Map_Player]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MapEnemy.png",//��	
		&m_pTexture[Map_Enemy]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Purpose000.png",//��	
		&m_pTexture[Map_Purpose]);
	//============================================================
	//COMMAND�֌W
	//============================================================
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Attack000.png",//��	
		&m_pTexture[COMMAND_ATTACK]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Magic000.png",//��	
		&m_pTexture[COMMAND_MAGIC]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Enhance000.png",//��	
		&m_pTexture[COMMAND_ENHANCE]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/CommandBG.png",//�R�}���h�̔w�i
		&m_pTexture[COMMAND_BG]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Blaze000.png",//�u���C�Y
		&m_pTexture[BLAZE]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Icecle000.png",//�A�C�V�N��
		&m_pTexture[ICECLE]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/COMMAND/Heel000.png",//�q�[��
		&m_pTexture[HEEL]);
	//============================================================
	//�{�^��
	//============================================================
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/A_button.png",//A�{�^��
		&m_pTexture[A_BUTTON]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/B_button.png",//A�{�^��
		&m_pTexture[B_BUTTON]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/X_button.png",//A�{�^��
		&m_pTexture[X_BUTTON]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/L_button.png",//A�{�^��
		&m_pTexture[L_BUTTON]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/SkillGauge000.png",//�Q�[�W
		&m_pTexture[SKILLGAUGE]);

	//============================================================
	//�^�C�g��
	//============================================================
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title000.png",//�^�C�g���w�i
		&m_pTexture[Title]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Please Enter.png",//�Q�[���X�^�[�g
		&m_pTexture[GameStart]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/operation_method000.png",//�`���[�g���A��(�Q�[�����ɕ\�����鑀����@)
		&m_pTexture[Operation]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Cursor.png",		  //�J�[�\��
		&m_pTexture[Cursol]);

	//============================================================
	//�Q�[���I�[�o�[
	//============================================================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/FAILED000.png",//�Q�[���I�[�o�[���S
		&m_pTexture[Gameover000]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result000.png",//�Q�[���N���A�w�i
		&m_pTexture[GameClear000]);
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Retry000.png",//���g���C
		&m_pTexture[Retry]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titleback.png",//�^�C�g���ɖ߂�
		&m_pTexture[TitleBack001]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Gameback.png",//�Q�[���ɖ߂�
		&m_pTexture[GameBack]);
	//============================================================
	//�Q�[�����
	//============================================================
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.png",//�`���[�g���A�����
		&m_pTexture[Operation01]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/targettext000.png",//�ړI�n�\���p��
		&m_pTexture[TargetText]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/stagecursor.png",//���b�N�I���p
		&m_pTexture[RockOn]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SkillOK000.png",//���b�N�I���p
		&m_pTexture[SkillOk]);

}

//=============================================================================
// �I��
//=============================================================================
void CTexture::Uniinit(void)
{
	for (int nCntTexture = 0; nCntTexture < Max; nCntTexture++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}