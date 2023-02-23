#include "stdafx.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	//Wireframe toggle
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Default material
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f,0.5f,0.2f);

	//CUBE
	const XMFLOAT3 actorDimensions{ 1.f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x,actorDimensions.y,actorDimensions.z);
	AddGameObject(m_pCube);

	//CUBE ACTOR
	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	//Box Geomery (because we have a cube)
	//Need half extent to link this to the cube 
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimensions.x / 2.f,actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	/* LONG WAY
	//!isExclusive --> are we gonna share it with multiple actors?
	PxShape* pCubeShape = pPhysX->createShape(boxGeometry, *pDefaultMaterial, true);

	//Hey, Use this shape
	pCubeActor->attachShape(*pCubeShape);

	m_pPhysxScene->addActor(*pCubeActor);
	*/

	//Short way
	PxRigidActorExt::createExclusiveShape(*pCubeActor,boxGeometry,*pDefaultMaterial);

	//Link cube with cube actor
	m_pCube->AttachRigidActor(pCubeActor);
	m_pCube->Translate(0.f, 10.f, 0.f);
	m_pCube->RotateDegrees(30.f, 45.f, 30.f);

	//GROUNDPLANE (MOSTLY DEFAULT CODE)
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo,PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	//Toevoegen aan game object --> voor als je wil renderen
	//Omda we da nie doen moeten we zelf toevoegen aan physx scene
	m_pPhysxScene->addActor(*pGroundActor);
	
}


void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		//Also resets velocity
		m_pCube->Translate(0.f, 10.f, 0.f);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
