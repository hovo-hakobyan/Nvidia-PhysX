#include "stdafx.h"
#include "PhysXMeshTestScene.h"
#include "PhysxManager.h"
#include "MeshObject.h"
#include "ContentManager.h"
#include "CubePosColorNorm.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw
};


void PhysXMeshTestScene::Initialize()
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

	//***Convex Chair**/

	//Game obj
	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairConvex);

	//Mesh
	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");

	//Actor
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	//Connect mesh to actor
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);

	//WSet the mass to 10kg and the center of mass to be 4 units above the origin
	PxVec3 massLocalPose{ 0.f,4.f,0.f };
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPose);

	m_pChairConvex->AttachRigidActor(pConvexActor);

	m_pChairConvex->Translate(0.f, 12.f, 0.f);


	//***Triangle chair**/

	//Game obj
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairTriangle);

	//Mesh
	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");

	//Actor
	const auto pTrianglActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTrianglActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pTrianglActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTrianglActor);
	m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	m_KinematicPos = { 10.f,0.f,0.f };


	//Input
	auto input = m_SceneContext.GetInput();

	input->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	input->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U });

}


void PhysXMeshTestScene::Update()
{
	float speed{ 2.f };
	float elapsed{ m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_KinematicPos.x -= speed * elapsed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_KinematicPos.x += speed * elapsed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Forw))
	{
		m_KinematicPos.z += speed * elapsed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Back))
	{
		m_KinematicPos.z -= speed * elapsed;
	}

	m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPos });
}

void PhysXMeshTestScene::Draw() const
{
}

void PhysXMeshTestScene::OnSceneActivated()
{
}

void PhysXMeshTestScene::OnSceneDeactivated()
{
}
