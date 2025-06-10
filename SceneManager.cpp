#include "SceneManager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//古いシーンを終了処理
		Clear();
		//新しいシーンを設定
		currentScene = nextScene;
		nextScene = nullptr;
		//シーン初期化処理
		currentScene->Initialize();
	}

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//描画処理
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

//GUI描画
void SceneManager::DrawGUI()
{
	if (currentScene != nullptr)
	{
		currentScene->DrawGUI();
	}
}

//シーンクリア	
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	//新しいシーン設定
	nextScene = scene;
}

