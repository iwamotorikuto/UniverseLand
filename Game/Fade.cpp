#include "stdafx.h"
#include "Fade.h"

namespace
{
	const Vector3 SCALE = Vector3(2.16f, 2.16f, 1.0f);
	const Vector3 POSITION = Vector3(-140.0f, 10.0f, 0.0f);
}

Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	m_spriteRender.Init("Assets/sprite/fade.dds", 1024.0f, 512.0f);
	m_spriteRender.SetScale(SCALE);
	m_spriteRender.SetPosition(POSITION);
	m_spriteRender.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
	case enState_FadeIn:
		m_currentAlpha -= 10.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 10.0f) {
			m_currentAlpha = 10.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut:
		m_currentAlpha += 10.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 10.0f) {
			m_currentAlpha = 10.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:

		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_currentAlpha > 0.0f) {
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_spriteRender.Draw(rc);
	}
}