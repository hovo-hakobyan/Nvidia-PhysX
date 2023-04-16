#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"


//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

//#define W01
//#define W02
#define AssignmentScenes

#ifdef W01
#include "TestScene.h"
#include "PhysXTestScene.h"
#include "BouncingBallScene.h"
#include "FrictionTestScene.h"
#include "BoxForceScene.h"
#include "BoxTorqueScene.h"
#include "W1_AssignmentScene.h"
#endif

#ifdef W02
#include "PhysXMeshTestScene.h"
#include "TriggerTestScene.h"
#include "AudioTestScene.h"
#include "W2_Assignment.h"
#endif

#ifdef AssignmentScenes
#include "W1_AssignmentScene.h"
#include "W2_Assignment.h"
#endif
namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		pSceneManager->AddGameScene(new W1_AssignmentScene());
		pSceneManager->AddGameScene(new BoxTorqueScene());
		pSceneManager->AddGameScene(new BoxForceScene());
		pSceneManager->AddGameScene(new FrictionTestScene());
		pSceneManager->AddGameScene(new TestScene());
		pSceneManager->AddGameScene(new PhysXTestScene());
		pSceneManager->AddGameScene(new BouncingBallScene());
		
#endif

#ifdef W02
		pSceneManager->AddGameScene(new W2_Assignment());
		pSceneManager->AddGameScene(new AudioTestScene());
		pSceneManager->AddGameScene(new TriggerTestScene());
		pSceneManager->AddGameScene(new PhysXMeshTestScene());
		
#endif
#ifdef AssignmentScenes
		pSceneManager->AddGameScene(new W1_AssignmentScene());
		pSceneManager->AddGameScene(new W2_Assignment());
#endif
	}
}

