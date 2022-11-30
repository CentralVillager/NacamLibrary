#include "Missile.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/Lib/NacamLib/NacamLib.h"
#include "../../../Lib/NacamError/NacamError.h"
#include "../Sources/App/Math/NcmMath.h"
#include "../../../Lib/PreDraw/PreDraw.h"

using namespace DirectX;
using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::coll_model_ = nullptr;

Missile::Missile()
	: AbsUniqueObj(3.0f, 1.0f),
	emitter_(std::make_unique<Emitter>()),
	mi_args_()
{}

Missile::~Missile()
{}

void Missile::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_unique<Model>();
		model_->LoadObjModel("Resources/bullet/", "bullet.obj", "bullet.mtl");
	}

	if (!coll_model_)
	{
		coll_model_ = std::make_unique<Model>();
		coll_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Missile::Initialize(const MissileArgs &args)
{
	// 値を入力より設定
	mi_args_ = args;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(XMFLOAT3(mi_args_.pos));

	EmitterDesc emi{};
	emi.particle.position_ = mi_args_.pos;
	// エミッター用にミサイルの速度を反転して提供
	XMFLOAT3 temp = mi_args_.vel;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.particle.velocity_ = temp;
	// エミッター用にミサイルの加速度を反転して提供
	temp = mi_args_.acc;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.particle.accel_ = temp;
	emi.particle.life_ = mi_args_.life;
	emi.particle.s_scale_ = 1.0f;
	emi.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	emi.vel_rand_ = { 0.01f, 0.01f, 0.01f };
	emi.gene_num_ = 1;
	emi.use_life_ = true;
	emi.life_ = mi_args_.life;
	emitter_->SetEmitterDesc(emi);
}

void Missile::Initialize()
{}

void Missile::Finalize()
{}

void Missile::Update()
{
	// 寿命が尽きたら
	if (IsZeroOrLess(mi_args_.life))
	{
		// ミサイルを無効にする
		mi_args_.is_validity = false;

		// エミッターの終了準備が出来たら
		if (emitter_->NoticeCanTerminate())
		{
			// 殺す
			is_dead_ = true;
			return;
		}
	}

	// まだ寿命があるなら
	if (!IsZero(mi_args_.life))
	{
		// 寿命を減らす
		mi_args_.life--;
	}

	// ミサイルが有効なら
	if (mi_args_.is_validity)
	{
		// 更新処理をする
		obj_->Update();
		UpdateColl();
	}

	UpdateEmitter();
	CalcFwdVec();
}

void Missile::Draw()
{
	// ミサイルが有効ならミサイルを描画
	PreDraw::PreRender(PipelineName::Object3d);
	if (mi_args_.is_validity) { obj_->Draw(); }

	// パーティクルを描画
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (mi_args_.is_validity) { coll_obj_->Draw(); }
}

void Missile::DebugDraw()
{
	ImGui::Checkbox("validity", &mi_args_.is_validity);
	ImGui::Text("vel : (%f, %f, %f)", mi_args_.vel.x, mi_args_.vel.y, mi_args_.vel.z);
	ImGui::Text("acc : (%f, %f, %f)", mi_args_.acc.x, mi_args_.acc.y, mi_args_.acc.z);
}

void Missile::Activate()
{
	is_dead_ = false;
	mi_args_.is_validity = true;
}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.z += mi_args_.vel.z;
	obj_->SetPos(pos);
}

void Missile::HomingTarget(EnemiesList &enemies)
{
	// 位置を取得
	XMFLOAT3 pos = obj_->GetPos();

	// 追尾したい敵の添字を検索
	int index = enemies.GetEnemyIndexWithID(mi_args_.tgt_id);

	// 居なかったら
	if (index == (int)(NacamError::NonDetected))
	{
		// 以前の速度をそのまま加算
		pos.x *= mi_args_.vel.x;
		pos.y *= mi_args_.vel.y;
		pos.z *= mi_args_.vel.z;

		// 位置を反映
		obj_->SetPos(pos);

		// その後の処理をスキップ
		return;
	}

	// 判明した添字をもとに敵の位置を特定
	XMVECTOR tgt_vec = XMLoadFloat3(&enemies.GetPos(index));

	// ミサイルのベクトルをXMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPos());

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR dest_vec = tgt_vec - mi_vec;

	// 正規化
	dest_vec = XMVector3Normalize(dest_vec);

	// 速度を乗算
	dest_vec.m128_f32[0] *= speed_;
	dest_vec.m128_f32[1] *= speed_;
	dest_vec.m128_f32[2] *= speed_;

	// その値を格納
	XMStoreFloat3(&mi_args_.vel, dest_vec);

	// 速度を加算
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// 位置を更新 
	obj_->SetPos(pos);

	// 回転角を更新
	obj_->SetRot(LookAt(mi_args_.vel));
}

void Missile::TestHomingTarget(EnemiesList &enemies)
{
	// 位置を取得
	XMFLOAT3 pos = obj_->GetPos();

	// 追尾したい敵の添字を検索
	int index = enemies.GetEnemyIndexWithID(mi_args_.tgt_id);

	// 居なかったら
	if (index == (int)(NacamError::NonDetected))
	{
		// 以前の速度をそのまま加算
		pos.x *= mi_args_.vel.x;
		pos.y *= mi_args_.vel.y;
		pos.z *= mi_args_.vel.z;

		// 位置を反映
		obj_->SetPos(pos);

		// その後の処理をスキップ
		return;
	}

	// 判明した添字をもとに敵の位置を特定
	XMVECTOR tgt_vec = XMLoadFloat3(&enemies.GetPos(index));

	// ミサイルのベクトルをXMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPos());

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR dest_vec = tgt_vec - mi_vec;

	// 正規化
	dest_vec = XMVector3Normalize(dest_vec);

	// 速度を乗算
	dest_vec.m128_f32[0] *= speed_;
	dest_vec.m128_f32[1] *= speed_;
	dest_vec.m128_f32[2] *= speed_;

	// その値を格納
	XMStoreFloat3(&mi_args_.vel, dest_vec);

	// 速度を加算
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// 位置を更新 
	obj_->SetPos(pos);

	// 回転角を更新
	obj_->SetRot(LookAt(mi_args_.vel));
}

void Missile::PrepareTermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPos());
	emitter_->UseParticle();
	emitter_->Update();
	emitter_->GenerateParticle();
}
