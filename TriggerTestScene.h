#pragma once
#include "GameScene.h"

class TriggerTestScene final : public GameScene
{
public:
	TriggerTestScene() :GameScene(L"TriggerTestScene") {}
	~TriggerTestScene() override = default;
	TriggerTestScene(const TriggerTestScene& other) = delete;
	TriggerTestScene(TriggerTestScene&& other) noexcept = delete;
	TriggerTestScene& operator=(const TriggerTestScene& other) = delete;
	TriggerTestScene& operator=(TriggerTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:
	GameObject* m_pSphere;
	PxRigidDynamic* m_pSphereActor;

	PxRigidStatic* m_pTriggerActorLeft;
	PxRigidStatic* m_pTriggerActorRight;

	bool m_isRightTrigger{ false };
};