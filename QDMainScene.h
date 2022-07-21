#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include "AbstractScene.h"
#include "InputManager.h"
#include "KeyboardInput.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "DebugText.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "PrimitiveObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Singleton.h"

class QDMainScene : public AbstractScene, Singleton<QDMainScene> {
	friend Singleton<QDMainScene>;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// �J����
	unique_ptr<Camera> camera_;

	// �v���C���[
	unique_ptr<Player> player_;

	// �G
	std::vector<Enemy> enemy_;

	// sound
	unique_ptr<AudioManager> bgm_;

	// �Q�[���I�[�o�[��
	bool is_game_over_ = false;

public:

	QDMainScene();
	~QDMainScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// �G���o��������
	/// </summary>
	/// <param name="pos"></param>
	void SpawnEnemy(XMFLOAT3 pos, Pattern p);
};
