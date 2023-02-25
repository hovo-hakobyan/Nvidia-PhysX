#include "stdafx.h"
#include "FrictionTestScene.h"
#include "CubePosColorNorm.h"

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Materials
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.f);
	const PxMaterial* pNoFrictionMat = pPhysX->createMaterial(0.f, 0.f, 0.f);
	const PxMaterial* pHalfFrictionMat = pPhysX->createMaterial(0.4f, 0.4f, 0.f);
	const PxMaterial* pFullFrictionMat = pPhysX->createMaterial(1.f, 1.f, 0.f);
	const PxMaterial* pRampMat = pPhysX->createMaterial(0.f, 0.f, 0.f);

	//Groundplane
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo,PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);


	//** Cube Dynamic */
	const XMFLOAT3 DynamicCubeDimensions{ 1.f,1.f,1.f };
	const int nrCubes{ 3 };

	std::vector<GameObject*> pDynCubes{ nrCubes };

	for (int i = 0; i < nrCubes; i++)
	{
		pDynCubes[i]= new CubePosColorNorm(DynamicCubeDimensions.x, DynamicCubeDimensions.y, DynamicCubeDimensions.z);
		AddGameObject(pDynCubes[i]);
	}

	//Create Cube Actors
	std::vector<PxRigidDynamic*> pDynCubeActors{ nrCubes };

	for (int i = 0; i < nrCubes; i++)
	{
		pDynCubeActors[i] = pPhysX->createRigidDynamic(PxTransform{PxIdentity});	
	}

	//Create Cube Geometry (since same size we create 1)
	PxBoxGeometry cubeGeometry = PxBoxGeometry{ DynamicCubeDimensions.x / 2.f,DynamicCubeDimensions.y / 2.f, DynamicCubeDimensions.z / 2.f };
	

	//Connect Geometry with Actors
	PxRigidActorExt::createExclusiveShape(*pDynCubeActors[0], cubeGeometry, *pNoFrictionMat);
	PxRigidActorExt::createExclusiveShape(*pDynCubeActors[1], cubeGeometry, *pHalfFrictionMat);
	PxRigidActorExt::createExclusiveShape(*pDynCubeActors[2], cubeGeometry, *pFullFrictionMat);

	float distanceBetween{ 10.f };
	//Attach actor to shapes
	for (int i = 0; i < nrCubes; i++)
	{
		
		pDynCubes[i]->AttachRigidActor(pDynCubeActors[i]);
		pDynCubes[i]->Translate(distanceBetween * i, 10.f, 0.f);
		pDynCubes[i]->RotateDegrees(0.f, 0.f, -25.f);
		
	}

	//** Ramps */
	const XMFLOAT3 rampDimensions{ 5.f,1.f,2.f };
	const int nrRamps{ 3 };

	std::vector<GameObject*> pRamps{ nrRamps };

	for (int i = 0; i < nrRamps; i++)
	{
		pRamps[i] = new CubePosColorNorm(rampDimensions.x, rampDimensions.y, rampDimensions.z);
		AddGameObject(pRamps[i]);
	}

	//Create ramp actors
	std::vector<PxRigidStatic*> pRampActors{ nrRamps };
	for (int i = 0; i < nrRamps; i++)
	{
		pRampActors[i] = pPhysX->createRigidStatic(PxTransform{PxIdentity});
	}

	//Create ramp geometry
	PxBoxGeometry rampGeometry = PxBoxGeometry{ rampDimensions.x / 2.f,rampDimensions.y / 2.f, rampDimensions.z / 2.f };

	//Connect geometry with actors
	for (int i = 0; i < nrRamps; i++)
	{
		PxRigidActorExt::createExclusiveShape(*pRampActors[i], rampGeometry, *pRampMat);
	}

	//Attach actor to shapes
	for (int i = 0; i < nrRamps; i++)
	{
		pRamps[i]->AttachRigidActor(pRampActors[i]);
		pRamps[i]->Translate(distanceBetween * i, 5.f, 0.f);
		pRamps[i]->RotateDegrees(0.f, 0.f, -25.f);
	}
}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		for (int i = 0; i < 3; i++)
		{
			//Reset translation
		}
	}
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::OnSceneActivated()
{
}

void FrictionTestScene::OnSceneDeactivated()
{
}
