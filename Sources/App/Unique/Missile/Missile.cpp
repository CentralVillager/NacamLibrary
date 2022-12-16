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
	explo_emi_(std::make_unique<Emitter>()),
	mi_param_()
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

void Missile::Initialize(const MissileParam &args)
{
	// 値を入力より設定
	mi_param_ = args;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(XMFLOAT3(mi_param_.pos));

	/* ミサイル軌跡のエミッター */
	EmitterDesc emi{};
	emi.part_desc_.position_ = mi_param_.pos;
	// エミッター用にミサイルの速度を反転して提供
	XMFLOAT3 temp = mi_param_.vel;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.part_desc_.velocity_ = temp;
	// エミッター用にミサイルの加速度を反転して提供
	temp = mi_param_.acc;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.part_desc_.accel_ = temp;
	emi.part_desc_.life_ = mi_param_.life;
	emi.part_desc_.s_scale_ = 1.0f;
	emi.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	emi.vel_rand_ = { 0.01f, 0.01f, 0.01f };
	emi.gene_num_ = 1;
	emi.use_life_ = true;
	emi.life_ = mi_param_.life;
	emitter_->SetEmitterDesc(emi);
}

void Missile::Initialize()
{}

void Missile::Finalize()
{}

void Missile::Update()
{
	// 寿命が尽きたら
	if (IsZeroOrLess(mi_param_.life))
	{
		// ミサイルを無効にする
		//InvalidateMissile();

		// エミッターの終了準備が出来たら
		if (emitter_->NoticeCanTerminate() &&
			explo_emi_->NoticeCanTerminate())
			//if (emitter_->NoticeCanTerminate())
		{
			// 殺す
			is_dead_ = true;
			return;
		}
	}

	// まだ寿命があるなら
	if (!IsZero(mi_param_.life))
	{
		// 寿命を減らす
		mi_param_.life--;
	}

	// ミサイルが有効なら
	if (mi_param_.is_validity)
	{
		// 更新処理をする
		obj_->Update();
		UpdateColl();
	}

	// 爆発中なら
	if (mi_param_.is_explode)
	{
		explo_emi_->UpdateParticle();
	}

	UpdateEmitter();
	CalcFwdVec();
}

void Missile::Draw()
{
	// ミサイルが有効ならミサイルを描画
	PreDraw::SetPipeline(PipelineName::Object3d);
	if (mi_param_.is_validity) { obj_->Draw(); }

	// パーティクルを描画
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (mi_param_.is_validity) { coll_obj_->Draw(); }
}

void Missile::DebugDraw()
{
	ImGui::Text("emitter_pos : (%f, %f, %f)", explo_emi_->GetPosition().x, explo_emi_->GetPosition().y, explo_emi_->GetPosition().z);
	ImGui::Checkbox("validity", &mi_param_.is_validity);
	ImGui::Text("pos : (%f, %f, %f)", mi_param_.pos.x, mi_param_.pos.y, mi_param_.pos.z);
	ImGui::Text("vel : (%f, %f, %f)", mi_param_.vel.x, mi_param_.vel.y, mi_param_.vel.z);
	ImGui::Text("acc : (%f, %f, %f)", mi_param_.acc.x, mi_param_.acc.y, mi_param_.acc.z);
	ImGui::Text("alpha : %f", emitter_->GetEmitterDesc().part_desc_.alpha_);
}

void Missile::Activate()
{
	// 死亡フラグを降ろす
	is_dead_ = false;

	// ミサイルを有効化
	mi_param_.is_validity = true;
}

void Missile::InvalidateMissile()
{
	// ミサイルを無効化
	mi_param_.is_validity = false;

	// 爆発させる
	ExplosionOnDeath();
}

void Missile::ExplosionOnDeath()
{
	// 爆発フラグを建てる
	mi_param_.is_explode = true;

	/* 爆発のエミッター */
	EmitterDesc explo_emi{};
	explo_emi.part_desc_.position_ = mi_param_.pos;
	XMVECTOR norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_param_.vel));
	explo_emi.part_desc_.velocity_.x = -norm_vec.m128_f32[0] * 0.1f;
	explo_emi.part_desc_.velocity_.y = -norm_vec.m128_f32[1] * 0.1f;
	explo_emi.part_desc_.velocity_.z = -norm_vec.m128_f32[2] * 0.1f;
	explo_emi.part_desc_.accel_ = XMFLOAT3(0, 0, 0);
	explo_emi.part_desc_.life_ = 50;
	explo_emi.part_desc_.s_scale_ = 1.0f;
	explo_emi.part_desc_.e_scale_ = 0;
	explo_emi.part_desc_.alpha_ = 1.0f;
	explo_emi.pos_rand_ = XMFLOAT3(0, 0, 0);
	explo_emi.vel_rand_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	explo_emi.gene_num_ = EXPLO_PARTICLE_NUM_;
	explo_emi.use_life_ = false;
	explo_emi_->SetEmitterDesc(explo_emi);

	// パーティクルを生成
	explo_emi_->GenerateParticle();
}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.z += mi_param_.vel.z;
	obj_->SetPos(pos);
}

void Missile::HomingTarget(EnemiesList &enemies)
{
	mi_param_.pos = obj_->GetPos();

	// 敵がいないなら
	if (enemies.GetEnemies().size() <= 0)
	{
		// 以前の速度をそのまま加算
		mi_param_.pos.x += mi_param_.vel.x;
		mi_param_.pos.y += mi_param_.vel.y;
		mi_param_.pos.z += mi_param_.vel.z;

		// 位置を反映
		obj_->SetPos(mi_param_.pos);

		return;
	}

	// 無追尾時間中なら
	if (mi_param_.init_straight_time > 0)
	{
		mi_param_.init_straight_time--;

		// 直進させる
		//MoveZ(speed);

		return;
	}

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPos());

	// 追尾したい敵のIDが入っている要素の添字を検索
	int index = enemies.GetEnemyIndexWithID(mi_param_.tgt_id);

	// 居なかったら
	if (index == (int)(NacamError::NonDetected))
	{
		// 以前の速度をそのまま加算
		mi_param_.pos.x += mi_param_.vel.x;
		mi_param_.pos.y += mi_param_.vel.y;
		mi_param_.pos.z += mi_param_.vel.z;

		// 位置を反映
		obj_->SetPos(mi_param_.pos);

		// その後の処理をスキップ
		return;
	}

	// 判明した添字をもとに敵の位置を特定
	XMVECTOR tgt_vec = XMLoadFloat3(&enemies.GetPos(index));

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec =
	{
		(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	XMVECTOR len = XMVector3Length(vec);

	// 追尾範囲外なら
	if (len.m128_f32[0] >= mi_param_.detection_range)
	{
		// 直進だけして
		MoveZ(speed_);

		// その後の追尾処理をスキップ
		return;
	}

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_param_.vel));

	// 離れている距離
	XMVECTOR differ =
	{
		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	mi_param_.vel.x += mi_param_.acc.x;
	mi_param_.vel.y += mi_param_.acc.y;
	mi_param_.vel.z += mi_param_.acc.z;

	// 速度を加算
	mi_param_.vel.x += differ.m128_f32[0] * speed_;
	mi_param_.vel.y += differ.m128_f32[1] * speed_;
	mi_param_.vel.z += differ.m128_f32[2] * speed_;

	// 位置を更新
	mi_param_.pos.x += mi_param_.vel.x;
	mi_param_.pos.y += mi_param_.vel.y;
	mi_param_.pos.z += mi_param_.vel.z;

	// 位置を反映
	obj_->SetPos(mi_param_.pos);

	obj_->SetRot(LookAt(XMFLOAT3(mi_norm_vec.m128_f32[0], mi_norm_vec.m128_f32[1], mi_norm_vec.m128_f32[2])));
	//obj_->SetRot(LookAt(mi_param_.vel));
}

void Missile::TestHomingTarget(EnemiesList &enemies)
{
	// 位置を取得
	XMFLOAT3 pos = obj_->GetPos();

	// 追尾したい敵の添字を検索
	int index = enemies.GetEnemyIndexWithID(mi_param_.tgt_id);

	// 居なかったら
	if (index == (int)(NacamError::NonDetected))
	{
		// 以前の速度をそのまま加算
		pos.x *= mi_param_.vel.x;
		pos.y *= mi_param_.vel.y;
		pos.z *= mi_param_.vel.z;

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
	XMStoreFloat3(&mi_param_.vel, dest_vec);

	// 速度を加算
	pos.x += mi_param_.vel.x;
	pos.y += mi_param_.vel.y;
	pos.z += mi_param_.vel.z;

	// 位置を更新 
	obj_->SetPos(pos);

	// 回転角を更新
	obj_->SetRot(LookAt(mi_param_.vel));
}

void Missile::PrepareTermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPos());
	//emitter_->Update();
	emitter_->GenerateParticle();
	emitter_->UpdateParticle();
}
