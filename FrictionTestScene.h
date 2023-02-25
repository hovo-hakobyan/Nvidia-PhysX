#pragma once
#include "GameScene.h"
class FrictionTestScene final : public GameScene
{
public:
	FrictionTestScene() :GameScene(L"FrictionTestScene") {}
	~FrictionTestScene() override = default;
	FrictionTestScene(const FrictionTestScene& other) = delete;
	FrictionTestScene(FrictionTestScene&& other) noexcept = delete;
	FrictionTestScene& operator=(const FrictionTestScene& other) = delete;
	FrictionTestScene& operator=(FrictionTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	PxRigidStatic* m_pGroundPlane{ nullptr };
	std::vector<GameObject*> m_Spheres{ 3 };
};