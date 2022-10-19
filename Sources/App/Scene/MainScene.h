#pragma once
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>
#include "../../App/Scene/AbsScene.h"
#include "../../Lib/DirectXBase/DirectXBase.h"
#include "../../Lib/Camera/Camera.h"
#include "../../Lib/Sprite/NcmSprite.h"
#include "../Grid/GridRender.h"
#include "../Unique/Player/Player.h"
#include "../Unique/Enemy/Enemy.h"
#include "../Unique/Missile/MissileManager.h"
#include "../Unique/Enemy/EnemiesList.h"
#include "../Unique/Reticle/Reticle.h"
#include "../Unique/LockOnSystem/LockOnSystem.h"
#include "../Unique/WaveManager/WaveManager.h"
#include "../Number/Numbers.h"

class MainScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
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

	static constexpr UINT GRID_NUM_ = 3;

private:

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<EnemiesList> ene_list_;
	std::unique_ptr<GridRender> grid_;
	std::array<GridRender, GRID_NUM_> grid_floor_;
	std::unique_ptr<MissileManager> missile_mgr_;
	std::unique_ptr<Emitter> dust_;
	std::unique_ptr<Object3d> sky_dome_;
	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Reticle> reticle_;
	std::unique_ptr<LockOnSystem> lockon_sys_;
	std::unique_ptr<Numbers> numbers_;
	int texture_;
	int clear_;
	int space_;

	bool use_keybind_ = true;
	int key_bind_ = 0;
	bool is_wire_ = true;
	bool draw_dust_ = true;
	bool draw_coll_ = false;
	bool draw_numbers_ = false;

	bool is_result_ = false;

	float ImGui_detection_range_ = 1000.0f;

	static constexpr float SPEED_ = 2.0f;
	int player_camera_speed_;

public:

	MainScene();
	~MainScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

private:

	void CollisionProcess();
	bool CheckDoDisplay();
};
