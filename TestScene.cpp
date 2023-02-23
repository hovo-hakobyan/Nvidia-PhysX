#include "stdafx.h"
#include "TestScene.h"
#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

enum InputIds 
{
	Up,
	Down,
	Left,
	Right
};

void TestScene::Initialize()
{
	//COBE
	m_pBox = new CubePosColorNorm(5.f, 2.f, 1.f);
	//Deals with memory leaks
	AddGameObject(m_pBox);

	m_pBox->Translate(0.f, 1.f, 0.f);
	m_pBox->RotateDegrees(0.f, 45.f, 0.f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.f,10.f,-25.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.f,-.3f,1.f });

	//TORUS
	auto pTorus = new TorusPosColorNorm(8.f, 50, 1.f, 10, XMFLOAT4{ Colors::Bisque });
	AddGameObject(pTorus);

	//CYLINDER
	const auto pCylinder = new CylinderPosColorNorm(1.5f, 10, 4.f);
	pTorus->AddChild(pCylinder);

	pTorus->Translate(20.f, 0.f, 0.f);
	pCylinder->Translate(0.f, 8.f, 0.f);

	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down, InputTriggerState::down,VK_DOWN,-1,XINPUT_GAMEPAD_DPAD_DOWN});
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up, InputTriggerState::down,VK_UP,-1,XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left, InputTriggerState::down,VK_LEFT,-1,XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right, InputTriggerState::down,VK_RIGHT,-1,XINPUT_GAMEPAD_DPAD_DOWN });

}

void TestScene::Update()
{
	const float totalTime = m_SceneContext.GetGameTime()->GetTotal();
	m_pBox->RotateDegrees(0.f, 90.f * totalTime, 0.f);

	XMFLOAT3 translation{};
	const float movementSpeed{ 10.f * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		translation.x -= movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		translation.x += movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		translation.y += movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Down))
	{
		translation.y -= movementSpeed;
	}

	
	//There are 2 types of vectors in DX, check it out
	XMStoreFloat3(&translation, XMVectorAdd(XMLoadFloat3(&translation),XMLoadFloat3(&m_pBox->GetPosition())));
	m_pBox->Translate(translation.x, translation.y, translation.z);

	
}

void TestScene::Draw() const
{
	
}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
