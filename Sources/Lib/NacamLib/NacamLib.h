#pragma once
#include <memory>
#include "../DirectX/DirectXBase.h"
#include "../DirectX/PipelineManager.h"
#include "../DirectX/PreDraw.h"
#include "../PostEffect/PostEffect.h"
#include "../../App/Scene/SceneManager/SceneManager.h"
#include "../../App/Singleton/Singleton.h"

class NacamLib  final : public Singleton<NacamLib>
{
	friend Singleton<NacamLib>;

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	DirectXBase *dx_base_ = DirectXBase::GetInstance();
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	std::unique_ptr<PostEffect> post_effect_scene_;

	std::unique_ptr<PipelineManager> pipeline_mgr_;

private:

	~NacamLib() override;

public:

	/* ��΂ɕK�v�Ȋ֐� */

	/// <summary>
	/// ����������
	/// </summary>
	void NacamLib_Initialize(SceneName initial_scene_name);

	/// <summary>
	/// �I������
	/// </summary>
	void NacamLib_Finalize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void NacamLib_Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void NacamLib_Draw();

private:

	void Win32AppInitialize();
	void DirectXInitialize();
	void PipelineInitialize();
	void InputInitialize();
	void GameObjectInitialize();
	void ImGuiInitialize();
	void SceneInitialize(SceneName initial_scene_name);

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

