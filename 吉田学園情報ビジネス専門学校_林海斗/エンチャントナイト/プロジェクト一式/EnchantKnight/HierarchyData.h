#ifndef HIERARCHY_DATA_H_
#define HIERARCHY_DATA_H_
#include "main.h"
//#include <d3d9.h>
//#include <d3dx9.h>

// ����t���[���f�[�^
struct FrameData : public D3DXFRAME
{
	// �����s��
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

// ���상�b�V���R���e�i
struct MeshContainer : public D3DXMESHCONTAINER
{
	// �}�e���A���Ŏg�p����e�N�X�`��
	LPDIRECT3DTEXTURE9 *m_TextureList;
};

// �K�w�N���X(ID3DXAllocateHierarchy�Ŏg�����z�֐��̂ݒ�`)
class HierarchyData : public ID3DXAllocateHierarchy
{
public:
	HierarchyData() {}
	
	/*
		���b�V���R���e�i�̍쐬
			�߂�l�F
				HRESULT
					S_OK => ����
					E_FAIL => ���s

			�����F
				THIS_ LPCSTR name�F
			
				LPD3DXFRAME *new_frame�F

			���e�F
				D3DXLoadMeshHierarchyFromX�֐������s���ꂽ�ۂ�
				�e�t���[���̐������Ăяo�����֐�
				�����ɂ̓t���[���̖��O�ƃA�E�g�v�b�g�p�̒l���n����Ă���A
				����̃t���[�����쐬���ăt���[���f�[�^���R�s�[���A
				new_frame�ɓn��
	*/
	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,
		LPD3DXFRAME *ppNewFrame);

	/*
		���b�V���R���e�i�̍쐬
			�߂�l�F
				HRESULT
					S_OK => ����
					E_FAIL => ���s
			
			�����F
				THIS_ LPCSTR name�F
					���b�V���̖��O

				CONST D3DXMESHDATA *mesh_data�F
					���b�V���f�[�^

				CONST D3DXMATERIAL *material_data�F
					�}�e���A���f�[�^

				CONST D3DXEFFECTINSTANCE *effect_instance�F
					�G�t�F�N�g�f�[�^

				DWORD material_num�F
					�}�e���A���̐�

				CONST DWORD *adjacency�F
					�אڃ|���S���f�[�^

				LPD3DXSKININFO skin_info�F
					�X�L���f�[�^

				LPD3DXMESHCONTAINER *new_mesh_container�F
					�A�E�g�v�b�g�p���b�V���R���e�i�̃|�C���^

			���e�F
				D3DXLoadMeshHierarchyFromX�֐������s���ꂽ�ۂ�
				�e�t���[���̃R���e�i�̐������Ăяo�����֐�
				�����ɂ̓��b�V���R���e�i�Ɋi�[����Ă���f�[�^��
				�n����Ă���A���̃f�[�^������̃��b�V���R���e�i��
				�R�s�[����new_mesh_container�ɓn��
	*/
	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	/*
		�t���[���̍폜
			�߂�l�F
				HRESULT
					S_OK => ����
					E_FAIL => ���s
			
			�����F
				THIS_ LPD3DXFRAME frame�F
					�폜�Ώۂ̃t���[���f�[�^

			���e�F
				CreateFrame�œ��I�Ɋm�ۂ������������������

	*/
	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree);
 
	/*
		���b�V���R���e�i�̍폜
			�߂�l�F
				HRESULT
					S_OK => ����
					E_FAIL => ���s
			
			�����F
				THIS_ LPD3DXMESHCONTAINER base�F
					�폜�Ώۂ̃��b�V���R���e�i

			���e�F
				CreateMeshContainer�œ��I�Ɋm�ۂ������������������

	*/
	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree);
};

#endif
