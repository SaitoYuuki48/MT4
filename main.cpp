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

	Quaternion rotation = MakeRotateAxisAngleQuaternion(
		Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix = MakeQRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

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
		
		QuaternionScreenPrintf(0, 0, rotation, " : rotatetion");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, " : rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, " : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, " : rotateByMatrix");

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
