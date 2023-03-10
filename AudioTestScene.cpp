#include "stdafx.h"
#include "AudioTestScene.h"

#include "PhysxManager.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "SoundManager.h"

enum InputIds
{
	Left,
	Right,
	Back,
	Forw
};

void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

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
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeo, *pDefaultMaterial);

	m_pSphere->AttachRigidActor(m_pSphereActor);
	m_pSphere->Translate(0.f, 5.f, 0.f);

	//***Input**/
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction({ Play3D,InputTriggerState::pressed, VK_SPACE });
	pInput->AddInputAction({ IncreaseVolume,InputTriggerState::pressed, 'H' });
	pInput->AddInputAction({ DecreaseVolume,InputTriggerState::pressed, 'J'});
	pInput->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,0U });
	pInput->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,0U });
	pInput->AddInputAction(InputAction{ InputIds::Forw,InputTriggerState::down,VK_UP,-1,0U });
	pInput->AddInputAction(InputAction{ InputIds::Back,InputTriggerState::down,VK_DOWN,-1,0U });

	const auto pFmod = SoundManager::GetInstance()->GetSystem();

	//***Sound 2D**/
	FMOD::Sound* pSound2D{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//***Sound 3D**/
	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->set3DMinMaxDistance(0.f, 100.f);
}

inline FMOD_VECTOR ToFMOD(const XMFLOAT3& v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFMOD(const PxVec3& v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Update()
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

	// Sound Input
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool isPaused;
		m_pChannel2D->getPaused(&isPaused);
		m_pChannel2D->setPaused(!isPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool isPaused;
		m_pChannel3D->getPaused(&isPaused);
		m_pChannel3D->setPaused(!isPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume += 0.1f;
		Clamp(volume, 10.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume -= 0.1f;
		Clamp(volume, 10.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);
	}

	//3D sound attributes
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFMOD(pCam->GetPosition());
	auto forw = ToFMOD(pCam->GetForward());
	auto up = ToFMOD(pCam->GetUp());

	FMOD_VECTOR vel{};
	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_PreviousCamPos.x) / deltaT;
	vel.y = (pos.y - m_PreviousCamPos.y) / deltaT;
	vel.z = (pos.z - m_PreviousCamPos.z) / deltaT;
	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forw, &up);
	auto spherePos = ToFMOD(m_pSphere->GetPosition());
	auto sphereVel = ToFMOD(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);

	m_PreviousCamPos = pos;
}

void AudioTestScene::Draw() const
{
}
