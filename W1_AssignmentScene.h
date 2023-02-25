
#pragma once
#include "GameScene.h"

class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pSphere;
	PxRigidDynamic* m_pSphereActor;

	const int m_NrCubesWidth{ 10 };
	const int m_NrCubesHeight{ 10 };

	const XMFLOAT3 m_BrickDimensions{ 2.f,2.f,2.f };
	const XMFLOAT3 m_WallStartPos{ 0.f,0.f,0.f };
	const float m_HeightToDropWall{ .5f };
	const float m_DistanceBetweenWallCol{ 1.2f };

	std::vector<GameObject*> m_Bricks;

	void ResetScene();
};