#include "DrawProc.h"
#include "DirectXBase.h"
#include "Window.h"

UINT DrawProc::back_buffer_index_;
float DrawProc::clear_color_[4] = { 0.1f, 0.1f, 0.1f, 0.0f };

void DrawProc::PreDraw(DirectXBase *dx_base) {

	// �o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂�0�Ԃ�1�ԁj
	back_buffer_index_ = dx_base->GetSwapchain()->GetCurrentBackBufferIndex();

	// ���\�[�X�o���A�ŏ������݉\�ɕύX
	// �\����Ԃ���`���ԂɕύX
	dx_base->GetCmdList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		)
	);

	// �`���w��
	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx_base->GetRtvHeaps()->GetCPUDescriptorHandleForHeapStart(),
		back_buffer_index_,
		dx_base->GetDevice()->GetDescriptorHandleIncrementSize(dx_base->GetHeapDesc().Type)
	);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dx_base->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
	dx_base->GetCmdList()->OMSetRenderTargets(
		1,
		&handle,
		false,
		&dsvH
	);

	// ��ʃN���A
	dx_base->GetCmdList()->ClearRenderTargetView(handle, clear_color_, 0, nullptr);

	// �[�x�o�b�t�@�̃N���A
	dx_base->GetCmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̐ݒ�R�}���h
	dx_base->GetCmdList()->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			Window::GetWindowWidth(),
			Window::GetWindowHeight()
		)
	);

	// �V�U�[��`�̐ݒ�R�}���h
	dx_base->GetCmdList()->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			0.0f,
			0.0f,
			Window::GetWindowWidth(),
			Window::GetWindowHeight()
		)
	);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	//dx_init->getCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DrawProc::PostDraw(DirectXBase *dx_base) {

	// �S�D�`��R�}���h �I��
	// �T�D���\�[�X�o���A��߂�
	// �`���Ԃ���\����ԂɕύX
	dx_base->GetCmdList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	);

	// ���߂̃N���[�Y
	dx_base->GetCmdList()->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList *cmdLists[] = { dx_base->GetCmdList().Get() };	// �R�}���h���X�g�̔z��
	dx_base->GetCmdQueue()->ExecuteCommandLists(1, cmdLists);

	// �o�b�t�@���t���b�v
	dx_base->GetSwapchain()->Present(1, 0);

	// �R�}���h�L���[�̎��s������҂�
	dx_base->GetCmdQueue()->Signal(dx_base->GetFence().Get(), dx_base->incrementFenceVal());

	if (dx_base->GetFence()->GetCompletedValue() != dx_base->GetFenceVal()) {

		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		dx_base->GetFence()->SetEventOnCompletion(dx_base->GetFenceVal(), event);

		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	dx_base->GetCmdAllocator()->Reset();

	// �ĂуR�}���h���X�g�𒙂߂鏀��
	dx_base->GetCmdList()->Reset(dx_base->GetCmdAllocator().Get(), nullptr);
}

void DrawProc::ClearDepthBuffer() {

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(DirectXBase::GetInstance()->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
	DirectXBase::GetInstance()->GetCmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
