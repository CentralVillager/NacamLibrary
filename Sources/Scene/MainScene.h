#pragma once
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>
#include "../Scene/AbstractScene.h"
#include "../DirectXBase/DirectXBase.h"
#include "../Camera/Camera.h"
#include "../Sprite/Sprite.h"
#include "../Grid/GridRender.h"
#include "../Unique/Player.h"
#include "../Unique/Enemy.h"
#include "../Unique/MissileManager.h"
#include "../Unique/EnemiesList.h"
#include "../Unique/Reticle.h"
#include "../Unique/LockOnSystem.h"
#include "../Unique/WaveManager.h"

class MainScene : public AbstractScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

public:

	enum class KeyBind
	{
		Start,

		Camera,
		Player,
		AddEnemy,

		MaxNum
	};

private:

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<EnemiesList> ene_list_;
	std::unique_ptr<Sprite> texture_;
	std::unique_ptr<Sprite> clear_;
	std::unique_ptr<Sprite> space_;
	std::unique_ptr<GridRender> grid_;
	std::unique_ptr<MissileManager> missile_mgr_;
	std::unique_ptr<Emitter> dust_;
	std::unique_ptr<Object3d> sky_dome_;
	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Reticle> reticle_;
	std::unique_ptr<LockOnSystem> lockon_sys_;

	int key_bind_ = 0;
	bool is_wire_ = false;
	bool draw_dust_ = false;
	bool draw_coll_ = false;

	bool is_result_ = false;

	float ImGui_detection_range_ = 1000.0f;

public:

	MainScene();
	~MainScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
