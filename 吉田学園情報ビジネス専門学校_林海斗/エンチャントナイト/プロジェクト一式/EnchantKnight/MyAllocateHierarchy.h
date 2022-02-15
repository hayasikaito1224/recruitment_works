//----------------------------------
//アニメーション付きのXfileの読み込みをするためのインターフェースの処理
//作成者：林　海斗
//----------------------------------
#ifndef _MYALLOCATEHIERARCHY_H
#define _MYALLOCATEHIERARCHY_H
#include "main.h"



class CMyAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	CMyAllocateHierarchy();
	virtual ~CMyAllocateHierarchy();

	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,
		LPD3DXFRAME *ppNewFrame
		);

	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		);

	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree
		);

	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree
		);
};
#endif
