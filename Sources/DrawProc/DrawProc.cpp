#include "../DrawProc/DrawProc.h"
#include "../DirectXBase/DirectXBase.h"
#include "../Win32App/Win32App.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

UINT DrawProc::back_buffer_index_;
float DrawProc::clear_color_[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
ComPtr<ID3D12GraphicsCommandList> DrawProc::command_list_;

void DrawProc::StaticInitialize()
{
	command_list_ = DirectXBase::GetInstance()->GetCommandList();
}

void DrawProc::PreDraw(DirectXBase *dx_base)
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂�0�Ԃ�1�ԁj
	back_buffer_index_ = dx_base->GetSwapchain()->GetCurrentBackBufferIndex();

	// ���\�[�X�o���A�ŏ������݉\�ɕύX
	// �\����Ԃ���`���ԂɕύX
	command_list_->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	// �`���w��
	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx_base->GetRtvHeaps()->GetCPUDescriptorHandleForHeapStart(),
		back_buffer_index_,
		dx_base->GetDevice()->GetDescriptorHandleIncrementSize(dx_base->GetHeapDesc().Type)
	);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dx_base->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
	dx_base->GetCommandList()->OMSetRenderTargets(
		1,
		&handle,
		false,
		&dsvH);

	// ��ʃN���A
	dx_base->GetCommandList()->ClearRenderTargetView(handle, clear_color_, 0, nullptr);

	// �[�x�o�b�t�@�̃N���A
	dx_base->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̐ݒ�R�}���h
	dx_base->GetCommandList()->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			(float)(Win32App::GetWindowWidth()),
			(float)(Win32App::GetWindowHeight())));

	// �V�U�[��`�̐ݒ�R�}���h
	dx_base->GetCommandList()->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			(long)(0.0f),
			(long)(0.0f),
			Win32App::GetWindowWidth(),
			Win32App::GetWindowHeight()));
}

void DrawProc::PostDraw(DirectXBase *dx_base)
{
	// �S�D�`��R�}���h �I��
	// �T�D���\�[�X�o���A��߂�
	// �`���Ԃ���\����ԂɕύX
	dx_base->GetCommandList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));

	// ���߂̃N���[�Y
	dx_base->GetCommandList()->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList *command_lists[] = { dx_base->GetCommandList().Get() };	// �R�}���h���X�g�̔z��
	dx_base->GetCommandQueue()->ExecuteCommandLists(1, command_lists);

	// �o�b�t�@���t���b�v
	dx_base->GetSwapchain()->Present(1, 0);

	// �R�}���h�L���[�̎��s������҂�
	dx_base->GetCommandQueue()->Signal(dx_base->GetFence().Get(), dx_base->IncrementFenceVal());

	if (dx_base->GetFence()->GetCompletedValue() != dx_base->GetFenceVal())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		dx_base->GetFence()->SetEventOnCompletion(dx_base->GetFenceVal(), event);

		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	dx_base->GetCommandAllocator()->Reset();

	// �ĂуR�}���h���X�g�𒙂߂鏀��
	dx_base->GetCommandList()->Reset(dx_base->GetCommandAllocator().Get(), nullptr);
}

void DrawProc::ClearDepthBuffer()
{
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(DirectXBase::GetInstance()->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
	DirectXBase::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
