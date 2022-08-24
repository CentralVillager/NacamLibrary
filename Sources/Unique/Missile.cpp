#include "Missile.h"
#include "../Utility/Utility.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::sphere_model_ = nullptr;

Missile::Missile()
{
	object_ = make_unique<Object3d>();
	emitter_ = std::make_unique<Emitter>();
	sphere_obj_ = std::make_shared<Object3d>();
}

Missile::~Missile()
{}

void Missile::LoadResources()
{
	if (!model_)
	{
		model_ = make_unique<Model>();
		model_->LoadObjModel("Resources/bullet/", "bullet.obj", "bullet.mtl");
	}

	if (!sphere_model_)
	{
		sphere_model_ = std::make_unique<Model>();
		sphere_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Missile::Initialize(const MissileArgs &args, LockOnSystem *sys)
{
	lockon_sys_ = sys;

	// 値を入力より設定
	mi_args_.pos = XMFLOAT3(args.pos);
	mi_args_.vel = XMFLOAT3(args.vel);
	mi_args_.acc = XMFLOAT3(args.acc);
	mi_args_.tgt_pos = args.tgt_pos;
	mi_args_.detection_range = args.detection_range;
	mi_args_.init_straight_time_ = args.init_straight_time_;
	mi_args_.life = args.life;
	mi_args_.is_alive = args.is_alive;
	tgt_index_ = sys->GetTgtIndex();

	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(XMFLOAT3(mi_args_.pos));
	object_->SetRotation(XMFLOAT3(0, 180.0f, 0));

	sphere_obj_->Initialize();
	sphere_obj_->SetModel(sphere_model_.get());

	Emitter::EmitterArgs emi;
	XMFLOAT3 temp = mi_args_.vel;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.particle.position_ = mi_args_.pos;
	emi.particle.velocity_ = temp;
	emi.particle.accel_ = { 0, 0, 0 };
	emi.particle.life_ = mi_args_.life;
	emi.particle.s_scale_ = 1.0f;
	emi.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	emi.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	emi.gene_num_ = 1;
	emi.use_life_ = true;
	emi.life_ = mi_args_.life;
	emitter_->SetEmitterArgs(emi);
}

void Missile::Finalize()
{}

void Missile::Update()
{
	// 自然消滅
	if (mi_args_.life <= 0 && emitter_->NoticeCanTerminate())
	{
		mi_args_.is_alive = false;
		return;
	}

	mi_args_.life--;

	if (!is_hit_)
	{
		object_->Update();
		UpdateCollision();
	}

	emitter_->SetPosition(object_->GetPosition());
	emitter_->GenerateParticle();
}

void Missile::Draw()
{
	if (!is_hit_) { object_->Draw(); }
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (!is_hit_) { sphere_obj_->Draw(); }
}

void Missile::DebugDraw()
{}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = object_->GetPosition();
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;
	object_->SetPosition(pos);
}

void Missile::HomingTarget(const XMFLOAT3 &target_pos)
{
	XMFLOAT3 pos = object_->GetPosition();

	static XMVECTOR old_len{};
	const float speed = 3.0f;	// 仮

	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;
		pos.z += speed;
		object_->SetPosition(pos);

		return;
	}

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&object_->GetPosition());
	XMVECTOR ta_vec = XMLoadFloat3(&mi_args_.tgt_pos);
	//XMVECTOR ta_vec = XMLoadFloat3(&target_pos);

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec = {
		(ta_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(ta_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(ta_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	XMVECTOR len = XMVector3Length(vec);
	min_dist_ = len.m128_f32[0];

	// ミサイルと目標が離れ始めたら
	//if (len.m128_f32[0] >= old_len.m128_f32[0]) {

	//	// 保持している速度を加算
	//	pos.x += mi_args_.vel.x;
	//	pos.y += mi_args_.vel.y;
	//	pos.z += mi_args_.vel.z;

	//	// 位置を反映
	//	object_->SetPosition(pos);

	//	// その後の追尾処理をスキップ
	//	return;
	//}

	// 追尾範囲外なら
	if (len.m128_f32[0] >= mi_args_.detection_range)
	{
		// 直進だけして
		pos.z += speed;
		object_->SetPosition(pos);

		// その後の追尾処理をスキップ
		return;
	}

	//if (pos.z >= target_pos.z)
	if (pos.z >= mi_args_.tgt_pos.z)
	{
		// 位置を更新
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// 位置を反映
		object_->SetPosition(pos);

		return;
	}

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_args_.vel));

	rot_dead_zone_ = 1.0f;

	XMVECTOR differ = {

		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	// 速度を加算
	/*norm_vec.m128_f32[0] *= speed;
	norm_vec.m128_f32[1] *= speed;
	norm_vec.m128_f32[2] *= speed;*/

	differ.m128_f32[0] /= rot_dead_zone_;
	differ.m128_f32[1] /= rot_dead_zone_;
	differ.m128_f32[2] /= rot_dead_zone_;

	/*mi_args_.vel.x += norm_vec.m128_f32[0];
	mi_args_.vel.y += norm_vec.m128_f32[1];
	mi_args_.vel.z += norm_vec.m128_f32[2];*/
	mi_args_.vel.x += differ.m128_f32[0] * speed;
	mi_args_.vel.y += differ.m128_f32[1] * speed;
	mi_args_.vel.z = speed;

	// 位置を更新
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// 位置を反映
	object_->SetPosition(pos);

	/* -- 外積追尾 -- */
	// XMVECTORに変換
	//XMVECTOR mi_vec = XMLoadFloat3(&object_->GetPosition());
	//XMVECTOR ta_vec = XMLoadFloat3(&target_pos);

	//// ふたつの座標を結ぶベクトルを計算
	//XMVECTOR vec = {
	//	(ta_vec.m128_f32[0] - mi_vec.m128_f32[0]),
	//	(ta_vec.m128_f32[1] - mi_vec.m128_f32[1]),
	//	(ta_vec.m128_f32[2] - mi_vec.m128_f32[2])
	//};

	//// 長さを計算
	//XMVECTOR len = XMVector3Length(vec);

	//// 正規化
	//XMVECTOR norm_vec = XMVector3Normalize(vec);

	//float r = vec.m128_f32[0] * mi_vec.m128_f32[2] - mi_vec.m128_f32[0] * vec.m128_f32[2];

	//if (IsPlus(r)) {

	//	norm_vec.m128_f32[0] += 1.0f;

	//} else if (IsMinus(r)) {

	//	norm_vec.m128_f32[0] += -1.0f;
	//}

	//pos.x += norm_vec.m128_f32[0];
	//pos.y += norm_vec.m128_f32[1];
	//pos.z += norm_vec.m128_f32[2];

	//// 位置を反映
	//object_->SetPosition(pos);
}

void Missile::HomingTarget(EnemiesList &enemies)
{
	XMFLOAT3 pos = object_->GetPosition();

	const float speed = 3.0f;	// 仮

	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;
		pos.z += speed;
		object_->SetPosition(pos);

		return;
	}

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&object_->GetPosition());
	XMVECTOR ta_vec = XMLoadFloat3(&enemies.GetPos(tgt_index_));

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec = {
		(ta_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(ta_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(ta_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	XMVECTOR len = XMVector3Length(vec);
	min_dist_ = len.m128_f32[0];

	// 追尾範囲外なら
	if (len.m128_f32[0] >= mi_args_.detection_range)
	{
		// 直進だけして
		pos.z += speed;
		object_->SetPosition(pos);

		// その後の追尾処理をスキップ
		return;
	}

	if (pos.z >= enemies.GetPos(tgt_index_).z)
	{
		// 位置を更新
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// 位置を反映
		object_->SetPosition(pos);

		return;
	}

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_args_.vel));

	rot_dead_zone_ = 1.0f;

	XMVECTOR differ = {

		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	// 速度を加算
	differ.m128_f32[0] /= rot_dead_zone_;
	differ.m128_f32[1] /= rot_dead_zone_;
	differ.m128_f32[2] /= rot_dead_zone_;

	mi_args_.vel.x += differ.m128_f32[0] * speed;
	mi_args_.vel.y += differ.m128_f32[1] * speed;
	mi_args_.vel.z = speed;

	// 位置を更新
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// 位置を反映
	object_->SetPosition(pos);
}

void Missile::TermEmitter()
{
	emitter_->PrepareTerminate();
	is_hit_ = true;	// 仮
}

void Missile::UpdateCollision()
{
	coll_.center = XMLoadFloat3(&object_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	sphere_obj_->SetPosition(ToFloat3(coll_.center));
	sphere_obj_->SetScale(coll_.radius);
	sphere_obj_->Update();
}
