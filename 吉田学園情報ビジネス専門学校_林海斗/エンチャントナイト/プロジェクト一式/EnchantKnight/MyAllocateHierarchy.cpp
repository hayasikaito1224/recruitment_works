//----------------------------------
//アニメーション付きのXfileの読み込みをするためのインターフェースの処理
//作成者：林　海斗
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
	// フレームを新しく生成する
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
	// メッシュコンテナオブジェクトの生成
	D3DXMESHCONTAINER *p = new D3DXMESHCONTAINER;
	p->MeshData = *pMeshData;
	p->Name = new char[strlen(Name) + 1];
	strcpy(p->Name, Name);
	p->MeshData.Type = pMeshData->Type;

	// 通常メッシュ
	if (pMeshData->Type == D3DXMESHTYPE_MESH) {
		p->MeshData.pMesh = pMeshData->pMesh;
		p->MeshData.pMesh->AddRef();
	}

	// プログレッシブメッシュ
	else if (pMeshData->Type == D3DXMESHTYPE_PMESH) {
		p->MeshData.pPMesh = pMeshData->pPMesh;
		p->MeshData.pPMesh->AddRef();
	}

	// パッチメッシュ
	else {
		p->MeshData.pPatchMesh = pMeshData->pPatchMesh;
		p->MeshData.pPatchMesh->AddRef();
	}
	// 配列の確保
	p->pMaterials = new D3DXMATERIAL[NumMaterials];

	for (int i = 0; i<NumMaterials; i++) {
		p->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
		if (pMaterials[i].pTextureFilename != NULL)
		{
			p->pMaterials[i].pTextureFilename = new char[strlen(pMaterials[i].pTextureFilename) + 1];
			strcpy(p->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
		}
	}
	const D3DXEFFECTINSTANCE *pEI = pEffectInstances;  // 長いので
	p->pEffects = new D3DXEFFECTINSTANCE;
	p->pEffects->pEffectFilename = pEI->pEffectFilename;
	p->pEffects->NumDefaults = pEI->NumDefaults;
	p->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEI->NumDefaults];

	D3DXEFFECTDEFAULT *pDIST = pEI->pDefaults; // コピー元
	D3DXEFFECTDEFAULT *pCOPY = p->pEffects->pDefaults; // コピー先
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
	// ポリゴン数
	DWORD NumPolygon = pMeshData->pMesh->GetNumFaces();

	// 配列の確保
	p->pAdjacency = new DWORD[NumPolygon * 3];

	// コピー
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
	D3DXMESHCONTAINER *p = pMeshContainerToFree;   // 長いので

	delete[] p->Name;
	//SAFE_RELEASE(p->MeshData.pMesh)
		for (unsigned int i = 0; i<p->NumMaterials; i++) {
			delete[] p->pMaterials[i].pTextureFilename;
		}
	delete[] p->pMaterials;

	// エフェクト
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