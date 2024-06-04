// 2023 Takeru Yui All Rights Reserved.
#include "Player.h"
#include "DxLib.h"

// 静的定数
// 速度（1=1m、60fps固定として、時速10km）
// 10000m ÷ 時間 ÷ 分 ÷ 秒 ÷ フレーム
const float Player::Speed = static_cast<float>(100000.0 / 60.0 / 60.0 / 60.0);
const float Player::Scale = 0.004f;		// スケール

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: modelHandle(-1)
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/Robot.mv1");
	VECTOR playerFootPos = VGet(0, 1, 0);
	pos = VGet(0, 0, 0);
	velocity = VGet(0, 0, 0);
	dir = VGet(0, 0, 0);
	// 最下段の階段の座標を設定（プレイヤーの足元を中心とする）
	vertex[0][0] = VAdd(playerFootPos, VGet(-1.0f, 0.0f, 0.0f)); // 左
	vertex[0][1] = VAdd(playerFootPos, VGet(1.0f, 0.0f, 0.0f));  // 右

	for (int i = 1; i < 10; ++i) 
	{
		vertex[i][0] = VAdd(vertex[i - 1][1], VGet(2.0f, 2.0f, 0.0f));  // 左
		vertex[i][1] = VAdd(vertex[i - 1][0], VGet(2.0f, 2.0f, 0.0f));  // 右
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	

	 // 現在のキー入力状態を取得
	int currentKeyState = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 単純に方向転換
	dir = VGet(0, 0, 0);

	// 右キーが押された瞬間を検出
	if ((CheckHitKey(KEY_INPUT_SPACE)) && !(previousKeyState & KEY_INPUT_SPACE) && !moveTriggered)
	{
		if (moveChange == true)
		{
			Ritemoveflg = true;
		}
		else
		{
			Leftmoveflg = true;
		}
		
		moveTriggered = true;
	}
	// コントロールキーが押された瞬間を検出
	if ((CheckHitKey(KEY_INPUT_LCONTROL)) && !(previousKeyState & KEY_INPUT_LCONTROL) && !moveTriggered)
	{
		if (moveChange == false)
		{       
			moveChange = true;
		}
		else
		{
			moveChange = false;
		}
		if (moveChange == true)
		{
			Ritemoveflg = true;
		}
		else
		{
			Leftmoveflg = true;
		}
		moveTriggered = true;
	}

	if (!(CheckHitKey(KEY_INPUT_SPACE)) && !(CheckHitKey(KEY_INPUT_LCONTROL)))
	{
		moveTriggered = false; // キーが離されたらフラグをリセット
	}

	if (Ritemoveflg)
	{
		MV1SetRotationXYZ(modelHandle, VGet(0.0f, -90.0f, 0.0f));
		dir = VAdd(dir, VGet(1, 1, 0)); // 右
		Ritemoveflg = false;
	}
	else if (Leftmoveflg)
	{
		MV1SetRotationXYZ(modelHandle, VGet(0.0f, 90.0f, 0.0f));
		dir = VAdd(dir, VGet(-1, 1, 0)); // 左
		Leftmoveflg = false;
	}

	// ゼロ除算避け
	if (VSquareSize(dir) > 0)
	{
		// 正規化
		dir = VNorm(dir);
	}

	// ポジションを更新
	velocity = VScale(dir, Speed);
	pos = VAdd(pos, velocity);

	// 力をかけ終わったベロシティの方向にディレクションを調整
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}

	// 3Dモデルのスケール決定
	MV1SetScale(modelHandle, VGet(Scale, Scale, Scale));

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);

	// 回転
	//MV1SetRotationXYZ(modelHandle, VGet(0.0f, -90.0f, 0.0f));
	// 現在のキー入力状態を次回のために保存
	previousKeyState = currentKeyState;
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}

void Player::DrawTriangleAtPlayerFoot()
{
	
	

	
	//VECTOR vertex3 = VAdd(playerFootPos, VGet(0.0f, 5.0f, 0.0f));  // 上

	// 三角形の描画
	for (int i = 0; i < 10; ++i) {
		DrawLine3D(vertex[i][0], vertex[i][1], GetColor(255, 0, 0));
	}

}
void Player::underLine()
{
	// XYZ軸
		DrawLine3D(VGet(-WorldlineSize, 0, 0), VGet(WorldlineSize, 0, 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(0, -WorldlineSize, 0), VGet(0, WorldlineSize, 0), GetColor(0, 255, 0));
		DrawLine3D(VGet(0, 0, -WorldlineSize), VGet(0, 0, WorldlineSize), GetColor(0, 0, 255));

		//階段
		/*DrawLine3D(pos,pos, GetColor(0, 0, 0));*/

		


}