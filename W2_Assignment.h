#pragma once
#include "GameScene.h"

class W2_Assignment final : public GameScene
{
public:
	W2_Assignment() :GameScene(L"W2_Assignment") {}
	~W2_Assignment() override = default;
	W2_Assignment(const W2_Assignment& other) = delete;
	W2_Assignment(W2_Assignment&& other) noexcept = delete;
	W2_Assignment& operator=(const W2_Assignment& other) = delete;
	W2_Assignment& operator=(W2_Assignment&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:
	GameObject* m_pLevel{ nullptr };
	GameObject* m_pRedBox{ nullptr };
	GameObject* m_pBlueBox{ nullptr };
	GameObject* m_pTriggerBoxRed{ nullptr };
	GameObject* m_pTriggerBoxBlue{ nullptr };

	GameObject* m_pSphere;
	PxRigidDynamic* m_pSphereActor;

	GameObject* m_pLeftSphere;
	GameObject* m_pRightSphere;

	PxRigidStatic* m_pTriggerActorRed;
	PxRigidStatic* m_pTriggerActorBlue;

	bool m_isBlueTrigger{ false };
	bool m_isRedTrigger{ false };

	bool m_shouldOpenRed{ false };
	bool m_shouldOpenBlue{ false };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pBellSound{ nullptr };

	GameObject* m_pBlueHatch{ nullptr };
	GameObject* m_pRedHatch{ nullptr };

	PxRevoluteJoint* m_pBlueJoint{ nullptr };
	PxRevoluteJoint* m_pRedJoint{ nullptr };

	const float m_LowerHatchLimit { -PxPi / 2.f };

	XMFLOAT3 m_RedBoxStartPos{ 4.f, 7.f, 0.f };
	XMFLOAT3 m_BlueBoxStartPos{ -4.f, 7.f, 0.f };
	XMFLOAT3 m_SphereStartPos{ 0.f, 5.f, 0.f };
	XMFLOAT3 m_LeftSphereStartPos{ -4.0f, 24.f, 0.f };
	XMFLOAT3 m_RightSphereStartPos{ 4.0f, 24.0f, 0.f };

	void ResetScene();
};