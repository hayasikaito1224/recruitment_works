//===============================================-
//âÒïúñÇñ@ÇÃÉNÉâÉX
//===============================================
#ifndef _MAGIC_HEEL_H_
#define _MAGIC_HEEL_H_
#include "magic.h"
#define HEEL_MP (30)//MPè¡îÔó 
class CParticle_HeelEffect;
class C_Magic_Heel : public C_Magic
{
public:
	C_Magic_Heel(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Heel();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Heel *Create(D3DXVECTOR3 pos);

private:
	CParticle_HeelEffect *pEffect;
};
#endif // !_MAGIC_HEEL_H_
