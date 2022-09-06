#pragma once
#include "../AbstractScene.h"
#include "../../Camera/Camera.h"
#include <memory>
#include <wrl.h>
#include "../../Particle/Emitter.h"
#include <DirectXMath.h>
#include <forward_list>

class ParticleDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	enum class MODE {

		MONO,
		MISSILE,
		MISSILE_CONTAINER
	};

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// �V��
	std::unique_ptr<Object3d> sky_dome_;

	// �V�����f��
	std::unique_ptr<Model> model_sky_dome_;

	// �e�X�g�p�[�e�B�N��
	std::unique_ptr<Emitter> contrail_1_;
	std::unique_ptr<Emitter> contrail_2_;
	std::unique_ptr<Emitter> emitter_1_;

	// �G�~�b�^�[�R���e�i
	std::forward_list<Emitter> contrails_1_;

	std::unique_ptr<Emitter> dust_;

	// ���[�h�i�[
	MODE mode_;

	// �G�~�b�^�[�̎���
	int emitter_life_;

	// ���Z�b�g���邩
	bool notice_reset_ = false;

	// �G�~�b�^�[�𐶐����邩
	bool notice_generate_ = false;

	bool is_wire_ = true;

public:

	ParticleDemoScene();
	~ParticleDemoScene();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

public:

	void ResetParam();
	void ResetPos();
	void MoveZ();
};