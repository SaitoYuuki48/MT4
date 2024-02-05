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

	Quaternion result;
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

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
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

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

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
		
		QuaternionScreenPrintf(10, 0, identity, " : Identity");
		QuaternionScreenPrintf(10, kRowHeight, conj, " : Conjugate");
		QuaternionScreenPrintf(10, kRowHeight * 2, inv, " : Inverse");
		QuaternionScreenPrintf(10, kRowHeight * 3, normal, " : rotatetion");
		QuaternionScreenPrintf(10, kRowHeight * 4, mul1, " : Multiply(q1, q2)");
		QuaternionScreenPrintf(10, kRowHeight * 5, mul2, " : Multiply(q2, q1)");
		Novice::ScreenPrintf(10, kRowHeight * 6, "%.2f : Norm", norm);

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
