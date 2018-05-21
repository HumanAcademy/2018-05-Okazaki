#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

// 画面の種類
enum Mode
{
	Title,
	Game,
};
int MAIN()
{
	const int blockNumber = 20;          // ブロックの数
	const float blockInterval = 250.0;   // ブロックの間隔
	const float blockAmplitude = 75.0;   // ブロックの隙間
	const float speed = 5.0;             // ブロックの進む速さ


	float gravity = 0.0;                 // 重力
	float blockPosition = 0.0;
	float distance;                      // 進んだ距離
	int score;                           // くぐった数
	Mode mode = Title;                   // 画面（Titleは初期の画面）
	Float2 blockOffset[blockNumber];

	Sprite sprite(L"player.png");        // プレイヤーの画像
	sprite.scale = 3.0;          

	Sprite block(L"block.png");          // ブロックの画像
	block.scale = 5.0;

	Text scoreText(L"0", 10.0);
	scoreText.position.y = 200.0;
	scoreText.scale = 10.0;
	scoreText.color = Float4(1.0, 0.0, 0.0, 1.0);

	Text titleText(L"ThroughBird", 80.0);
	titleText.position.x = 90.0;
	titleText.color = Float4(1.0, 0.0, 0.0, 1.0);

	App::SetTitle(L"ThroughBird");

	Camera camera;

	Sound flySound(L"fly.wav");
	Sound hitSound(L"hit.wav");
	Sound pointSound(L"point.wav");

	// 繰り返し
	while (App::Refresh())
	{
		camera.Update();

		switch (mode)
		{
			// タイトル画面の処理
		case Title:

			// エンターキーを押したらゲーム画面へ
			if (App::GetKeyDown(VK_RETURN))
			{
				// パラメータをリセット
				blockPosition = 0.0;
				gravity = 0.0;
				distance = sprite.position.x;
				score = 0;

				// ブロックを並べる処理
				for (int i = 0; i < blockNumber; i++)
				{
					// ブロックを横に等間隔、縦にランダムに並べる
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * 150.0 - 150.0 / 2
					);
					blockPosition = blockOffset[i].x;
				}
				mode = Game;
			}

			// プレイヤーのリセット
			sprite.position = Float3(-200.0, 0.0, 0.0);
			sprite.angles.z = 0.0;

			sprite.Draw();

			titleText.Draw();

			scoreText.Draw();

			break;

			// ゲーム画面の処理
		case Game:
			for (int i = 0; i < blockNumber; i++)
			{
				// ブロックが左にいって見えなくなったら、右に移動する
				if (blockOffset[i].x < -App::GetWindowSize().x)
				{
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * 250.0 - 250.0 / 2
					);
					blockPosition = blockOffset[i].x;
				}
				// 当たり判定
				float playerSize = sprite.GetSize().x * sprite.scale.x * 0.8;
				float blockWidth = (block.GetSize().x * block.scale.x + playerSize) / 2.0;
				float blockHeight = (blockAmplitude - playerSize) / 2.0;

				if (blockOffset[i].x - blockWidth < sprite.position.x &&
					blockOffset[i].x + blockWidth > sprite.position.x)
				{
					if (blockOffset[i].y - blockHeight > sprite.position.y ||
						blockOffset[i].y + blockHeight < sprite.position.y)
					{
						// 当たった時の判定
						hitSound.Play();
						mode = Title;

					}
				}
				blockOffset[i].x -= speed * 1.0;

				block.position = blockOffset[i];
				block.position.y +=
					(block.GetSize().y * block.scale.y + blockAmplitude) / 2.0;
				block.Draw();

				block.position = blockOffset[i];
				block.position.y -=
					(block.GetSize().y * block.scale.y + blockAmplitude) / 2.0;
				block.Draw();
			}
			blockPosition -= speed;
			
			float playerSpeed = 9.0;

			if (App::GetKey(VK_SPACE))
			{
				playerSpeed /= 2.0;
			}

			if (App::GetKey(VK_UP))
			{
				// プレイヤーを上に動かす
				sprite.position.y += playerSpeed;
			}

			if (App::GetKey(VK_DOWN))
			{
				// プレイヤーを下に動かす
				sprite.position.y -= playerSpeed;
			}

			// プレイヤーの描画
			sprite.Draw();

			// 距離を加算
			distance += speed;

			if (distance > blockInterval)
			{
				distance -= blockInterval;
				score++;
				pointSound.Play();
			}

			scoreText.Create(to_wstring(score), 10.0);
			scoreText.Draw();

			break;
		}
	}

	return 0;
}