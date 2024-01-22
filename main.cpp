#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include <math.h>
#include <cmath>
#include "compute.h"

const char kWindowTitle[] = "LE2C_07_サイトウ_ユウキ_MT4_01_01";

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle){
	Matrix4x4 result = {};

	result.m[0][0] = std::powf(axis.x, 2) * (1.0f - std::cos(angle)) + std::cos(angle);
	result.m[0][1] = axis.x * axis.y * (1.0f - std::cos(angle)) + axis.z * std::sin(angle);
	result.m[0][2] = axis.x * axis.z * (1.0f - std::cos(angle)) - axis.y * std::sin(angle);

	result.m[1][0] = axis.x * axis.y * (1.0f - std::cos(angle)) - axis.z * std::sin(angle);
	result.m[1][1] = std::powf(axis.y, 2) * (1.0f - std::cos(angle)) + std::cos(angle);
	result.m[1][2] = axis.y * axis.z * (1.0f - std::cos(angle)) + axis.x * std::sin(angle);

	result.m[2][0] = axis.x * axis.z * (1.0f - std::cos(angle)) + axis.y * std::sin(angle);
	result.m[2][1] = axis.y * axis.z * (1.0f - std::cos(angle)) - axis.x * std::sin(angle);
	result.m[2][2] = std::powf(axis.z, 2) * (1.0f - std::cos(angle)) + std::cos(angle);

	result.m[3][3] = 1;

	return result;
}


static const int kRowHeight = 20;
static const int kColumnWidth = 60;
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


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
	float angle = 0.44f;

	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

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

		MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");

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
