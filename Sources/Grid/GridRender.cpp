#include "../Grid/GridRender.h"
#include "../Utility/Utility.h"

Camera *GridRender::cam_ptr_ = nullptr;

void GridRender::Initialize(XMFLOAT3 pos) {

	Line::SetCamera(cam_ptr_);

	for (auto &i : grid_x_) {

		i.Initialize();
	}

	for (auto &i : grid_z_) {

		i.Initialize();
	}

	// xŽ²‚É‚Â‚¢‚Ä
	for (UINT i = 0; i < grid_x_.size(); i++) {

		grid_x_[i].SetRotation({ 0, 90.0f, 0 });
		grid_x_[i].SetScale(50.0f);
		grid_x_[i].SetPosition({ (pos.x + OFFSET_ * i) - (GRID_NUM_ * OFFSET_) / 2, 0, 0 });
	}

	// zŽ²‚É‚Â‚¢‚Ä
	for (UINT i = 0; i < grid_z_.size(); i++) {

		grid_z_[i].SetRotation({ 0, 0, 0 });
		grid_z_[i].SetScale(50.0f);
		grid_z_[i].SetPosition({ 0, 0, (pos.z + OFFSET_ * i) - (GRID_NUM_ * OFFSET_) / 2 });
	}
}

void GridRender::Update() {

	for (auto &i : grid_x_) {

		i.Update();
	}

	for (auto &i : grid_z_) {

		i.Update();
	}
}

void GridRender::Draw() {

	for (auto &i : grid_x_) {

		i.Draw();
	}

	for (auto &i : grid_z_) {

		i.Draw();
	}
}
