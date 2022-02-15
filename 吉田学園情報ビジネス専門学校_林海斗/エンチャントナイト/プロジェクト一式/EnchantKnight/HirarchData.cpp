#include "HierarchyData.h"

//map<string, LPDIRECT3DTEXTURE9> g_TextureList;

HRESULT HierarchyData::CreateFrame(THIS_ LPCSTR name, LPD3DXFRAME *new_frame)
{
	// 自作フレームの作成
	FrameData *frame = new FrameData();
	ZeroMemory(frame, sizeof(FrameData));
	*new_frame = frame;

	// 名前のコピー
	frame->Name = new char[lstrlenA(name) + 1];
	strcpy_s(frame->Name, lstrlenA(name) + 1, name);

	return S_OK;
}

HRESULT HierarchyData::CreateMeshContainer(
	THIS_ LPCSTR name,
	CONST D3DXMESHDATA *mesh_data,
	CONST D3DXMATERIAL *material_data,
	CONST D3DXEFFECTINSTANCE *effect_instance,
	DWORD material_num,
	CONST DWORD *adjacency,
	LPD3DXSKININFO skin_info,
	LPD3DXMESHCONTAINER *new_mesh_container)
{
	// メッシュの取得
	LPD3DXMESH mesh = mesh_data->pMesh;
	// 自作メッシュコンテナの作成
	MeshContainer *container = new MeshContainer;
	// デバイス
	LPDIRECT3DDEVICE9 device = NULL;
	// ポリゴンの数
	int porigon_num = 0;

	if (container == NULL)
	{
		return E_FAIL;
	}
	ZeroMemory(container, sizeof(MeshContainer));

	// 名前のコピー
	container->Name = new char[lstrlenA(name) + 1];
	strcpy_s(container->Name, lstrlenA(name) + 1, name);

	// ポリゴン数の取得
	porigon_num = mesh->GetNumFaces();

	// デバイスの取得
	mesh->GetDevice(&device);

	// メッシュのコピー
	container->MeshData.pMesh = mesh;
	container->MeshData.Type = D3DXMESHTYPE_MESH;
	// 参照カウンタを加算する
	mesh->AddRef();

	// 隣接ポリゴン番号のコピー
	container->pAdjacency = new DWORD[porigon_num * 3];
	memcpy(container->pAdjacency, adjacency, sizeof(DWORD) * porigon_num * 3);

	// マテリアルのコピー
	container->NumMaterials = material_num;
	if (material_num > 0)
	{
		container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
		container->m_TextureList = new LPDIRECT3DTEXTURE9[container->NumMaterials];
		memcpy(container->pMaterials, material_data, sizeof(D3DXMATERIAL) * material_num);

		// ファイル読み込み
		for (int i = 0; i < material_num; i++)
		{
			//if (container->pMaterials[i].pTextureFilename != NULL)
			//{
			//	//if (g_TextureList[container->pMaterials[i].pTextureFilename] == NULL)
			//	//{
			//	//	LPDIRECT3DTEXTURE9 texture;

			//	//	if (SUCCEEDED(D3DXCreateTextureFromFile(
			//	//		device,
			//	//		container->pMaterials[i].pTextureFilename,
			//	//		&texture)))
			//	//	{
			//	//		g_TextureList[container->pMaterials[i].pTextureFilename] = texture;
			//	//		container->m_TextureList[i] = texture;
			//	//	} 
			//	//	else 
			//	//	{
			//	//		container->m_TextureList[i] = g_TextureList[container->pMaterials[i].pTextureFilename];
			//	//	}
			//	//} 

			//}
			//else
			//{
			//	container->m_TextureList[i] = NULL;
			//}
		}
	} 
	else 
	{
		// マテリアルの数が0だったら時の仮のマテリアル追加
		container->NumMaterials = 1;
		container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
		container->m_TextureList = NULL;
		container->pMaterials[0].pTextureFilename = NULL;
		memset(&container->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		container->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.a = 1.0f;
		container->pMaterials[0].MatD3D.Ambient = container->pMaterials[0].MatD3D.Diffuse;
	}

	*new_mesh_container = container;

	return S_OK;
}

HRESULT HierarchyData::DestroyFrame(THIS_ LPD3DXFRAME frame)
{
	if (frame != NULL)
	{
		if (frame->Name != NULL)
		{
			delete[](frame->Name);
		}
		delete(frame);
	}
	return S_OK;
}

HRESULT HierarchyData::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER base)
{
	MeshContainer *container = (MeshContainer*)base;

	if (container->Name != NULL)
	{
		delete[] (container->Name);
	}

	if (container->pAdjacency != NULL)
	{
		delete[](container->pAdjacency);
	}

	if (container->pMaterials != NULL)
	{
		delete[](container->pMaterials);
	}

	if (container->NumMaterials > 0)
	{
		for (int i = 0; i < container->NumMaterials; i++)
		{
			if (container->m_TextureList[i] != NULL)
			{
				container->m_TextureList[i]->Release();
				container->m_TextureList[i] = NULL;
			}
		}
		delete[](container->m_TextureList);
	}

	if (container->MeshData.pMesh != NULL)
	{
		container->MeshData.pMesh->Release();
	}

	delete(container);

	return S_OK;
}

