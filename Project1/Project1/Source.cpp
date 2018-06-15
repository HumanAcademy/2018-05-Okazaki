#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

// ��ʂ̎��
enum Mode
{
	Title,
	Game,
};
int MAIN()
{
	const int blockNumber = 20;          // �u���b�N�̐�
	const float blockInterval = 250.0;   // �u���b�N�̊Ԋu
	const float blockAmplitude = 75.0;   // �u���b�N�̌���
	const float speed = 5.0;             // �u���b�N�̐i�ޑ���


	float gravity = 0.0;                 // �d��
	float blockPosition = 0.0;
	float distance;                      // �i�񂾋���
	int score;                           // ����������
	Mode mode = Title;                   // ��ʁiTitle�͏����̉�ʁj
	Float2 blockOffset[blockNumber];

	Sprite sprite(L"player.png");        // �v���C���[�̉摜
	sprite.scale = 3.0;          

	Sprite block(L"block.png");          // �u���b�N�̉摜
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

	// �J��Ԃ�
	while (App::Refresh())
	{
		camera.Update();

		switch (mode)
		{
			// �^�C�g����ʂ̏���
		case Title:

			// �G���^�[�L�[����������Q�[����ʂ�
			if (App::GetKeyDown(VK_RETURN))
			{
				// �p�����[�^�����Z�b�g
				blockPosition = 0.0;
				gravity = 0.0;
				distance = sprite.position.x;
				score = 0;

				// �u���b�N����ׂ鏈��
				for (int i = 0; i < blockNumber; i++)
				{
					// �u���b�N�����ɓ��Ԋu�A�c�Ƀ����_���ɕ��ׂ�
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * 150.0 - 150.0 / 2
					);
					blockPosition = blockOffset[i].x;
				}
				mode = Game;
			}

			// �v���C���[�̃��Z�b�g
			sprite.position = Float3(-200.0, 0.0, 0.0);
			sprite.angles.z = 0.0;

			sprite.Draw();

			titleText.Draw();

			scoreText.Draw();

			break;

			// �Q�[����ʂ̏���
		case Game:
			for (int i = 0; i < blockNumber; i++)
			{
				// �u���b�N�����ɂ����Č����Ȃ��Ȃ�����A�E�Ɉړ�����
				if (blockOffset[i].x < -App::GetWindowSize().x)
				{
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * 250.0 - 250.0 / 2
					);
					blockPosition = blockOffset[i].x;
				}
				// �����蔻��
				float playerSize = sprite.GetSize().x * sprite.scale.x * 0.8;
				float blockWidth = (block.GetSize().x * block.scale.x + playerSize) / 2.0;
				float blockHeight = (blockAmplitude - playerSize) / 2.0;

				if (blockOffset[i].x - blockWidth < sprite.position.x &&
					blockOffset[i].x + blockWidth > sprite.position.x)
				{
					if (blockOffset[i].y - blockHeight > sprite.position.y ||
						blockOffset[i].y + blockHeight < sprite.position.y)
					{
						// �����������̔���
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
				// �v���C���[����ɓ�����
				sprite.position.y += playerSpeed;
			}

			if (App::GetKey(VK_DOWN))
			{
				// �v���C���[�����ɓ�����
				sprite.position.y -= playerSpeed;
			}

			// �v���C���[�̕`��
			sprite.Draw();

			// ���������Z
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