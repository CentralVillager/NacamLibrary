#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "../DirectX/PipelineManager.h"
#include <memory>

class PreDraw
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	PreDraw();
	~PreDraw();

public:

	static void StaticInitialize(const PipelineManager &p_mgr);

private:

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// PipelineManager�̃C���X�^���X
	static std::unique_ptr<PipelineManager> pipeline_mgr_;

public:

	/// <summary>
	/// �O���t�B�N�X�p�C�v���C�����Z�b�g
	/// </summary>
	/// <param name="p_name"></param>
	static void SetPipeline(PipelineName p_name);
};

