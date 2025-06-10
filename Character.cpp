#include "Character.h"
//行列更新処理
void Character::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}
void Character::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	////衝突判定用のデバッグ球を描画
 //   renderer->RenderSphere(rc, position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//衝突判定用のデバッグ円柱を描画
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
//ジャンプ処理
void Character::Jump(float speed)
{
	velocity.y = speed;
}
//衝撃を与える
void Character::AddImupulse(const DirectX::XMFLOAT3& impulse)
{
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}
//速力更新処理
void Character::UpdateVelocity(float elapsedTime)
{
	UpdateVerticalVelocity(elapsedTime);
	UpdateVerticalMove(elapsedTime);
	UpdateHorizontalVelocity(elapsedTime);
	UpdatehorizontalMove(elapsedTime);
}
void Character::UpdateVerticalVelocity(float elapsedTime)
{
	//重力処理
	velocity.y += gravity * elapsedTime;
}
void Character::UpdateVerticalMove(float elapsedTime)
{
	//移動処理
	position.y += velocity.y * elapsedTime;
	//地面判定
	if (position.y < 0.0f)
	{
		position.y = 0.0f;

		//着地した
		if (!isGround)
		{
			OnLanding();
		}
		isGround = true;
		velocity.y = 0.0f;
	}
	else
	{
		isGround = false;
	}
}
//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedTime)
{
	//XZ面の速力を減速
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	//XZ面の速力を加速する
	if (length <= maxMoveSpeed) {
		//移動ベクトルがゼロでないなら加速
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f) {
			//加速力
			float acceleration = this->acceleration * elapsedTime;
			if (!isGround) acceleration *= airControl;
			//移動ベクトルによる加速処理
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;
			//最大速度制限
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed) {
				float vx = velocity.x / length;
				float vz = velocity.z / length;
				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}
		}
	}
	//移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
	if (length > 0.0f) {
		//摩擦力
		float friction = this->friction * elapsedTime;
		//空中にいるときは摩擦を減らす
		if (!isGround) friction *= airControl;
		//摩擦による横方向の減速処理
		if (length > friction) {
			//単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;
			//減速処理
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		//横方向の速力が摩擦力以下になったので速力を無効化
		else {
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
}
void Character::UpdatehorizontalMove(float elapsedTime)
{
	//移動処理
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;
}
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;
	// 進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;
	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);
	//--- ガタつきに対応させる ---
		// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);	//内積：フロントが基準
	// 内積値は-1.0～1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;	//補正値
	//rot = 1.0f - dot;	//ImGuiで表示するためにメンバー変数とした
	if (rot > speed) rot = speed;	//回転速度よりも、rotが大きい場合は、回転速度を使う
	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);
	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		//angle.y -= speed;
		angle.y -= rot;
	}
	else
	{
		//angle.y += speed;
		angle.y += rot;
	}
}
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	//ダメージが0の場合は健康状態を変更する必要なし
	if (damage == 0) return false;
	//死亡している場合は健康状態を変更ない
	if (health <= 0) return false;
	//無敵時間中はダメージを与えない
	if (invincibleTimer > 0.0f)return false;
	//無敵時間設定
	invincibleTimer = invincibleTime;
	//ダメージ処理
	health -= damage;
	//死亡通知
	if (health <= 0) {
		OnDead();
	}
	//ダメージ通知
	else {
		OnDamaged();
	}
	//健康状態を変更した場合はtrue
	return true;
}
// 移動処理
void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	//speed *= elapsedTime;
	//position.x += vx * speed;
	//position.z += vz * speed;
	//移動方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;
	//最大速度設定
	maxMoveSpeed = speed;
}
//無敵時間更新
void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}