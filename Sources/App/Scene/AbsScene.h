#pragma once
#include "../../Lib/DebugUtill/NcmImGui.h"

class AbsScene
{
public:

	AbsScene() = default;
	virtual ~AbsScene() = default;

	/// <summary>
	/// �e�V�[���̎��I�u�W�F�N�g�̏���������
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
