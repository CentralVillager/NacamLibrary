#include "Missile.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/Lib/NacamLib/NacamLib.h"
#include "../../../Lib/NacamError/NacamError.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::coll_model_ = nullptr;

Missile::Missile()
	: AbsUniqueObj(1.0f, 1.0f),
	emitter_(std::make_unique<Emitter>()),
	mi_args_(),
	min_dist_(),
	rot_dead_zone_(),
	tgt_index_(),
	lockon_sys_(nullptr)
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

void Missile::Initialize(const MissileArgs &args, LockOnSystem *sys)
{
	lockon_sys_ = sys;

	// 値を入力より設定
	mi_args_ = args;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPosition(XMFLOAT3(mi_args_.pos));
	obj_->SetRotation(XMFLOAT3(0, 180.0f, 0));

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

void Missile::Initialize()
{}

void Missile::Finalize()
{}

void Missile::Update()
{
	// 自然消滅
	// 寿命が尽きた && エミッターの終了準備が出来た
	if (mi_args_.life <= 0 && emitter_->NoticeCanTerminate())
	{
		// 殺す
		is_dead_ = true;
		return;
	}

	if (mi_args_.life > 0)
	{
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
}

void Missile::Draw()
{
	if (mi_args_.is_validity) { obj_->Draw(); }
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (mi_args_.is_validity) { coll_obj_->Draw(); }
}

void Missile::DebugDraw()
{}

void Missile::Activate()
{
	is_dead_ = false;
	mi_args_.is_validity = true;
}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPosition();
	pos.z += mi_args_.vel.z;
	obj_->SetPosition(pos);
}

void Missile::HomingTarget(const XMFLOAT3 &target_pos)
{
	XMFLOAT3 pos = obj_->GetPosition();

	static XMVECTOR old_len{};
	const float speed = 3.0f;	// 仮

	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;
		pos.z += speed;
		obj_->SetPosition(pos);

		return;
	}

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPosition());
	XMVECTOR ta_vec = XMLoadFloat3(&mi_args_.tgt_pos);
	//XMVECTOR tgt_vec = XMLoadFloat3(&target_pos);

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec =
	{
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
		obj_->SetPosition(pos);

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
		obj_->SetPosition(pos);

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
	obj_->SetPosition(pos);

	/* -- 外積追尾 -- */
	// XMVECTORに変換
	//XMVECTOR mi_vec = XMLoadFloat3(&object_->GetPosition());
	//XMVECTOR tgt_vec = XMLoadFloat3(&target_pos);

	//// ふたつの座標を結ぶベクトルを計算
	//XMVECTOR vec = {
	//	(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
	//	(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
	//	(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
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
	const float speed = 3.0f;	// 仮
	XMFLOAT3 pos = obj_->GetPosition();

	// 敵がいないなら
	if (enemies.GetEnemies().size() <= 0)
	{
		// 直進させる
		MoveZ(speed);

		return;
	}

	// 無追尾時間中なら
	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;

		// 直進させる
		MoveZ(speed);

		return;
	}

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPosition());

	// 追尾したい敵のIDが入っている要素の添字を検索
	int index = enemies.GetEnemyIndexWithID(mi_args_.tgt_id);

	// 居なかったら
	if (index == (int)(NacamError::NonDetected))
	{
		// 以前の速度をそのまま加算
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// 位置を反映
		obj_->SetPosition(pos);

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
	min_dist_ = len.m128_f32[0];

	// 追尾範囲外なら
	if (len.m128_f32[0] >= mi_args_.detection_range)
	{
		// 直進だけして
		MoveZ(speed);

		// その後の追尾処理をスキップ
		return;
	}

	// ミサイルが敵を追い越したら
	if (pos.z >= enemies.GetPos(index).z)
	{
		// 以前の速度をそのまま加算
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// 位置を反映
		obj_->SetPosition(pos);

		return;
	}

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_args_.vel));

	rot_dead_zone_ = 1.0f;

	// 離れている距離
	XMVECTOR differ =
	{
		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	// 回頭角度を適用
	differ.m128_f32[0] /= rot_dead_zone_;
	differ.m128_f32[1] /= rot_dead_zone_;
	differ.m128_f32[2] /= rot_dead_zone_;

	// 速度を加算
	mi_args_.vel.x += differ.m128_f32[0] * speed;
	mi_args_.vel.y += differ.m128_f32[1] * speed;
	mi_args_.vel.z = speed;

	// 位置を更新
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// 位置を反映
	obj_->SetPosition(pos);
}

void Missile::TermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPosition());
	emitter_->GenerateParticle();
}
