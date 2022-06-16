#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <stdint.h>
#include <string>

#pragma comment(lib, "xaudio2.lib")

class AudioManager {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static ComPtr<IXAudio2> x_audio2_;
	static IXAudio2MasteringVoice *master_voice_;

	AudioManager();

	// �`�����N�w�b�_
	struct ChunkHeader {
		char id[4];			// �`�����N���Ƃ�ID
		int32_t size;		// �`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader {
		ChunkHeader chunk;	// "RIFF"
		char type[4];		// "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk {
		ChunkHeader chunk;	// "fmt"
		WAVEFORMATEX fmt;	// �g�`�t�H�[�}�b�g
	};

	// �����f�[�^
	struct SoundData {
		WAVEFORMATEX wfex;				// �g�`�t�H�[�}�b�g
		BYTE *buffer_ptr;				// �o�b�t�@�̐擪�A�h���X
		unsigned int buffer_size;		// �o�b�t�@�T�C�Y
		IXAudio2SourceVoice *source_voice;
		bool is_playing = false;		// �Đ�����
		bool allow_duplicate = false;	// �d���������邩
	}sound_data_;

	// �P�������[�v��
	enum PlayStyle {
		SINGLE = 0,
		LOOP = 255
	};

	/// <summary>
	/// ����������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// .wav�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="file_name">�t�@�C����</param>
	/// <returns></returns>
	void LoadWaveFile(const std::string &directory, const std::string &file_name);

	/// <summary>
	/// �ǂݍ���.wav�t�@�C���̉��
	/// </summary>
	/// <param name="sound_data"></param>
	void UnloadWaveFile();

	/// <summary>
	/// �ǂݍ���.wav�t�@�C���̍Đ�	
	/// </summary>
	/// <param name="x_audio2"></param>
	/// <param name="sound_data"></param>
	void PlayWave(float volume, PlayStyle play_style, bool allow_duplicate);

	/// <summary>
	/// �Đ����Ă���.wav�t�@�C���̒�~
	/// </summary>
	/// <param name="sound_data"></param>
	void StopWave();
};

