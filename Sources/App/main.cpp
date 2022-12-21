#include "../Lib/NacamLib/NacamLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// �i�J�����C�u�����̃C���X�^���X���擾
	NacamLib *nacam_lib_ = NacamLib::GetInstance();

	/*-- ���������� --*/
	nacam_lib_->NacamLib_Initialize(SceneName::TITLE);

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
