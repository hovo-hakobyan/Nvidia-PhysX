#include "stdafx.h"
#include "W2_Assignment.h"
#include "MeshObject.h"
#include "ContentManager.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"
#include "SoundManager.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw,
	Jump
};

void W2_Assignment::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//*** Materials **/
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.3f, 0.3f, 0.2f);

	//*** Ground **/
	auto pGroundPlane = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo,PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundPlane, PxPlaneGeometry{}, *pDefaultMaterial);

	//Add groundplane to physx scene
	m_pPhysxScene->addActor(*pGroundPlane);

	//*** Level **/
	m_pLevel = new MeshObject(L"Resources/Meshes/Level.ovm");
	AddGameObject(m_pLevel);

	const auto pLevelMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pLevelActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pLevelActor, PxTriangleMeshGeometry{ pLevelMesh }, *pDefaultMaterial);
	m_pLevel->AttachRigidActor(pLevelActor);

	//*** Boxes **/
	const XMFLOAT3 boxSize{ 1.f,1.f,1.f };

	//Box geometry
	PxBoxGeometry boxGeo = PxBoxGeometry{ boxSize.x / 2.f,boxSize.y / 2.f , boxSize.z / 2.f };

	//Red box
	m_pRedBox = new CubePosColorNorm(boxSize.x, boxSize.y, boxSize.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pRedBox);

	//Actor
	PxRigidDynamic* pRedBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity});

	//Create shape
	PxRigidActorExt::createExclusiveShape(*pRedBoxActor, boxGeo, *pDefaultMaterial);
	
	m_pRedBox->AttachRigidActor(pRedBoxActor);
	m_pRedBox->Translate(4.f, 7.f, 0.f);

	//BlueBox
	m_pBlueBox = new CubePosColorNorm(boxSize.x, boxSize.y, boxSize.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBlueBox);

	//Actor
	PxRigidDynamic* pBlueBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	//Create shape
	PxRigidActorExt::createExclusiveShape(*pBlueBoxActor, boxGeo, *pDefaultMaterial);

	m_pBlueBox->AttachRigidActor(pBlueBoxActor);
	m_pBlueBox->Translate(-4.f, 7.f, 0.f);

	//*** Hatches **/
	const XMFLOAT3 hatchSize{ 2.5f,0.3f,5.f };
	PxBoxGeometry hatchGeo = PxBoxGeometry{ hatchSize.x / 2.f, hatchSize.y / 2.f, hatchSize.z / 2.f };
	//Blue
	m_pBlueHatch = new CubePosColorNorm(hatchSize.x, hatchSize.y, hatchSize.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBlueHatch);
	auto pBlueHatchActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pBlueHatchActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pBlueHatchActor, hatchGeo, *pDefaultMaterial);
	m_pBlueHatch->AttachRigidActor(pBlueHatchActor);
	m_pBlueHatch->Translate(-9.f, 17.f, 0.f);

	//Red
	m_pRedHatch = new CubePosColorNorm(hatchSize.x, hatchSize.y, hatchSize.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pRedHatch);
	auto pRedHatchActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pRedHatchActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pRedHatchActor, hatchGeo, *pDefaultMaterial);
	m_pRedHatch->AttachRigidActor(pRedHatchActor);
	m_pRedHatch->Translate(9.f, 17.f, 0.f);

	//*** Triggers **/
	XMFLOAT3 triggerSize = { 2.f,0.3f,5.f };
	PxBoxGeometry triggerGeo = PxBoxGeometry{ triggerSize.x / 2.f, triggerSize.y / 2.f, triggerSize.z / 2.f };

	//Red trigger
	m_pTriggerBoxRed = new CubePosColorNorm(triggerSize.x, triggerSize.y, triggerSize.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pTriggerBoxRed);

	m_pTriggerActorRed = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pRedTrShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerActorRed, triggerGeo, *pDefaultMaterial);

	m_pTriggerBoxRed->AttachRigidActor(m_pTriggerActorRed);
	m_pTriggerBoxRed->Translate(6.5f, 2.2f, 0.f);

	pRedTrShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pRedTrShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	//Blue trigger
	m_pTriggerBoxBlue = new CubePosColorNorm(triggerSize.x, triggerSize.y, triggerSize.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pTriggerBoxBlue);

	m_pTriggerActorBlue = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pBlueTrShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerActorBlue, triggerGeo, *pDefaultMaterial);

	m_pTriggerBoxBlue->AttachRigidActor(m_pTriggerActorBlue);
	m_pTriggerBoxBlue->Translate(-7.5f, 2.2f, 0.f);

	pBlueTrShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pBlueTrShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	//*** Sphere **/
	
	//Sphere info
	const float radius{ 1.f };
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
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeo, *pDefaultMaterial);

	m_pSphere->AttachRigidActor(m_pSphereActor);
	m_pSphere->Translate(0.f, 5.f, 0.f);

	//*** Input **/
	auto input = m_SceneContext.GetInput();

	input->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Jump,InputTriggerState::pressed,VK_SPACE,-1,0U });

	//*** Sounds **/
	const auto pFmod = SoundManager::GetInstance()->GetSystem();
	
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_2D | FMOD_LOOP_OFF, nullptr, &m_pBellSound);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(m_pBellSound, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	
}

void W2_Assignment::Update()
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
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Jump))
	{
		m_pSphereActor->addForce(PxVec3{ 0.0f,jumpForce,0.0f }, PxForceMode::eIMPULSE);
	}

	if (m_isBlueTrigger)
	{
		SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
		m_isBlueTrigger = false;
	}
	if (m_isRedTrigger)
	{
		SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
		m_isRedTrigger = false;
	}
}

void W2_Assignment::Draw() const
{
}

void W2_Assignment::OnSceneActivated()
{
}

void W2_Assignment::OnSceneDeactivated()
{
}

void W2_Assignment::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
	{
		//ignore pairs when shapes have been removed
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
			continue;

		const PxTriggerPair& triggerPair = pairs[i];
		const auto pTriggerActor = triggerPair.triggerActor;
		const auto pOtherActor = triggerPair.otherShape->getActor();

		//If we enter the trigger
		if (triggerPair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//If the entered trigger is the blue one
			if (pTriggerActor == m_pTriggerActorBlue)
			{
				//if the entered actor is the blue actor
				if (pOtherActor == m_pBlueBox->GetRigidActor())
				{
					m_isBlueTrigger = true;
					
				}
			}

			//if the entered trigger is the red one
			if (pTriggerActor == m_pTriggerActorRed)
			{
				//if the entered actor is the red actor
				if (pOtherActor == m_pRedBox->GetRigidActor())
				{
					m_isRedTrigger = true;
				}
			}
		}

		
	}
}
