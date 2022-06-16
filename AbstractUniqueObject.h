#pragma once
#include "ImGuiManager.h"

class AbstractUniqueObject {

public:

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �f�o�b�O�`��
	/// </summary>
	virtual void DebugDraw() = 0;
};
