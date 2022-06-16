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

	// チャンクヘッダ
	struct ChunkHeader {
		char id[4];			// チャンクごとのID
		int32_t size;		// チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk;	// "RIFF"
		char type[4];		// "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk;	// "fmt"
		WAVEFORMATEX fmt;	// 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex;				// 波形フォーマット
		BYTE *buffer_ptr;				// バッファの先頭アドレス
		unsigned int buffer_size;		// バッファサイズ
		IXAudio2SourceVoice *source_voice;
		bool is_playing = false;		// 再生中か
		bool allow_duplicate = false;	// 重複を許可するか
	}sound_data_;

	// 単発かループか
	enum PlayStyle {
		SINGLE = 0,
		LOOP = 255
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// .wavファイルの読み込み
	/// </summary>
	/// <param name="file_name">ファイル名</param>
	/// <returns></returns>
	void LoadWaveFile(const std::string &directory, const std::string &file_name);

	/// <summary>
	/// 読み込んだ.wavファイルの解放
	/// </summary>
	/// <param name="sound_data"></param>
	void UnloadWaveFile();

	/// <summary>
	/// 読み込んだ.wavファイルの再生	
	/// </summary>
	/// <param name="x_audio2"></param>
	/// <param name="sound_data"></param>
	void PlayWave(float volume, PlayStyle play_style, bool allow_duplicate);

	/// <summary>
	/// 再生している.wavファイルの停止
	/// </summary>
	/// <param name="sound_data"></param>
	void StopWave();
};

