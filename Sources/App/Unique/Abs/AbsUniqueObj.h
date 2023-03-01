#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../../../Lib/Object3d/Object3d.h"
#include "../../../Lib/Model/Model.h"
#include "../../../App/Collision/CollisionPrimitive.h"
#include "../../DebugUtill/NcmImGui.h"

/// <summary>
/// 移動 || 生死判定のあるゲーム内オブジェクトのインターフェース
/// </summary>
class AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected:

	// 回転行列
	XMMATRIX mat_rot_;

	// 描画データ
	std::shared_ptr<Object3d> obj_;
	std::shared_ptr<Object3d> coll_obj_;

	// 当たり判定
	Sphere coll_;

	// 正面ベクトル
	XMFLOAT3 forward_vec_;

	// その他情報
	float speed_;
	bool is_dead_;

public:

	AbsUniqueObj(float speed, float coll_radius);
	virtual ~AbsUniqueObj() = default;

public:

	inline const XMMATRIX &GetRotMat() { return mat_rot_; }
	inline const XMFLOAT3 &GetPos() { return obj_->GetPos(); }
	inline const XMFLOAT3 &GetRot() { return obj_->GetRot(); }
	inline const Sphere &GetCollData() { return coll_; }
	inline const XMFLOAT3 &GetFwdVec() { return forward_vec_; }
	inline const float &GetSpeed() { return speed_; }
	inline const bool &IsDead() { return is_dead_; }
	inline const void Death() { is_dead_ = true; }

	inline void SetPos(const XMFLOAT3 &pos) { obj_->SetPos(pos); }
	inline void SetRot(const XMFLOAT3 &rot) { obj_->SetRot(rot); }
	inline void SetSpeed(const float speed) { speed_ = speed; }
	inline void SetIsDead(bool b) { is_dead_ = b; }

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawColl() = 0;
	virtual void DebugDraw() = 0;

	/// <summary>
	/// 描画データの初期化
	/// </summary>
	/// <param name="obj_model">使用するモデルデータ</param>
	/// <param name="coll_model">使用する当たり判定可視化用モデルデータ</param>
	void InitObj3d(Model *obj_model, Model *coll_model);

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void UpdateColl();

	/// <summary>
	/// 正面ベクトルの計算
	/// </summary>
	void CalcFwdVec();
};
