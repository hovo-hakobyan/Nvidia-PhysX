#include "stdafx.h"
#include "BouncingBallScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Default material
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 1.f);

	//GROUNDPLANE (MOSTLY DEFAULT CODE)
	m_pGroundPlane= pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo,PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*m_pGroundPlane, PxPlaneGeometry{}, *pDefaultMaterial);

	//Add groundplane to physx scene
	m_pPhysxScene->addActor(*m_pGroundPlane);

	//Materials for spheres
	const PxMaterial* pSphereMat1 = pPhysX->createMaterial(.5f, .5f, 1.f);
	const PxMaterial* pSphereMat2 = pPhysX->createMaterial(.5f, .5f, 0.5f);
	const PxMaterial* pSphereMat3 = pPhysX->createMaterial(.5f, .5f, 0.f);

	//Sphere properties
	const float radius{ 1.f};
	const int slices = 30;
	const int stacks = 30;

	//Sphere creation
	m_Spheres[0] = new SpherePosColorNorm(radius, slices, stacks, XMFLOAT4{ Colors::Beige });
	AddGameObject(m_Spheres[0]);

	m_Spheres[1] = new SpherePosColorNorm(radius, slices, stacks, XMFLOAT4{ Colors::Beige });
	AddGameObject(m_Spheres[1]);

	m_Spheres[2] = new SpherePosColorNorm(radius, slices, stacks, XMFLOAT4{ Colors::Beige });
	AddGameObject(m_Spheres[2]);

	//Actors
	PxRigidDynamic* pSphereActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	//Geometry (can reuse)
	PxSphereGeometry pSphereGeometry = PxSphereGeometry{ radius };

	//Connect geometry with actor
	PxRigidActorExt::createExclusiveShape(*pSphereActor1, pSphereGeometry, *pSphereMat1);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, pSphereGeometry, *pSphereMat2);
	PxRigidActorExt::createExclusiveShape(*pSphereActor3, pSphereGeometry, *pSphereMat3);

	//Attach actor to sphere
	m_Spheres[0]->AttachRigidActor(pSphereActor1);
	m_Spheres[0]->Translate(-10.f, 10.f, 0.f);

	m_Spheres[1]->AttachRigidActor(pSphereActor2);
	m_Spheres[1]->Translate(0.f, 10.f, 0.f);

	m_Spheres[2]->AttachRigidActor(pSphereActor3);
	m_Spheres[2]->Translate(10.f, 10.f, 0.f);

	
}

void BouncingBallScene::Update()
{
}

void BouncingBallScene::Draw() const
{
}

void BouncingBallScene::OnSceneActivated()
{
}

void BouncingBallScene::OnSceneDeactivated()
{
}
