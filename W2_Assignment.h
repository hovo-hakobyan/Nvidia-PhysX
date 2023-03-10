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

};