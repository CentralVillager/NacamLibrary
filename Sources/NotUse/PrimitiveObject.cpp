#include "PrimitiveObject.h"
#include "DirectXBase.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

ComPtr<ID3D12Device> PrimitiveObject::device_ = nullptr;
UINT PrimitiveObject::desc_handle_increment_size_ = 0;
ComPtr<ID3D12GraphicsCommandList> PrimitiveObject::command_list_ = nullptr;
ComPtr<ID3D12DescriptorHeap> PrimitiveObject::desc_heap_;
Camera *PrimitiveObject::cam_ptr_ = nullptr;

PrimitiveObject::PrimitiveObject() {

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	command_list_ = DirectXBase::GetInstance()->GetCommandList().Get();
}

PrimitiveObject::~PrimitiveObject() {
}

void PrimitiveObject::Initialize() {
}

void PrimitiveObject::Finalize() {
}

void PrimitiveObject::Update() {
}

void PrimitiveObject::Draw() {
}
