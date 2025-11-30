#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	m_modelRender.Init("Assets/modelData/Map/Stage1.tkm");	
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix());
	m_modelRender.Update();
}

BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB
	m_modelRender.Draw(rc);
}
