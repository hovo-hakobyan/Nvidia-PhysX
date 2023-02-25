#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw,
	Jump
};


void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Materials
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.3f, 0.3f, 0.2f);
	const PxMaterial* pBrickMaterial = pPhysX->createMaterial(0.4f, 0.4f, 0.f);
	const PxMaterial* pSphereMaterial = pPhysX->createMaterial(1.f, 1.f, 0.5f);

	//***Ground**/
	const XMFLOAT3 groundDimensions{ 100.f,0.1f,100.f };
	GameObject* pGround = new CubePosColorNorm(groundDimensions.x, groundDimensions.y, groundDimensions.z, XMFLOAT4{ Colors::LightGray });
	AddGameObject(pGround);

	//Ground actor
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });

	//Ground geometry
	PxBoxGeometry groundGeometry = PxBoxGeometry{ groundDimensions.x / 2.f,groundDimensions.y / 2.f, groundDimensions.z / 2.f };

	//Connect
	PxRigidActorExt::createExclusiveShape(*pGroundActor, groundGeometry, *pDefaultMaterial);

	//Link to shape
	pGround->AttachRigidActor(pGroundActor);

	//***Wall**/
	m_Bricks.resize(m_NrCubesWidth * m_NrCubesHeight);

	//Geometry
	PxBoxGeometry brickGeo = PxBoxGeometry{ m_BrickDimensions.x / 2.f, m_BrickDimensions.y / 2.f, m_BrickDimensions.z / 2.f };

	//Wall Creation
	for (int i = 0; i < m_NrCubesWidth; i++)
	{
		for (int j = 0; j < m_NrCubesHeight; j++)
		{
			int idx = i * m_NrCubesWidth + j;
			m_Bricks[idx] = new CubePosColorNorm(m_BrickDimensions.x, m_BrickDimensions.y, m_BrickDimensions.z, XMFLOAT4{Colors::SaddleBrown});
			AddGameObject(m_Bricks[idx]);

			//Actor
			PxRigidDynamic* pBrickActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

			//Connect Geometry to actor
			PxRigidActorExt::createExclusiveShape(*pBrickActor, brickGeo, *pBrickMaterial);

			PxRigidBodyExt::setMassAndUpdateInertia(*pBrickActor, 1.f);

			m_Bricks[idx]->AttachRigidActor(pBrickActor);
			m_Bricks[idx]->Translate(m_WallStartPos.x + j * m_BrickDimensions.x * m_DistanceBetweenWallCol, i * m_BrickDimensions.y + m_BrickDimensions.y / 2.f + m_HeightToDropWall, m_WallStartPos.z);

			int randDeg = std::rand() % 30;
			m_Bricks[idx]->RotateDegrees(0.f, static_cast<float>(randDeg), 0.f);
		}
	}

	//***Sphere **/

		//Sphere info
	const float radius{ 1.5f };
	const int stacks{ 60 };
	const int slices{ 60 };

	//Sphere object creation
	m_pSphere = new SpherePosColorNorm{ radius,slices,stacks,XMFLOAT4{Colors::Yellow} };
	AddGameObject(m_pSphere);

	//Sphere actor
	m_pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidBodyExt::setMassAndUpdateInertia(*m_pSphereActor, 20.f);

	//Geometry
	PxSphereGeometry sphereGeo = PxSphereGeometry{ radius };

	//Connect geometry to actor
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeo, *pSphereMaterial);

	m_pSphere->AttachRigidActor(m_pSphereActor);
	m_pSphere->Translate(0.f, 5.f, -40.f);

	//Input
	auto input = m_SceneContext.GetInput();

	input->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Jump,InputTriggerState::pressed,VK_SPACE,-1,0U });

	//Camera
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.f,10.f,-80.f });
	
}

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetScene();
	}

	const float deltaTime = m_SceneContext.GetGameTime()->GetElapsed();
	float force{ 100000.f * deltaTime};
	float jumpForce{ 50000.f * deltaTime };

	const XMFLOAT3 camForward{ m_SceneContext.GetCamera()->GetForward() };
	const XMFLOAT3 camRight{ m_SceneContext.GetCamera()->GetRight() };

	const PxVec3 forward{ camForward.x,camForward.y,camForward.z };
	const PxVec3 right{ camRight.x,camRight.y,camRight.z };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_pSphereActor->addTorque(forward * force);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_pSphereActor->addTorque(forward * -force);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Forw))
	{
		m_pSphereActor->addTorque(right * force);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Back))
	{
		m_pSphereActor->addTorque(right * -force);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Jump))
	{
		m_pSphereActor->addForce(PxVec3{ 0.0f,jumpForce,0.0f }, PxForceMode::eIMPULSE);
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{

}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::ResetScene()
{
	for (int i = 0; i < m_NrCubesWidth; i++)
	{
		for (int j = 0; j < m_NrCubesHeight; j++)
		{
			int idx = i * m_NrCubesWidth + j;
			m_Bricks[idx]->Translate(m_WallStartPos.x + j * m_BrickDimensions.x * m_DistanceBetweenWallCol, i * m_BrickDimensions.y + m_BrickDimensions.y / 2.f + m_HeightToDropWall, m_WallStartPos.z);

			int randDeg = std::rand() % 30;
			m_Bricks[idx]->RotateDegrees(0.f, static_cast<float>(randDeg), 0.f);
		}
	}

	m_pSphere->Translate(0.f, 5.f, -40.f);

}


