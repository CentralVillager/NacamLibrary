#pragma once
#include <memory>
#include <wrl.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Model/Model.h"
#include "../../../Lib/Object3d/Object3d.h"

class GravityDemoScene : public AbsScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> object_;

private:

	// �d�͉����x
	const float G_ACCELERATION_ = 9.8f;

private:

	// �ʒu
	float pos_ = 0;

	// �����x
	float g_ = G_ACCELERATION_ / 60.0f;

	// ���x
	float velocity_ = 0;

	// �J�X�^�������x
	float acceleration_ = 0.1f;

private:

	int frame_count_ = 0;

public:

	GravityDemoScene();
	~GravityDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// ����
	/// </summary>
	void Fall();

	/// <summary>
	/// �ҋ@���邩�ǂ���
	/// </summary>
	bool Wait();
};
