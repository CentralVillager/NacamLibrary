#pragma once
#pragma once
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>
#include "../Grid/GridFloorManager.h"
#include "../Grid/NcmGridFloor.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Missile/MissileManager.h"
#include "../Enemy/EnemiesList.h"
#include "../Reticle/Reticle.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../WaveManager/WaveManager.h"
#include "../Number/Numbers.h"
#include "../Ui/NcmUi.h"
#include "../../App/Scene/AbsScene.h"
#include "../../App/Ultimate/UltimateManager.h"
#include "../../App/Particle/NcmParticleManager.h"
#include "../../Lib/DirectX/DirectXBase.h"
#include "../../Lib/Camera/Camera.h"
#include "../../Lib/2D/NcmSprite.h"
#include "../../Lib/3D/PlatePoly.h"
#include "../../Lib/3D/NcmPlatePoly.h"
#include "../../Lib/Input/NcmInput.h"

class MainScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

public:

	/// <summary>
	/// デバッグ用のキーバインド
	/// </summary>
	enum class KeyBind
	{
		Start,

		Camera,
		Player,
		AddEnemy,

		MaxNum
	};

private:

	static constexpr float SPEED_ = 1.5f;
	static constexpr XMFLOAT3 INIT_POS_ = XMFLOAT3(0, 0, -100.0f);
	static constexpr XMFLOAT3 CAM_INIT_POS_ = XMFLOAT3(0, 10.0f, INIT_POS_.z);

	static constexpr float NORMAL_FOV_ = 60.0f;
	static constexpr float ACCEL_FOV_ = 70.0f;

	static constexpr float SCENE_CHANGE_SPEED_ = 0.05f;

private:

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<EnemiesList> ene_list_;
	std::unique_ptr<NcmGridFloor> grid_floor_;
	std::unique_ptr<MissileManager> missile_mgr_;
	std::unique_ptr<Emitter> dust_;
	std::unique_ptr<Object3d> sky_dome_;
	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Reticle> reticle_;
	std::unique_ptr<LockOnSystem> lockon_sys_;
	std::unique_ptr<Numbers> numbers_;
	std::unique_ptr<NcmUi> ui_;
	std::unique_ptr<UltimateManager> ult_;
	std::unique_ptr<NcmParticleManager> particle_mgr_;

	// テクスチャ
	ncm_thandle texture_;
	ncm_thandle clear_;
	ncm_thandle over_;
	ncm_thandle space_;

	// クリアフラグ
	bool is_clear_;
	bool is_failed_;

	// デバッグ用
	int key_bind_;
	bool is_wire_;
	bool draw_dust_;
	bool draw_coll_;
	bool draw_numbers_;
	float ImGui_detection_range_;
	XMFLOAT2 ImGui_Ui_pos_;

	// イージング用ハンドル
	ncm_ehandle player_speed_;
	ncm_ehandle player_dec_speed_;

	ncm_ehandle fov_acc_value_;
	ncm_ehandle fov_dec_value_;

	bool is_scene_change_;

	float result_alpha_;

public:

	MainScene();
	~MainScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;

private:

	void CollisionProcess();
	bool CheckDoDisplay();
	void AlphaTransition(float trans_speed);
	void HitSequence(int i, int j);
};
