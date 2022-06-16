#include "PipelineManager.h"
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "DirectXBase.h"

using namespace std;
using namespace Microsoft::WRL;

DirectXBase *PipelineManager::dx_base_ = DirectXBase::GetInstance();
ComPtr<ID3D12Device> PipelineManager::device_;
