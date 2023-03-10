#include "stdafx.h"
#include "TriggerTestScene.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "Logger.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw,
	Jump
};

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Materials
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.3f, 0.3f, 0.2f);
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
	m_pSphere->Translate(0.f, 5.f, 0.f);

	//Input
	auto input = m_SceneContext.GetInput();

	input->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Jump,InputTriggerState::pressed,VK_SPACE,-1,0U });

	//*** Triggers **/

	XMFLOAT3 triggerSize = { 2.f,2.f,2.f };

	PxBoxGeometry triggerGeo = PxBoxGeometry{ triggerSize.x / 2.f,triggerSize.y /2.f,triggerSize.z /2.f};

	m_pTriggerActorLeft = pPhysX->createRigidStatic(PxTransform{ -10.f,triggerSize.y /2.f,0.f });
	auto pLeftShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerActorLeft, triggerGeo, *pDefaultMaterial);

	pLeftShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pLeftShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerActorLeft);

	m_pTriggerActorRight = pPhysX->createRigidStatic(PxTransform{ 10.f,triggerSize.y / 2.f,0.f });
	auto pRightShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerActorRight, triggerGeo, *pDefaultMaterial);

	pRightShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pRightShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerActorRight);
}

void TriggerTestScene::Update()
{
	const float deltaTime = m_SceneContext.GetGameTime()->GetElapsed();
	float force{ 100000.f * deltaTime };
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
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Jump) || m_isRightTrigger)
	{
		m_pSphereActor->addForce(PxVec3{ 0.0f,jumpForce,0.0f }, PxForceMode::eIMPULSE);
		m_isRightTrigger = false;
	}
}

void TriggerTestScene::Draw() const
{
}

void TriggerTestScene::OnSceneActivated()
{
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
	{
		//ignore pairs when shapes have been removed
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
			continue;

		const PxTriggerPair& triggerPair = pairs[i];
		const auto pTriggerActor = triggerPair.triggerActor;
		
		//On enter
		if (triggerPair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (pTriggerActor == m_pTriggerActorLeft)
			{
				Logger::GetInstance()->LogInfo(L"Sphere entered left trigger");
			}

			if (pTriggerActor == m_pTriggerActorRight)
			{
				Logger::GetInstance()->LogInfo(L"Sphere entered right trigger");
				m_isRightTrigger = true;

			}
		}

		//On leave
		if (triggerPair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (pTriggerActor == m_pTriggerActorLeft)
			{
				Logger::GetInstance()->LogInfo(L"Sphere left left trigger");
			}

			if (pTriggerActor == m_pTriggerActorRight)
			{
				Logger::GetInstance()->LogInfo(L"Sphere left right trigger");

			}
		}


	}

	

}
