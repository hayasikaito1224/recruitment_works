//=============================================================================
// �G�t�F�N�g�̏���
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "scene.h"
#include "texture.h"

class CEffect : public CScene
{
public:
	typedef enum
	{
		VectorDelete = 0,//�x�N�g���̒����ŏ�����
		TimeDelete		//���Ԃŏ�����
	}DeleteType;
	CEffect(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CEffect();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetCol_E(D3DXCOLOR col);
	void Setpos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);//���_���ݒ�
	void SetAng(float Ang) { m_fAng = Ang; }
	void SetCulling(bool bCulling) { m_bCulling = bCulling; }
	void bOnBill(bool bOnBillbord) { m_bOnBillbord = bOnBillbord; }//�r���{�[�h��L���ɂ��邩���Ȃ���
	void AddColA();
	/*
	 �ʒu�̐ݒ�,
	 �ړ���,
	 �T�C�Y,
	 �������Ȃ�X�s�[�h
	 Texture
	*/
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 size, D3DXCOLOR col,bool bGravity,
		float fGravity, float fDefSpeed, bool bIsBill,CTexture::Type type = CTexture::Text,float fAng = 0.0f,
		bool bScele=false,bool bScaleX=false,bool bDef = false);
	
private:
	float m_fGravity;//�d��
	bool m_bGravity;
	DeleteType m_DeleteType;//�G�t�F�N�g��������^�C�v
	float		m_fDeleteTime;
	float		m_fDeleteVec;
	float		m_fDeleteMaxTime;
	float		m_fDeleteMaxVec;
	float		m_fDefSpeedA;
	float		m_fAddA;
	bool m_bDefScale;//�������Ȃ锻��
protected:
	D3DXVECTOR3 m_pos,m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_initsize;

	D3DXCOLOR m_col;//�F
	float m_fScaleSpeed;//�T�C�Y������������ϐ�
	int m_nLife;//����
	float m_fRand;
	float m_fAng;
	CTexture::Type m_TexType;
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;									//���[���h�}�g���b�N�X	
	D3DXMATRIX m_mtxView;									//�r���[�}�g���b�N�X
	bool		m_bUninit;//��������
	bool		m_bOnBillbord;
	bool		m_bCulling;
	bool		m_bAddColA;
	bool		m_bScale;
	bool		m_bScaleX;

};


#endif // !_EFFECT_H_

