#include "MathDemoScene.h"
#include "../../Math/Easing/Easing.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../../../Lib/PreDraw/PreDraw.h"
#include "../../../Lib/Win32App/Win32App.h"
#include "../../../Lib/Input/KeyboardInput.h"

using namespace DirectX;

//MathDemoScene::MathDemoScene() {
//
//	camera_ = make_unique<Camera>();
//
//	model_ = make_unique<Model>();
//	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
//
//	object_ = make_unique<Object3d>();
//	line_ = make_unique<Line>();
//}
//
//MathDemoScene::~MathDemoScene() {
//}
//
//void MathDemoScene::Initialize() {
//
//	camera_->Initialize();
//	camera_->SetDistance(20.0f);
//	camera_->MoveCameraTrack({ 0, 0, 0 });
//	Object3d::SetCamera(camera_.get());
//	Line::SetCamera(camera_.get());
//
//	object_->Initialize();
//	object_->SetModel(model_.get());
//	object_->SetPosition({ 0, 0, 0 });
//
//	sphere_.center = XMLoadFloat3(&object_->GetPosition());
//	sphere_.radius = object_->GetScale().x;
//
//	line_->Initialize();
//	line_->SetPosition({ 5.0f, 0.0f, 0 });
//
//	XMFLOAT3 temp = XMFLOAT3(0.0f, 10.0f, 0.0f);
//	XMFLOAT3 temp2 = XMFLOAT3(10.0f, 0.0f, 0.0f);
//
//	line_col_.start = XMLoadFloat3(&temp);
//	line_col_.end = XMLoadFloat3(&temp2);
//}
//
//void MathDemoScene::Finalize() {
//}
//
//void MathDemoScene::Update() {
//
//	camera_->BasicCameraMove(0.2f);
//	static float t = 0.0f;
//
//	if (KeyboardInput::TriggerKey(DIK_SPACE)) {
//
//		camera_->ResetCamera();
//		t = 0.0f;
//	}
//
//	if (KeyboardInput::PushKey(DIK_UP)) {
//
//		XMFLOAT3 pos = line_->GetPosition();
//		pos.y += 0.5f;
//		line_->SetPosition(pos);
//
//		line_col_.start.m128_f32[1] += 0.5f;
//		line_col_.end.m128_f32[1] += 0.5f;
//	}
//
//	if (KeyboardInput::PushKey(DIK_DOWN)) {
//
//		XMFLOAT3 pos = line_->GetPosition();
//		pos.y -= 0.5f;
//		line_->SetPosition(pos);
//
//		line_col_.start.m128_f32[1] -= 0.5f;
//		line_col_.end.m128_f32[1] -= 0.5f;
//	}
//
//	if (KeyboardInput::PushKey(DIK_RIGHT)) {
//
//		XMFLOAT3 pos = line_->GetPosition();
//		pos.x += 0.5f;
//		line_->SetPosition(pos);
//
//		line_col_.start.m128_f32[0] += 0.5f;
//		line_col_.end.m128_f32[0] += 0.5f;
//	}
//
//	if (KeyboardInput::PushKey(DIK_LEFT)) {
//
//		XMFLOAT3 pos = line_->GetPosition();
//		pos.x -= 0.5f;
//		line_->SetPosition(pos);
//
//		line_col_.start.m128_f32[0] -= 0.5f;
//		line_col_.end.m128_f32[0] -= 0.5f;
//	}
//
//	if (Collision::CheckLine2Circle(line_col_, sphere_)) {
//
//		is_hit_ = true;
//
//	} else {
//
//		is_hit_ = false;
//	}
//
//	object_->Update();
//	line_->Update();
//}
//
//void MathDemoScene::Draw() {
//
//	PreDraw::PreRender(PipelineName::Object3d);
//	object_->Draw();
//
//	PreDraw::PreRender(PipelineName::Line);
//	line_->Draw();
//}
//
//void MathDemoScene::DebugDraw() {
//
//	ImGui::DragFloat3("pos", &line_col_.start.m128_f32[0]);
//	ImGui::DragFloat3("pos", &line_col_.end.m128_f32[0]);
//	ImGui::Checkbox("hit", &is_hit_);
//}

//const float Obj::RADIUS_ = 1.0f;
//const float Obj::LENGTH_LINK_ = 2.0f;
//const float Obj::K_HOOK_ = 0.01f;
//const float Obj::K_V_RES_ = 0.1f;
//const float Obj::ACC_G_ = -0.2f;

//MathDemoScene::MathDemoScene() {
//
//	camera_ = make_unique<Camera>();
//
//	model_ = make_unique<Model>();
//	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
//}
//
//MathDemoScene::~MathDemoScene() {
//}
//
//void MathDemoScene::Initialize() {
//
//	camera_->Initialize();
//	camera_->SetDistance(20.0f);
//	camera_->MoveCameraTrack({ 0, 0, 0 });
//	Object3d::SetCamera(camera_.get());
//	Line::SetCamera(camera_.get());
//
//	for (auto &i : object_) {
//
//		i.Initialize();
//		i.SetModel(model_.get());
//	}
//
//	object_[0].SetPosition({ 0, 5.0f, 0 });
//	object_[1].SetPosition({ 0, -5.0f, 0 });
//	object_[2].SetPosition({ 0, -10.0f, 0 });
//
//	obj_[0].stat = 2;
//	obj_[0].pos.y = 10.0f;
//
//	obj_[1].stat = 1;
//	obj_[1].vel.clear(0);
//	obj_[1].pos = obj_[0].pos;
//	// �����̐eobj���P���obj��
//	obj_[1].link0 = &obj_[0];
//	// �P���obj�̎qobj��������
//	obj_[0].link1 = &obj_[1];
//
//	obj_[2].stat = 1;
//	obj_[2].vel.clear(0);
//	obj_[2].pos = obj_[1].pos;
//	// �����̐eobj���P���obj��
//	obj_[2].link0 = &obj_[1];
//	// �P���obj�̎qobj��������
//	obj_[1].link1 = &obj_[2];
//}
//
//void MathDemoScene::Finalize() {
//}
//
//void MathDemoScene::Update() {
//
//	camera_->BasicCameraMove(0.2f);
//	static float t = 0.0f;
//
//	if (KeyboardInput::TriggerKey(DIK_SPACE)) {
//
//		camera_->ResetCamera();
//		t = 0.0f;
//	}
//
//	for (auto &i : obj_) {
//
//		i.update();
//	}
//
//	object_[0].SetPosition({ obj_[0].pos.x, obj_[0].pos.y, 0});
//	object_[1].SetPosition({ obj_[1].pos.x, obj_[1].pos.y, 0});
//	object_[2].SetPosition({ obj_[2].pos.x, obj_[2].pos.y, 0});
//
//	for (auto &i : object_) {
//
//		i.Update();
//	}
//}
//
//void MathDemoScene::Draw() {
//
//	PreDraw::PreRender(PipelineName::Object3d);
//
//	for (auto &i : object_) {
//
//		i.Draw();
//	}
//}
//
//void MathDemoScene::DebugDraw() {
//
//	XMFLOAT3 pos = { obj_[0].pos.x, obj_[0].pos.y, 0};
//	ImGuiManager::DragFloat3("pos", pos, 1.0f, -30.0f, 30.0f);
//	obj_[0].pos.x = pos.x;
//	obj_[0].pos.y = pos.y;
//
//	XMFLOAT3 pos1 = { obj_[1].pos.x, obj_[1].pos.y, 0};
//	ImGuiManager::DragFloat3("pos1", pos1, 1.0f, -30.0f, 30.0f);
//	obj_[1].pos.x = pos1.x;
//	obj_[1].pos.y = pos1.y;
//
//	XMFLOAT3 pos2 = { obj_[2].pos.x, obj_[2].pos.y, 0 };
//	ImGuiManager::DragFloat3("pos2", pos2, 1.0f, -30.0f, 30.0f);
//	obj_[2].pos.x = pos2.x;
//	obj_[2].pos.y = pos2.y;
//}
//
//void Obj::update() {
//
//	hp[idx_hist % NUM_HIST_].x = pos.x;
//	hp[idx_hist % NUM_HIST_].y = pos.y;
//	idx_hist++;
//
//	// 0:���݂��ĂȂ��@1:���݂��Ċ������@2:���݂��ĂĊ������ĂȂ�
//	if (stat == 2)
//		return;
//
//	{
//		// acc:�����x
//		acc.x = 0.0f;
//		acc.y = ACC_G_;	//�d�͉����x�i�l�͓K���ŗǂ��j
//
//		if (link0)	//�eOBJ������?
//		{
//			// ������1���obj�ւ̃x�N�g��
//			Vec2	_dist = link0->pos - pos;
//			// ���̃x�N�g���̑傫��
//			double	_len = _dist.length();
//
//			// �eobj�Ƃ̋������W�������iLENGTH_LINK_�j���傫�����
//			// �����x�����Z
//			if (_len > LENGTH_LINK_) {
//				// �x�N�g��(_dist)���W����������̑���
//				_dist = _dist * (_len - LENGTH_LINK_);
//
//				_dist /= LENGTH_LINK_;
//
//				// K_HOOK_:�΂˒萔�i�L�тɂ����j
//				// ����(m)���傫���ق�_dist���|���������x(acc)�̑����͌���
//				Vec2 add_acc = _dist * K_HOOK_ / m;
//				acc += add_acc;
//			}
//		}
//		if (link1)	//�qOBJ������?
//		{
//			// �����̂P����obj�ւ̃x�N�g��
//			Vec2	_dist = link1->pos - pos;
//			// ���̃x�N�g���̑傫��
//			double	_len = _dist.length();
//
//			if (_len > LENGTH_LINK_) {
//				_dist = _dist * (_len - LENGTH_LINK_) / LENGTH_LINK_;
//				acc += _dist * K_HOOK_ / m;
//			}
//		}
//
//		vel += acc;			//���x+=�����x
//
//		vel -= vel * K_V_RES_;	//�S����R
//								// (���̂��ߖT�̗��̂��������邱�Ƃɂ���Ď󂯂锽��p)
//
//		pos += vel;			//�ʒu+=���x
//	}
//}

MathDemoScene::MathDemoScene() {

	camera_ = make_unique<Camera>();

	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
}

MathDemoScene::~MathDemoScene() {
}

void MathDemoScene::Initialize() {

	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	Object3d::SetCamera(camera_.get());
	Line::SetCamera(camera_.get());

	for (auto &i : object_) {

		i.Initialize();
		i.SetModel(model_.get());
	}

	object_[0].SetPosition({ 0, 0.0f, 0 });
	object_[1].SetPosition({ 0, 0.0f, 0 });
	object_[1].SetScale(0.2f);

	x = CLENGTH_ / 8.0f;
	speed = 0.0f;
}

void MathDemoScene::Finalize() {
}

void MathDemoScene::Update() {

	camera_->BasicCameraMove(0.2f);
	static float t = 0.0f;

	if (KeyboardInput::TriggerKey(DIK_SPACE)) {

		camera_->ResetCamera();
		t = 0.0f;
	}

	// �����ɏ]���đ��x�����Z
	// MASS�̒l������������Ƃ�����蓮��
	speed += -MASS_ * G_ * sin(x / LENGTH_);

	// ���x�ɏ]���ĉ~��̍��W��ύX
	x += speed;

	// �������_�Ƃ����ꍇ�̂Ԃ牺�����Ă��镨�̍��W���Z�o
	// ���̂܂܂��Ɓ|45�`45�x�̐U��q�ɂȂ�̂�
	// ���v����90�x�iPI/2.0�j��]
	angle = x / LENGTH_ + PI_ / 2.0f;

	// ���߂��p�x���玲�����_�Ƃ����~����̍��W���擾
	nx = cos(angle) * LENGTH_;
	ny = sin(angle) * LENGTH_;

	object_[0].SetPosition({ jiku_x + nx, jiku_y + ny, 0 });

	for (auto &i : object_) {

		i.Update();
	}
}

void MathDemoScene::Draw() {

	PreDraw::PreRender(PipelineName::Object3d);

	for (auto &i : object_) {

		i.Draw();
	}
}

void MathDemoScene::DebugDraw() {

	//ImGuiManager::DragFloat3("pos", object_[0].GetPosition(),)
}
