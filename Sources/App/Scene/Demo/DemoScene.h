#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/3D/FbxObject3d.h"
#include "../../../Lib/3D/FbxModel.h"
#include "../../../Lib/3D/Point.h"
#include "../../../Lib/3D/PlatePoly.h"
#include "../../../Lib/3D/NcmPlatePoly.h"
#include "../../Grid/GridRender.h"
#include "../../../Lib/Math/Easing/NcmEasing.h"
#include "../../Player/Player.h"
#include "../../../Lib/Loader/JsonLoader.h"
#include "../../Level/LevelDataManager.h"

class DemoScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	ncm_thandle texture_;
	ncm_ehandle ease_;
	ncm_thandle test_;

	bool b = true;

	std::unique_ptr<GridRender> grid_;
	std::unique_ptr<GridRender> ugrid_;

	std::unique_ptr<Point> point_;
	std::unique_ptr<PlatePoly> poly_;

	std::unique_ptr<Player> player_;

	// レベルデータ
	std::unique_ptr<LevelDataManager> level_;

	// オブジェクトs
	std::vector<Object3d> objs_;
	std::unique_ptr<Model> model_;

	std::unique_ptr<NcmPlatePoly> particle_;

	std::unique_ptr<Object3d> tex_obj_;
	std::unique_ptr<Model> tex_model_;

	ncm_thandle dds_;

public:

	DemoScene();
	~DemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
