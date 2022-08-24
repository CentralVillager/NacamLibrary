#pragma once
#include <stack>
#include <memory>
#include <array>
#include "../AbstractScene.h"

// �S�V�[����
enum class SceneName : unsigned int
{
	SPLASH,		// ���S�Ȃǂ�\��
	TITLE,		// �^�C�g����\��
	MAIN,		// �Q�[���̃��C���v�f�̃V�[��
	RESULT,		// ���U���g��\��
	TEMPORARY,	// �f�o�b�O�⌟�؂Ȃǂ̉��V�[��
	DEMO,		// ������������p�̃V�[��
	GRAVITY_DEMO,
	FRICTION_DEMO,
	POST_EFFECT_DEMO,
	PARTICLE_DEMO,
	REPULSION_DEMO,
	EXECUTE_INDIRECT_DEMO,
	MATH_DEMO
};

class SceneManager
{
	enum class SceneState : unsigned int
	{
		Current,
		Next,
		StateNum
	};

	// �V�[���𗭂߂邽�߂̃X�^�b�N
	static stack<shared_ptr<AbstractScene>> scene_stack_;

	static std::array<SceneName, (int)(SceneState::StateNum)> scene_state_;

public:

	static const stack<shared_ptr<AbstractScene>> &GetSceneStack() { return scene_stack_; }

	/// <summary>
	/// �V�[���؂�ւ��̎��s
	/// </summary>
	static void ExecuteSceneChange();

	/// <summary>
	/// �A�v�����������̃V�[���̐ݒ�
	/// </summary>
	/// <param name="scene_name"></param>
	static void SetInitialScene(SceneName scene_name);

	/// <summary>
	/// ���ɉf�������V�[���̃Z�b�g
	/// �A�v�����ł̃V�[���؂�ւ����ɂ͊�{�I�ɂ��̊֐����g�p
	/// </summary>
	static void SetNextScene(SceneName name);

	/// <summary>
	/// �V�[���؂�ւ����s���悤�Ƃ��Ă��邱�Ƃ�ʒm
	/// </summary>
	static bool NoticeChangeScene();

	/// <summary>
	/// �ЂƂO�̃V�[���ɖ߂�
	/// </summary>
	static void ReturnScene();

	/// <summary>
	/// �X�^�b�N�ɐς܂�Ă���V�[����S�ĉ���
	/// </summary>
	static void ClearSceneStack();

	/// <summary>
	/// �f�o�b�O�`��
	/// </summary>
	static void DebugDraw();
};
