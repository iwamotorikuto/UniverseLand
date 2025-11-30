#include "stdafx.h"
#include "BackGround3.h"

BackGround3::BackGround3()
{
	m_modelRender.Init("Assets/modelData/Map/Stage3.tkm");
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix());
	m_modelRender.Update();
}

BackGround3::~BackGround3()
{

}

void BackGround3::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB
	m_modelRender.Draw(rc);
}
