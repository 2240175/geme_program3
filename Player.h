#pragma once

#include "System/ModelRenderer.h"
#include "character.h"
#include "ProjectileManager.h"
#include "System/AudioSource.h"
#include "Effect.h"
// プレイヤー
class Player : public Character
{
private:
	Player() {};
	~Player() override {};

public:
	//インスタンス取得
	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	//初期化
	void Initialize();

	//終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグ用GUI描画
	void DrawDebugGUI();

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	//// ジャンプ処理
	//void Jump(float speed);

	////速力処理更新
	//void UpdateVelocity(float elapsedTime);

	// ジャンプ入力処理
	void InputJump();

protected:
	//着地したときに呼ばれる
	void OnLanding() override;

private:
	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//// 移動処理
	//void Move(float elapsedTime, float vx, float vz, float speed);

	// 移動入力処理
	void InputMove(float elapsedTime);

	//弾丸入力処理
	void InputProjectile();

	//// 旋回処理
	//void Turn(float elapsedTime, float vx, float vz, float speed);

	//プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();


private:
	Model* model = nullptr;
	float		moveSpeed = 5.0f;
	float		turnSpeed = DirectX::XMConvertToRadians(720);

	float jumpSpeed = 12.0f;
	//float gravity = -30.0f;
	//DirectX::XMFLOAT3 velocity = {0,0,0};
	int jumpCount = 0;
	int jumpLimit = 2;
	ProjectileManager projectileManager;
	AudioSource* hitSE = nullptr;
	Effect* hitEffect = nullptr;
};