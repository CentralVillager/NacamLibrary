#include "Missile.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/Lib/NacamLib/NacamLib.h"
#include "../../../Lib/NacamError/NacamError.h"
#include "../Sources/App/Math/NcmMath.h"
#include "../../../Lib/PreDraw/PreDraw.h"
#include "../Player/Player.h"
#include "../../Utility/NcmColor.h"

using namespace DirectX;
using namespace NcmUtill;
using namespace NcmMath;
using namespace NcmColor;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::coll_model_ = nullptr;

Missile::Missile()
	: AbsUniqueObj(3.0f, 1.0f),
	homing_sequence_(std::make_unique<MissileHoming>()),
	emitter_(std::make_unique<Emitter>()),
	explo_emi_(std::make_unique<Emitter>()),
	mi_param_(),
	for_lerp_handle_x_(),
	for_lerp_handle_y_(),
	for_lerp_handle_z_(),
	before_particle_pos_(),
	current_particle_pos_()
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
	speed_ = mi_param_.speed;

	// 描画データを初期化
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
	emi.part_desc_.s_scale_ = 1.5f;
	emi.part_desc_.color_ = mi_param_.trail_color;
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
		InvalidateMissile();

		// エミッターの終了準備が出来たら
		if (emitter_->NoticeCanTerminate() &&
			explo_emi_->NoticeCanTerminate())
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
	//emitter_->Draw();
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
	if (!mi_param_.is_validity)
	{
		return;
	}

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
	explo_emi.part_desc_.velocity_.x = norm_vec.m128_f32[0] * 0.1f;
	explo_emi.part_desc_.velocity_.y = norm_vec.m128_f32[1] * 0.1f;
	explo_emi.part_desc_.velocity_.z = norm_vec.m128_f32[2] * 0.1f;
	explo_emi.part_desc_.accel_ = XMFLOAT3(0, 0, 0);
	explo_emi.part_desc_.life_ = 50;
	explo_emi.part_desc_.s_scale_ = 2.0f;
	explo_emi.part_desc_.e_scale_ = 0;
	explo_emi.part_desc_.color_ = NcmColor::EXPLOSION_ORENGE;
	explo_emi.part_desc_.alpha_ = 1.0f;
	explo_emi.pos_rand_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	explo_emi.vel_rand_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
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

void Missile::UpdateTargetPos(EnemiesList &enemies)
{
	// 追尾したいターゲットのIDが入っている要素の添字を検索
	int32_t index = enemies.GetEnemyIndexWithID(mi_param_.tgt_id);

	// 居なかったら
	if (index == (int32_t)(NacamError::NonDetected))
	{
		InvalidateMissile();
		return;
	}
	// ターゲットがプレイヤーなら
	else if (index == (int32_t)(TargetIs::Player))
	{
		mi_param_.tgt_pos = p_player_->GetPos();
		return;
	}

	// 判明した添字をもとにターゲットの位置を特定
	mi_param_.tgt_pos = enemies.GetPos(index);
}

void Missile::HomingTarget(HomingAccuracy accuracy)
{
	homing_sequence_->HomingTarget(*this, accuracy);
}

void Missile::PrepareTermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPos());
	//emitter_->GenerateParticle();
	emitter_->UpdateParticle();

	before_particle_pos_ = current_particle_pos_;
	current_particle_pos_ = obj_->GetPos();

	if (IsAllZero(before_particle_pos_)) { return; }

	if (mi_param_.is_validity)
	{
		InterpolateParticle();
	}
}

void Missile::InterpolateParticle()
{
	// パーティクル間の距離
	XMFLOAT3 particle_differ =
	{
		current_particle_pos_.x - before_particle_pos_.x,
		current_particle_pos_.y - before_particle_pos_.y,
		current_particle_pos_.z - before_particle_pos_.z,
	};

	// イージング設定構造体
	NcmEaseDesc ease_desc_x{};
	NcmEaseDesc ease_desc_y{};
	NcmEaseDesc ease_desc_z{};
	// 初期値は前パーティクル
	ease_desc_x.init_value = before_particle_pos_.x;
	ease_desc_y.init_value = before_particle_pos_.y;
	ease_desc_z.init_value = before_particle_pos_.z;
	// 総移動量はパーティクル間の距離
	ease_desc_x.total_move = particle_differ.x;
	ease_desc_y.total_move = particle_differ.y;
	ease_desc_z.total_move = particle_differ.z;
	// その線形補間
	ease_desc_x.ease_type = NcmEaseType::Lerp;
	ease_desc_y.ease_type = NcmEaseType::Lerp;
	ease_desc_z.ease_type = NcmEaseType::Lerp;
	// 遷移率
	float rate = 0.05f;
	ease_desc_x.t_rate = rate;
	ease_desc_y.t_rate = rate;
	ease_desc_z.t_rate = rate;
	// 登録
	for_lerp_handle_x_ = NcmEasing::RegisterEaseData(ease_desc_x);
	for_lerp_handle_y_ = NcmEasing::RegisterEaseData(ease_desc_y);
	for_lerp_handle_z_ = NcmEasing::RegisterEaseData(ease_desc_z);

	while (!NcmEasing::IsFinished(for_lerp_handle_x_))
	{
		// 更新
		NcmEasing::UpdateValue(for_lerp_handle_x_);
		NcmEasing::UpdateValue(for_lerp_handle_y_);
		NcmEasing::UpdateValue(for_lerp_handle_z_);

		// 補間された位置
		XMFLOAT3 interpolated_pos =
		{
			NcmEasing::GetValue(for_lerp_handle_x_),
			NcmEasing::GetValue(for_lerp_handle_y_),
			NcmEasing::GetValue(for_lerp_handle_z_),
		};

		// パーティクル設定
		ParticleDesc part_desc{};
		// 粒を補間位置に置く
		part_desc.position_ = interpolated_pos;
		XMFLOAT3 temp = mi_param_.vel;
		temp.x -= temp.x;
		temp.y -= temp.y;
		temp.z -= temp.z;
		part_desc.velocity_ = temp;
		temp = mi_param_.acc;
		temp.x -= temp.x;
		temp.y -= temp.y;
		temp.z -= temp.z;
		part_desc.accel_ = temp;
		part_desc.life_ = mi_param_.life;
		part_desc.s_scale_ = GenerateRandom(1.0f, 2.0f);
		part_desc.e_scale_ = GenerateRandom(2.5f, 3.0f);
		part_desc.color_ = mi_param_.trail_color;

		emitter_->Add(part_desc);
	}
}

const XMFLOAT3 operator*(const DirectX::XMFLOAT3 &lhs, const float &rhs)
{
	return XMFLOAT3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
