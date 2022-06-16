#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"

class BackBoard {
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// �{�[�h���f��
	static std::unique_ptr<Model> model_board_;

	// �{�[�h�I�u�W�F�N�g
	std::shared_ptr<Object3d> board_ = nullptr;

	// �L����
	bool is_active_ = false;

	// ��ɔz�u���邩�ǂ���
	bool is_upper_ = false;

public:

	BackBoard();
	~BackBoard();

public:

	void Initialize(XMFLOAT3 pos, bool is_upper);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	// �L���ɂ���
	static BackBoard Activate(XMFLOAT3 pos, bool is_upper);

	// �ړ�������
	void Move();

	// �ʒu�����Z�b�g����
	void ResetPosition();
};
