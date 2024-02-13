#include <Novice.h>
#include "compute.h"

const char kWindowTitle[] = "LE2C_07_サイトウ_ユウキ_タイトル";

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

Quaternion Multiply(const Quaternion& lns, const Quaternion& rhs)
{
	Quaternion result;

	result.w = lns.w * rhs.w - lns.x * rhs.x - lns.y * rhs.y - lns.z * rhs.z;

	result.x = lns.y * rhs.z - lns.z * rhs.y + rhs.w * lns.x + lns.w * rhs.x;

	result.y = lns.z * rhs.x - lns.x * rhs.z + rhs.w * lns.y + lns.w * rhs.y;

	result.z = lns.x * rhs.y - lns.y * rhs.x + rhs.w * lns.z + lns.w * rhs.z;

	return result;
}

Quaternion IdentityQuaternion()
{
	Quaternion result;

	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;

	return result;
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	Quaternion result = {
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};

	return result;
}

float Norm(const Quaternion& quaternion)
{
	return sqrt(quaternion.w * quaternion.w
		+ quaternion.x * quaternion.x
		+ quaternion.y * quaternion.y
		+ quaternion.z * quaternion.z);
}

Quaternion Normalize(const Quaternion& quaternion)
{
	Quaternion result;

	float norm = Norm(quaternion);

	if (quaternion.x != 0.0f)
	{
		result.x = quaternion.x / norm;
	}
	else
	{
		result.x = 0.0f;
	}

	if (quaternion.y != 0.0f)
	{
		result.y = quaternion.y / norm;
	}
	else
	{
		result.y = 0.0f;
	}

	if (quaternion.z != 0.0f)
	{
		result.z = quaternion.z / norm;
	}
	else
	{
		result.z = 0.0f;
	}

	if (quaternion.w != 0.0f)
	{
		result.w = quaternion.w / norm;
	}
	else
	{
		result.w = 0.0f;
	}

	return result;
}


Quaternion Inverse(const Quaternion& quaternion)
{

	Quaternion result = {};
	Quaternion conjugate = Conjugate(quaternion);

	float norm = Norm(quaternion);

	if (norm != 0.0f) {
		result.x = conjugate.x / (norm * norm);
		result.y = conjugate.y / (norm * norm);
		result.z = conjugate.z / (norm * norm);
		result.w = conjugate.w / (norm * norm);
	}

	return result;
}

//任意軸回転を表すQuaternion
Quaternion MakeRotateAxisAngleQuaternion(
	const Vector3 & axis, float angl)
{
	Quaternion result;
	float sin = std::sinf(angl / 2.0f);

	result.x = sin * axis.x;
	result.y = sin * axis.y;
	result.z = sin * axis.z;
	result.w = std::cosf(angl / 2.0f);

	return result;
}

//ベクトルでquaternionを回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion vectorQuaternion = { vector.x, vector.y, vector.z, 0.0f };

	Quaternion conjugate = Conjugate(quaternion);

	Quaternion result = Multiply(Multiply(quaternion, vectorQuaternion), conjugate);

	Vector3 rotatedVector = { result.x, result.y, result.z };

	return rotatedVector;

}

//Quaternionから回転行列を求める
Matrix4x4 MakeQRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;

	result.m[0][0] = (quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[0][1] = ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z)) * 2;
	result.m[0][2] = ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y)) * 2;
	result.m[0][3] = 0.0f;

	result.m[1][0] = ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z)) * 2;
	result.m[1][1] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) +
		(quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[1][2] = ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x)) * 2;
	result.m[1][3] = 0.0f;

	result.m[2][0] = ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y)) * 2;
	result.m[2][1] = ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x)) * 2;
	result.m[2][2] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) -
		(quaternion.y * quaternion.y) + (quaternion.z * quaternion.z);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

void invertQuaternion(Quaternion* q) {
	q->w = -q->w;
	q->x = -q->x;
	q->y = -q->y;
	q->z = -q->z;
}

//球面線形保管

Quaternion Slerp(Quaternion& q0, Quaternion& q1, float t)
{
	Quaternion result{};

	float dot = (q0.w * q1.w) + (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z);

	// クォータニオンが逆向きの場合、符号を反転
	if (dot < 0.0) {

		invertQuaternion(&q1);

		dot = -dot;

	}

	// 線形補間
	float theta = std::acosf(dot);
	float sinTheta = std::sinf(theta);
	float weight1 = std::sinf((1.0f - t) * theta) / sinTheta;
	float weight2 = std::sinf(t * theta) / sinTheta;

	result.w = (weight1 * q0.w) + (weight2 * q1.w);
	result.x = (weight1 * q0.x) + (weight2 * q1.x);
	result.y = (weight1 * q0.y) + (weight2 * q1.y);
	result.z = (weight1 * q0.z) + (weight2 * q1.z);

	// 補間結果の正規化
	Normalize(result);

	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name)
{
	Novice::ScreenPrintf(x, y, "%s", name);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + 20, "%6.3f", matrix.m[row][column]);
		}
	}
}

void QuaternionScreenPrintf(int x, int y, Quaternion& Q, const char* label)
{
	Novice::ScreenPrintf(x, y, "%0.2f", Q.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", Q.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", Q.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%0.2f", Q.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, "%s", label);

}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);

	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.0f,0.71f }, 3.141592f);

	Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		QuaternionScreenPrintf(0, 0, interpolate0, " : interpolate0, Slerp(q0, q1, 0.0f)");
		QuaternionScreenPrintf(0, kRowHeight, interpolate1, " : interpolate1, Slerp(q0, q1, 0.3f)");
		QuaternionScreenPrintf(0, kRowHeight * 2, interpolate2, " : interpolate2, Slerp(q0, q1, 0.5f)");
		QuaternionScreenPrintf(0, kRowHeight * 3, interpolate3, " : interpolate3, Slerp(q0, q1, 0.7f)");
		QuaternionScreenPrintf(0, kRowHeight * 4, interpolate4, " : interpolate4, Slerp(q0, q1, 1.0f)");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
