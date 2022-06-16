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

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = x_audio2_->CreateMasteringVoice(&master_voice_);
}

void AudioManager::LoadWaveFile(const std::string &directory, const const std::string &file_name) {
	HRESULT result;

	std::string &full_path = directory + file_name + ".wav";

	// ファイルオープン
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(full_path, std::ios_base::binary);
	assert(file.is_open());

	// .wavデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read(reinterpret_cast<char *>(&riff), sizeof(riff));

	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};

	// チャンクヘッダーの確認
	file.read(reinterpret_cast<char *>(&format), sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read(reinterpret_cast<char *>(&format.fmt), format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read(reinterpret_cast<char *>(&data), sizeof(data));

	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read(reinterpret_cast<char *>(&data), sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char *p_buffer = new char[data.size];
	file.read(p_buffer, data.size);

	// ファイルクローズ
	file.close();

	// 読み込んだデータを元にSoundDataを生成
	sound_data_.wfex = format.fmt;
	sound_data_.buffer_ptr = reinterpret_cast<BYTE *>(p_buffer);
	sound_data_.buffer_size = data.size;

	// 波形フォーマットを元にSourceVoiceの生成	
	IXAudio2SourceVoice *source_voice_ptr = nullptr;
	result = x_audio2_->CreateSourceVoice(&source_voice_ptr, &sound_data_.wfex);
	assert(SUCCEEDED(result));
	sound_data_.source_voice = source_voice_ptr;
}

void AudioManager::UnloadWaveFile() {
	// バッファのメモリを解放
	delete[] sound_data_.buffer_ptr;

	sound_data_.buffer_ptr = 0;
	sound_data_.buffer_size = 0;
	sound_data_.wfex = {};
}

void AudioManager::PlayWave(float volume, PlayStyle play_style, bool allow_duplicate) {
	if (sound_data_.is_playing && !allow_duplicate) { return; }

	HRESULT result;

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sound_data_.buffer_ptr;
	buf.AudioBytes = sound_data_.buffer_size;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = play_style;
	if (play_style == SINGLE) {
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}

	// 波形データの再生
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
