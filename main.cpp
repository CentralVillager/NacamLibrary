// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// �G���[�S������������
//#pragma warning(disable : 4996)

#include "NacamLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// �i�J�����C�u�����̃C���X�^���X���擾
	NacamLib *nacam_lib_ = NacamLib::GetInstance();

	/*-- ���������� --*/
	nacam_lib_->NacamLib_Initialize(Scene::PARTICLE_DEMO);
	//nacam_lib_->NacamLib_Initialize(Scene::EXECUTE_INDIRECT_DEMO);

	/*-- �Q�[�����[�v --*/
	while (!nacam_lib_->CatchQuitSignal()) {

		/*-- �X�V --*/
		nacam_lib_->NacamLib_Update(60);

		/*-- �`�� --*/
		nacam_lib_->NacamLib_Draw();
	}

	/*-- �I������ --*/
	nacam_lib_->NacamLib_Finalize();

	return 0;
}



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
	�EDXC�R���p�C���̓���

	//�ERelease�r���h���ʂ�Ȃ��I

*/