#include <Novice.h>
#include <cstdint>
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "MyMath.h"


#include<imgui.h>
const char kWindowTitle[] = "LC1B_03_オオサキ_ハルキ_タイトル";



Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

struct AABB {
	Vector3 min;
	Vector3 max;
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHandleWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHandleWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHandleWidth + (xIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ x,0.0f,-kGridHandleWidth };
		Vector3 end{ x,0.0f,kGridHandleWidth };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (x == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHandleWidth + (zIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ -kGridHandleWidth,0.0f,z };
		Vector3 end{ kGridHandleWidth,0.0f,z };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (z == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);

	}
}

void DrawaAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 Bottom[4] = {
		{ aabb.min.x,aabb.min.y,aabb.min.z },
		{ aabb.max.x,aabb.min.y,aabb.min.z },
		{ aabb.min.x,aabb.min.y,aabb.max.z },
		{ aabb.max.x,aabb.min.y,aabb.max.z }
	};

	Vector3 Top[4] = {
		{ aabb.min.x,aabb.max.y,aabb.min.z },
		{ aabb.max.x,aabb.max.y,aabb.min.z },
		{ aabb.min.x,aabb.max.y,aabb.max.z },
		{ aabb.max.x,aabb.max.y,aabb.max.z }
	};

	Vector3 BottomScreen[4] = {};
	Vector3 TopScreen[4] = {};


	for (uint32_t i = 0; i < 4; i++) {
		BottomScreen[i] = Transform(Transform(Bottom[i], viewProjectionMatrix), viewportMatrix);
		TopScreen[i] = Transform(Transform(Top[i], viewProjectionMatrix), viewportMatrix);
	}


	Novice::DrawLine((int)BottomScreen[0].x, (int)BottomScreen[0].y, (int)BottomScreen[1].x, (int)BottomScreen[1].y, color);
	Novice::DrawLine((int)BottomScreen[0].x, (int)BottomScreen[0].y, (int)BottomScreen[2].x, (int)BottomScreen[2].y, color);
	Novice::DrawLine((int)BottomScreen[1].x, (int)BottomScreen[1].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);
	Novice::DrawLine((int)BottomScreen[2].x, (int)BottomScreen[2].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);

	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)TopScreen[1].x, (int)TopScreen[1].y, color);
	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)TopScreen[2].x, (int)TopScreen[2].y, color);
	Novice::DrawLine((int)TopScreen[1].x, (int)TopScreen[1].y, (int)TopScreen[3].x, (int)TopScreen[3].y, color);
	Novice::DrawLine((int)TopScreen[2].x, (int)TopScreen[2].y, (int)TopScreen[3].x, (int)TopScreen[3].y, color);

	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)BottomScreen[0].x, (int)BottomScreen[0].y, color);
	Novice::DrawLine((int)TopScreen[1].x, (int)TopScreen[1].y, (int)BottomScreen[1].x, (int)BottomScreen[1].y, color);
	Novice::DrawLine((int)TopScreen[2].x, (int)TopScreen[2].y, (int)BottomScreen[2].x, (int)BottomScreen[2].y, color);
	Novice::DrawLine((int)TopScreen[3].x, (int)TopScreen[3].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	AABB Box1 = {
		.min{-0.5f,-0.5f,-0.5f },
		.max{0.0f,0.0f,0.0f},
	};

	AABB Box2 = {
		.min{0.2f,0.2f,0.2f },
		.max{1.0f,1.0f,1.0f},
	};

	uint32_t color1 = WHITE;
	uint32_t color2 = WHITE;

	MyMath* myMath_ = new MyMath();
	
	Vector3 cameraPosition = { 0.0f ,0.0f,-20.0f };
	Vector3 cameraTranslate = { 0.0f,-1.0f,-6.49f };
	Vector3 cameraRotate = { -0.26f,0.26f,0.0f };

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

		Box1.min.x = (std::min)(Box1.min.x, Box1.max.x);
		Box1.max.x = (std::max)(Box1.min.x, Box1.max.x);
		Box1.min.y = (std::min)(Box1.min.y, Box1.max.y);
		Box1.max.y = (std::max)(Box1.min.y, Box1.max.y);
		Box1.min.z = (std::min)(Box1.min.z, Box1.max.z);
		Box1.max.z = (std::max)(Box1.min.z, Box1.max.z);


		Matrix4x4 worldMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 cameraMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = myMath_->Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = myMath_->MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
		Matrix4x4 WorldViewProjectionMatrix = myMath_->Multiply(worldMatrix, myMath_->Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = myMath_->MakeViewportMatrix(0, 0, float(1280.0f), float(720.0f), 0.0f, 1.0f);

		DrawGrid(WorldViewProjectionMatrix, viewportMatrix);

		DrawaAABB(Box1, WorldViewProjectionMatrix, viewportMatrix,color1);
		DrawaAABB(Box2, WorldViewProjectionMatrix, viewportMatrix,color2);
		
		
		if (IsCollision(Box1, Box2)) {
			color2 = RED;
		}
		else {
			color2 = WHITE;
		}


		ImGui::Begin("window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("box1 min", &Box1.min.x, 0.01f);
		ImGui::DragFloat3("box1 max", &Box1.max.x, 0.01f);

		ImGui::DragFloat3("box2 min", &Box2.min.x, 0.01f);
		ImGui::DragFloat3("box2 max", &Box2.max.x, 0.01f);


		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
