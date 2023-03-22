#include "../DirectX/PreDraw.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

ComPtr<ID3D12GraphicsCommandList> PreDraw::cmd_list_;
std::unique_ptr<PipelineManager> PreDraw::pipeline_mgr_;

PreDraw::PreDraw()
{}

PreDraw::~PreDraw()
{}

void PreDraw::StaticInitialize(const PipelineManager &p_mgr)
{
	cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();
	pipeline_mgr_ = std::make_unique<PipelineManager>(p_mgr);
}

void PreDraw::SetPipeline(PipelineName p_name)
{
	// パイプラインステートの設定
	cmd_list_->SetPipelineState(pipeline_mgr_->GetPipeline(p_name).pipeline_state.Get());

	// ルートシグネチャの設定
	cmd_list_->SetGraphicsRootSignature(pipeline_mgr_->GetPipeline(p_name).root_signature.Get());

	// プリミティブ形状を設定
	cmd_list_->IASetPrimitiveTopology(pipeline_mgr_->GetPipeline(p_name).primitive_topology);
}
