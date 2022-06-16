#pragma once
#include "InputManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Singleton.h"
#include "NacamLib.h"
#include <memory>

#include "PostEffectDemoScene.h"
#include "PostEffect.h"
#include "DirectXBase.h"

class GameSequence final : public Singleton<GameSequence> {
	friend Singleton<GameSequence>;

	NacamLib *nacam_lib_ = NacamLib::GetInstance();

public:

	/// <summary>
	/// �����őS�Ă�Update��Draw���s���܂��B
	/// </summary>
	void MainGameLoop();
};

/*

	TODO
	//�EImGui�̕\������
	//�EGameSequence�𕪗������ASceneManager�N���X�����
	�E�O���b�h�̕\��
	�E�}�E�X����
	�E���C�g�̒ǉ�
	//�E�N�A�h���̃��t�@�N�^�����O
	//�EDirectXBase�̐���
	�E�N�H�[�^�j�I���ɑΉ�
	�E�v���~�e�B�u�I�u�W�F�N�g�̕`��
	�EXInput�̎���
	�E���������[�N�̉��P
	�E���̂Ƃ���V�[�����X�^�b�N�ς݂ł���Ӗ����Ȃ�

	//�ERelease�r���h���ʂ�Ȃ��I

*/
