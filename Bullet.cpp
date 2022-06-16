#include "Bullet.h"
#include "ImGuiManager.h"

std::unique_ptr<Model> Bullet::model_bullet_ = nullptr;

void Bullet::InitializeModel() {

	// íeÉÇÉfÉãÇÃì«Ç›çûÇ›
	model_bullet_ = make_unique<Model>();
	model_bullet_->LoadObjModel("Resources/bullet/", "bullet.obj", "bullet.mtl");
}

void Bullet::Initialize(float speed, int life) {

	// íeÇÃèâä˙âª
	bullet_ = make_shared<Object3d>();
	bullet_->Initialize();
	bullet_->SetModel(model_bullet_.get());
	bullet_->SetRotation({ 0, 180.0f, 0 });
	bullet_->SetScale({ 0.5f, 0.5f, 0.5f });

	speed_ = speed;
	life_ = life;

	// ìñÇΩÇËîªíËÇÉZÉbÉg
	collision.width = 1.0f;
	collision.height = 1.0f;
	collision.depth = 3.0f;
	collision.center = { bullet_->GetPosition() };
	collision.near_left_upper = {
		collision.center.x - collision.width / 2,
		collision.center.y + collision.height / 2,
		collision.center.z - collision.depth / 2
	};
	collision.near_right_downer = {
		collision.near_left_upper.x + collision.width,
		collision.near_left_upper.y - collision.height,
		collision.near_left_upper.z
	};
	collision.far_left_upper = {
		collision.near_left_upper.x,
		collision.near_left_upper.y,
		collision.near_left_upper.z + collision.depth
	};
	collision.far_right_downer = {
		collision.near_right_downer.x,
		collision.near_right_downer.y,
		collision.near_right_downer.z + collision.depth
	};
}

void Bullet::Finalize() {
}

void Bullet::Update() {

	UpdateAABB();

	// éıñΩÇ™êsÇ´ÇΩÇÁ
	if (life_ <= 0) {

		// éEÇ∑
		is_alive_ = false;
	}

	// íeÇëOï˚Ç…îÚÇŒÇ∑ 
	MoveForward();

	bullet_->Update();
}

void Bullet::Draw() {

	/*ImGui::Text("Bullet Debug");
	ImGui::SliderFloat("bullet_speed", &speed_, 0, 50.0f);
	ImGui::SliderInt("bullet_life", &life_, 0, 100);*/

	bullet_->Draw();
}

void Bullet::Generate(XMFLOAT3 pos, float speed, int life) {

	Initialize(speed, life);
	bullet_->SetPosition(pos);
	is_alive_ = true;
}

void Bullet::MoveForward() {

	// åªç›ÇÃà íuÇéÊìæ
	XMFLOAT3 position = bullet_->GetPosition();

	// ëOêiÇ≥ÇπÇÈ
	position.z += speed_;

	// à íuÇîΩâf
	bullet_->SetPosition(position);

	// éıñΩÇå∏ÇÁÇ∑
	life_--;
}

void Bullet::UpdateAABB() {

	collision.center = { bullet_->GetPosition() };
	collision.near_left_upper = {
		collision.center.x - collision.width / 2,
		collision.center.y + collision.height / 2,
		collision.center.z - collision.depth / 2
	};
	collision.near_right_downer = {
		collision.near_left_upper.x + collision.width,
		collision.near_left_upper.y - collision.height,
		collision.near_left_upper.z
	};
	collision.far_left_upper = {
		collision.near_left_upper.x,
		collision.near_left_upper.y,
		collision.near_left_upper.z + collision.depth
	};
	collision.far_right_downer = {
		collision.near_right_downer.x,
		collision.near_right_downer.y,
		collision.near_right_downer.z + collision.depth
	};
}
