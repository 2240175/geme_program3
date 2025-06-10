#include "ProjectileManager.h"

//コンストラクタ
ProjectileManager::ProjectileManager()
{

}
//デストラクタ
ProjectileManager::~ProjectileManager()
{
	Clear();
}

//更新処理
void ProjectileManager::Update(float elapsedTime)
{
	//更新処理
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elapsedTime);
	}

	//// 敵同士の衝突処理
	//CollisionEnemyVsEnemies();

	//破棄処理
	for (Projectile* projectile : removes)
	{
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}
		//弾丸の破棄処理
		delete projectile;
	}
	//破棄リストをクリア
	removes.clear();


}

//描画処理
void ProjectileManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Render(rc, renderer);
	}

}

//デバッグプリミティブ描画
void ProjectileManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->RenderDebugPrimitive(rc, renderer);
	}

}

//弾丸登録
void ProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

//弾丸全削除
void ProjectileManager::Clear()
{
	for (Projectile* projectile : projectiles)
	{
		delete  projectile;
	}
	projectiles.clear();

}

//弾丸削除
void ProjectileManager::Remove(Projectile* projectile)
{
	//破棄リストに追加
	removes.insert(projectile);
}

