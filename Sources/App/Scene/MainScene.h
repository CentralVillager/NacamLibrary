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
#include "../Ui/NcmUi.h"
#include "../../App/Unique/Ultimate/UltimateManager.h"

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
	static constexpr float SPEED_ = 2.0f;
	static constexpr XMFLOAT3 init_pos_ = XMFLOAT3(0, 0, -200.0f);
	static constexpr XMFLOAT3 cam_init_pos_ = XMFLOAT3(0, 10.0f, init_pos_.z);

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
	std::unique_ptr<NcmUi> ui_;
	std::unique_ptr<UltimateManager> ult_;
	int texture_;
	int clear_;
	int over_;
	int space_;

	bool do_debug_;
	int key_bind_;
	bool is_wire_;
	bool draw_dust_;
	bool draw_coll_;
	bool draw_numbers_;

	bool is_clear_;
	bool is_failed_;

	float ImGui_detection_range_;
	XMFLOAT2 ImGui_Ui_pos_;

	int player_camera_speed_;

	static constexpr float normal_fov_ = 60.0f;
	static constexpr float accel_fov_ = 70.0f;
	uint32_t fov_acc_value_;
	uint32_t fov_dec_value_;

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
