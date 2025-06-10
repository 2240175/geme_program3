#include"Collision.h"

//球と球の交差判定

bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{

	//A->Bの単位ベクトルを算出
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

//距離判定
	float range = radiusA + radiusB;
	if(lengthSq > range)
	{
		return false;
	}

//AがBを押し出す

	Vec = DirectX::XMVector3Normalize(Vec);//押し出す方向の単位ベクトル
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);


	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;

}

//円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	//Aの足元がBの頭より上なら当たってない
	if (positionA.y + positionB.y + heightB)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当たってない
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}
	//XZ平面での範囲をチェック
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)
	{ 
		return false;
	}
	//単位ベクトル化
	vx /= distXZ;
	vz /= distXZ;

	//球が円柱を押し出す
	outPositionB.x = positionA.x + (vx * range);
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + (vz * range);

	return true;


}

//球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3 & spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3 & cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3 & outCylinderPosition)
{

	//	Aの足元がBの頭より上なら当たってない
	if(spherePosition.y - sphereRadius> cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当たっていない
	if (spherePosition.y  + cylinderRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ平面出の範囲チェック
	float vx = cylinderPosition.x - spherePosition.x;//X成分を引き算
	float vz = cylinderPosition.z - spherePosition.z;//ｚ成分を引き算

	//半径の合計を引き算
	float range = sphereRadius + cylinderRadius;

	//ＡからＢへのベクトルを求める（長さを求める）
	float distXZ = sqrtf(vx * vx + vz * vz);

	//長さが半径より大きかったら当たってない
	if (distXZ > range)
	{
		return false;
	}
	//単位ベクトル化
	vx /= distXZ;
	vz /= distXZ;

	//球が円柱を押し出す
	outCylinderPosition.x = spherePosition.x+(vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z +(vz * range);

	return true;
}
