#pragma once
#include <stack>
#include <memory>
#include "AbstractScene.h"
#include "Singleton.h"

// �S�V�[����
enum class Scene {

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

};

class SceneManager : public Singleton<SceneManager> {
	friend Singleton<SceneManager>;

	// �V�[���𗭂߂邽�߂̃X�^�b�N
	static stack<shared_ptr<AbstractScene>> scene_stack_;

public:

	static const stack<shared_ptr<AbstractScene>> &GetSceneStack() { return scene_stack_; }

	/// <summary>
	/// �w�肵���V�[���Ɉڂ�܂��B
	/// </summary>
	/// <param name="scene_name">���ɉf�������V�[����</param>
	static void SetScene(Scene scene_name);

	/// <summary>
	/// �ЂƂO�̃V�[���ɖ߂�܂��B
	/// </summary>
	static void ReturnScene();

	/// <summary>
	/// �X�^�b�N�ɐς܂�Ă���V�[����S�ĉ������܂��B
	/// </summary>
	static void ClearSceneStack();
};
