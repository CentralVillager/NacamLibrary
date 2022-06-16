#include "BackBoard.h"
#include "ImGuiManager.h"

std::unique_ptr<Model> BackBoard::model_board_;

BackBoard::BackBoard() {

	// ���f����null�Ȃ�
	if (!model_board_) {

		// ���f���f�[�^��ǂݍ���
		model_board_ = make_unique<Model>();
		model_board_->LoadObjModel("Resources/board/", "board.obj", "board.mtl");
	}

	// �`��f�[�^���쐬����
	board_ = make_shared<Object3d>();
}

BackBoard::~BackBoard() {
}

void BackBoard::Initialize(XMFLOAT3 pos, bool is_upper) {

	// �`��f�[�^������������
	board_->Initialize();

	// ���f�����Z�b�g����
	board_->SetModel(model_board_.get());

	// �ʒu���w�肷��
	board_->SetPosition(pos);

	// �X�P�[�����w�肷��
	board_->SetScale({ 2000.0f, 1.0f, 20.0f });

	// �㑤�ɔz�u���邩�ǂ������߂�
	is_upper_ = is_upper;

	// �L���ɂ���
	is_active_ = true;
}

void BackBoard::Finalize() {
}

void BackBoard::Update() {

	// �����ɂȂ�����
	if (!is_active_) {

		// �ʒu�����Z�b�g����
		ResetPosition();
	}

	// �L���Ȃ�
	if (is_active_) {

		// ������
		Move();

		// �{�[�h�����܂ł�������
		if (board_->GetPosition().z <= -20.0f) {

			// �����ɂ���
			is_active_ = false;
		}

		// �X�V
		board_->Update();
	}
}

void BackBoard::Draw() {

	// �L���Ȃ�
	if (is_active_) {

		// �`��
		board_->Draw();
	}
}

void BackBoard::DebugDraw() {
}

void BackBoard::Move() {

	// ���ʒu���擾
	XMFLOAT3 pos = board_->GetPosition();

	// z�ʒu����O�ɂ��炷
	pos.z -= 2.0f;

	// �ʒu���Z�b�g����
	board_->SetPosition(pos);
}

BackBoard BackBoard::Activate(XMFLOAT3 pos, bool is_upper) {

	// �C���X�^���X�𐶐�
	BackBoard b;

	// ������
	b.Initialize(pos, is_upper);

	// �L���ɂ���
	b.is_active_ = true;

	// �㑤�ɔz�u���邩���߂�
	b.is_upper_ = is_upper;

	// �Ԃ�
	return b;
}

void BackBoard::ResetPosition() {

	// �㑤�ɔz�u����Ȃ�
	if (is_upper_) {

		// ���̈ʒu
		board_->SetPosition({ 0, 30.0f, 500.0f });

		// ����ȊO�i���j�Ȃ�
	} else {

		// ���̈ʒu
		board_->SetPosition({ 0, -30.0f, 500.0f });

	}

	// �L���ɂ���
	is_active_ = true;
}
