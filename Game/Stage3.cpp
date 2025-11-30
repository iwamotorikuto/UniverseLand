#include "stdafx.h"
#include "Stage3.h"

Stage3::Stage3()
{
	m_modelRender.Init("Assets/modelData/Map/Stage3.tkm");
	//m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix());
}

Stage3::~Stage3()
{

}

void Stage3::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB
	m_modelRender.Draw(rc);
}
