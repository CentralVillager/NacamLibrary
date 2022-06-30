#pragma once
#include "DirectXBase.h"
#include "Singleton.h"
#include <memory>
#include "SceneManager.h"

#include "PostEffect.h"

class NacamLib  final : public Singleton<NacamLib> {
	friend Singleton<NacamLib>;

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	DirectXBase *dx_base_ = DirectXBase::GetInstance();
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	std::unique_ptr<PostEffect> post_effect_scene_;

private:

	~NacamLib() override;

public:

	/* ��΂ɕK�v�Ȋ֐� */

	/// <summary>
	/// ����������
	/// </summary>
	void NacamLib_Initialize(Scene initial_scene_name);

	/// <summary>
	/// �I������
	/// </summary>
	void NacamLib_Finalize();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="fps">fps����</param>
	void NacamLib_Update(int fps);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void NacamLib_Draw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void NacamLib_PostDraw();
	void NacamLib_DebugDraw();

private:

	void Win32AppInitialize();
	void DirectXInitialize();
	void InputInitialize();
	void GameObjectInitialize();
	void ImGuiInitialize();
	void SceneInitialize(Scene initial_scene_name);

public:

	/// <summary>
	/// �I�����߂��󂯎��
	/// </summary>
	/// <returns></returns>
	bool CatchQuitSignal();

	/// <summary>
	/// �ʏ�~�܂�Ȃ��G���[�������I�Ɏ~�߂邩
	/// </summary>
	/// <param name="should_activate">�L���ɂ���Ȃ�true</param>
	void DetectNonStopError(bool should_activate);
};

