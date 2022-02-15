//----------------------------------
//�A�j���[�V�����t����Xfile�̓ǂݍ��݂����邽�߂̃C���^�[�t�F�[�X�̏���
//�쐬�ҁF�с@�C�l
//----------------------------------
#include "MyAllocateHierarchy.h"

CMyAllocateHierarchy::CMyAllocateHierarchy()
{
}

CMyAllocateHierarchy::~CMyAllocateHierarchy()
{
}
HRESULT CMyAllocateHierarchy::CreateFrame(THIS_
	LPCSTR Name,
	LPD3DXFRAME *ppNewFrame)
{
	// �t���[����V������������
	D3DXFRAME *p = new D3DXFRAME;
	ZeroMemory(p, sizeof(D3DXFRAME));
	p->Name = new char[strlen(Name) + 1];
	strcpy(p->Name, Name);

	*ppNewFrame = p;

	return D3D_OK;
}


HRESULT CMyAllocateHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	// ���b�V���R���e�i�I�u�W�F�N�g�̐���
	D3DXMESHCONTAINER *p = new D3DXMESHCONTAINER;
	p->MeshData = *pMeshData;
	p->Name = new char[strlen(Name) + 1];
	strcpy(p->Name, Name);
	p->MeshData.Type = pMeshData->Type;

	// �ʏ탁�b�V��
	if (pMeshData->Type == D3DXMESHTYPE_MESH) {
		p->MeshData.pMesh = pMeshData->pMesh;
		p->MeshData.pMesh->AddRef();
	}

	// �v���O���b�V�u���b�V��
	else if (pMeshData->Type == D3DXMESHTYPE_PMESH) {
		p->MeshData.pPMesh = pMeshData->pPMesh;
		p->MeshData.pPMesh->AddRef();
	}

	// �p�b�`���b�V��
	else {
		p->MeshData.pPatchMesh = pMeshData->pPatchMesh;
		p->MeshData.pPatchMesh->AddRef();
	}
	// �z��̊m��
	p->pMaterials = new D3DXMATERIAL[NumMaterials];

	for (int i = 0; i<NumMaterials; i++) {
		p->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
		if (pMaterials[i].pTextureFilename != NULL)
		{
			p->pMaterials[i].pTextureFilename = new char[strlen(pMaterials[i].pTextureFilename) + 1];
			strcpy(p->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
		}
	}
	const D3DXEFFECTINSTANCE *pEI = pEffectInstances;  // �����̂�
	p->pEffects = new D3DXEFFECTINSTANCE;
	p->pEffects->pEffectFilename = pEI->pEffectFilename;
	p->pEffects->NumDefaults = pEI->NumDefaults;
	p->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEI->NumDefaults];

	D3DXEFFECTDEFAULT *pDIST = pEI->pDefaults; // �R�s�[��
	D3DXEFFECTDEFAULT *pCOPY = p->pEffects->pDefaults; // �R�s�[��
	for (unsigned int i = 0; i<pEI->NumDefaults; i++)
	{
		pCOPY[i].pParamName = pDIST[i].pParamName;
		DWORD NumBytes = pCOPY[i].NumBytes = pDIST[i].NumBytes;
		pCOPY[i].Type = pDIST[i].Type;
		if (pDIST[i].Type <= D3DXEDT_DWORD) {
			pCOPY[i].pValue = new DWORD[NumBytes];
			memcpy(pCOPY[i].pValue, pDIST[i].pValue, NumBytes);
		}
	}
	p->NumMaterials = NumMaterials;
	// �|���S����
	DWORD NumPolygon = pMeshData->pMesh->GetNumFaces();

	// �z��̊m��
	p->pAdjacency = new DWORD[NumPolygon * 3];

	// �R�s�[
	memcpy(p->pAdjacency, pAdjacency, NumPolygon * 3 * sizeof(DWORD));
	p->pSkinInfo = pSkinInfo;
	if (p->pSkinInfo != NULL)
	{
		p->pSkinInfo->AddRef();
	}

	*ppNewMeshContainer = p;
	return S_OK;
}


HRESULT CMyAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree->Name)
	{
		delete[] pFrameToFree->Name;
	}
	if (pFrameToFree->pMeshContainer)
	{
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	delete pFrameToFree;

	return D3D_OK;
}


HRESULT CMyAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER *p = pMeshContainerToFree;   // �����̂�

	delete[] p->Name;
	//SAFE_RELEASE(p->MeshData.pMesh)
		for (unsigned int i = 0; i<p->NumMaterials; i++) {
			delete[] p->pMaterials[i].pTextureFilename;
		}
	delete[] p->pMaterials;

	// �G�t�F�N�g
	int i;
	for (i = 0; i<p->pEffects->NumDefaults; i++) {
		delete[] p->pEffects->pDefaults[i].pParamName;
		delete[] p->pEffects->pDefaults[i].pValue;
	}

	delete[] p->pEffects->pEffectFilename;
	delete[] p->pEffects->pDefaults;
	delete p->pEffects;

	delete[] p->pAdjacency;
	//SAFE_RELEASE(p->pSkinInfo);

	return D3D_OK;
}