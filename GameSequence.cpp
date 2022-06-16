#include "GameSequence.h"
#include "SceneManager.h"
#include "FPSManager.h"

#include "DrawProc.h"

void GameSequence::MainGameLoop() {

	/*-- ���������� --*/
	nacam_lib_->NacamLib_Initialize();

	// �ʏ�~�܂�Ȃ��G���[�������I�Ɏ~�߂�
	nacam_lib_->DetectNonStopError(false);

	/*-- �����V�[����ݒ� --*/
	//SceneManager::SetScene(Scene::REPULSION_DEMO);
	SceneManager::SetScene(Scene::PARTICLE_DEMO);

	/*-- �Q�[�����[�v --*/
	while (!nacam_lib_->CatchQuitSignal()) {

		/*-- fps���� --*/
		FpsManager::RegulateFps(60);

		/*-- �X�V --*/
		nacam_lib_->NacamLib_Update();

		/*-- �`�� --*/
		nacam_lib_->NacamLib_Draw();

//#ifdef _DEBUG

		/*-- �f�o�b�O�`�� --*/
		nacam_lib_->NacamLib_DebugDraw();

//#endif

		/*-- �`��㏈�� --*/
		nacam_lib_->NacamLib_PostDraw();
	}

	/*-- �I������ --*/
	nacam_lib_->NacamLib_Finalize();
}

// FBX�V�[�����������ƃG���[�ɂȂ�I
// �_�[�e�B�[�t���O�������������I
