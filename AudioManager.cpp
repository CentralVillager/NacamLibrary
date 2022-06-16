#include "AudioManager.h"
#include <fstream>
#include <cassert>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

ComPtr<IXAudio2> AudioManager::x_audio2_;
IXAudio2MasteringVoice *AudioManager::master_voice_;

AudioManager::AudioManager() {
}

void AudioManager::StaticInitialize() {
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// �}�X�^�[�{�C�X�𐶐�
	result = x_audio2_->CreateMasteringVoice(&master_voice_);
}

void AudioManager::LoadWaveFile(const std::string &directory, const const std::string &file_name) {
	HRESULT result;

	std::string &full_path = directory + file_name + ".wav";

	// �t�@�C���I�[�v��
	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(full_path, std::ios_base::binary);
	assert(file.is_open());

	// .wav�f�[�^�ǂݍ���
	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read(reinterpret_cast<char *>(&riff), sizeof(riff));

	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	// �`�����N�w�b�_�[�̊m�F
	file.read(reinterpret_cast<char *>(&format), sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// �`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read(reinterpret_cast<char *>(&format.fmt), format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read(reinterpret_cast<char *>(&data), sizeof(data));

	// JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read(reinterpret_cast<char *>(&data), sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char *p_buffer = new char[data.size];
	file.read(p_buffer, data.size);

	// �t�@�C���N���[�Y
	file.close();

	// �ǂݍ��񂾃f�[�^������SoundData�𐶐�
	sound_data_.wfex = format.fmt;
	sound_data_.buffer_ptr = reinterpret_cast<BYTE *>(p_buffer);
	sound_data_.buffer_size = data.size;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���	
	IXAudio2SourceVoice *source_voice_ptr = nullptr;
	result = x_audio2_->CreateSourceVoice(&source_voice_ptr, &sound_data_.wfex);
	assert(SUCCEEDED(result));
	sound_data_.source_voice = source_voice_ptr;
}

void AudioManager::UnloadWaveFile() {
	// �o�b�t�@�̃����������
	delete[] sound_data_.buffer_ptr;

	sound_data_.buffer_ptr = 0;
	sound_data_.buffer_size = 0;
	sound_data_.wfex = {};
}

void AudioManager::PlayWave(float volume, PlayStyle play_style, bool allow_duplicate) {
	if (sound_data_.is_playing && !allow_duplicate) { return; }

	HRESULT result;

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sound_data_.buffer_ptr;
	buf.AudioBytes = sound_data_.buffer_size;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = play_style;
	if (play_style == SINGLE) {
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}

	// �g�`�f�[�^�̍Đ�
	result = sound_data_.source_voice->SubmitSourceBuffer(&buf);
	result = sound_data_.source_voice->SetVolume(volume);
	result = sound_data_.source_voice->Start();
	sound_data_.is_playing = true;
}

void AudioManager::StopWave() {
	HRESULT result;

	result = sound_data_.source_voice->Stop();
	sound_data_.is_playing = false;
}
