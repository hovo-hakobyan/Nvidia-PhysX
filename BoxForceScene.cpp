#include "stdafx.h"
#include "BoxForceScene.h"
#include "CubePosColorNorm.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw
};

void BoxForceScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.f);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Materials
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.25f, 0.25f, 0.f);

	//Groundplane
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo,PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	//Cube
	const XMFLOAT3 actorDimenstions{ 1.f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimenstions.x, actorDimenstions.y, actorDimenstions.z);
	AddGameObject(m_pCube);

	//Cube actor
	m_pActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	
	//Cube geometry
	PxBoxGeometry geometry = PxBoxGeometry{ actorDimenstions.x / 2,actorDimenstions.y / 2.f, actorDimenstions.z / 2.0f };

	//Connect
	PxRigidActorExt::createExclusiveShape(*m_pActor, geometry, *pDefaultMaterial);

	//Link to shape
	m_pCube->AttachRigidActor(m_pActor);
	Reset();


	//Input
	auto input = m_SceneContext.GetInput();

	input->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U});
	input->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U  });


}

void BoxForceScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}

	const float deltaTime = m_SceneContext.GetGameTime()->GetElapsed();
	float speed{ 50.f * deltaTime };

	//Takes z axis vector and rotates it to match the forward vector of the cube, using quaternion
	PxVec3 forward = m_pActor->getGlobalPose().q.rotate(PxVec3{ 0.f,0.f,1.f });
	PxVec3 right = m_pActor->getGlobalPose().q.rotate(PxVec3{ 1.f,0.f,0.f });

	PxForceMode::Enum forceMode = PxForceMode::eVELOCITY_CHANGE;

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_pActor->addForce(-right * speed, forceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_pActor->addForce(right * speed, forceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Forw))
	{
		m_pActor->addForce(forward * speed, forceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Back))
	{
		m_pActor->addForce(-forward * speed, forceMode);
	}


}

void BoxForceScene::Draw() const
{
}

void BoxForceScene::OnSceneActivated()
{
	Reset();
}

void BoxForceScene::OnSceneDeactivated()
{
}

void BoxForceScene::Reset()
{
	m_pCube->Translate(0.f, 1.f, 0.f);
}
