#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include <math.h>
#include <cmath>
#include "compute.h"


const char kWindowTitle[] = "LE2C_07_サイトウ_ユウキ_MT4_01_02";

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

	return result;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	Vector3 cross = Cross(from, to);
	float cos = Dot(from, to);
	float sin = Length(cross);

	float epsilon = 1e-6f;
	Vector3 axis;
	if (std::abs(cos + 1.0f) <= epsilon) {
		if (std::abs(from.x) > epsilon || std::abs(from.y) > epsilon) {
			axis.x = from.y;
			axis.y = -from.x;
			axis.z = 0.0f;
		}
		else if (std::abs(from.x) > epsilon || std::abs(from.z) > epsilon) {
			axis.x = from.z;
			axis.y = 0.0f;
			axis.z = -from.x;
		}
		else {
			//Zero vector
			assert(false);
		}
		axis = Normalize(axis);
	}
	else {
		axis = Normalize(cross);
	}

	Matrix4x4 result;
	result.m[0][0] = axis.x * axis.x * (1 - cos) + cos;
	result.m[0][1] = axis.x * axis.y * (1 - cos) + axis.z * sin;
	result.m[0][2] = axis.x * axis.z * (1 - cos) - axis.y * sin;

	result.m[1][0] = axis.x * axis.y * (1 - cos) - axis.z * sin;
	result.m[1][1] = axis.y * axis.y * (1 - cos) + cos;
	result.m[1][2] = axis.y * axis.z * (1 - cos) + axis.x * sin;

	result.m[2][0] = axis.x * axis.z * (1 - cos) + axis.y * sin;
	result.m[2][1] = axis.y * axis.z * (1 - cos) - axis.x * sin;
	result.m[2][2] = axis.z * axis.z * (1 - cos) + cos;

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

	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	Vector3 to0 = {};
	to0 = Subtract(to0, from0);
	Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
	Matrix4x4 rotateMatrix0 = DirectionToDirection(
		Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

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

		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

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
