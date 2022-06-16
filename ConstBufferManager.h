#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "Singleton.h"
#include <vector>

class ConstBufferManager : public Singleton<ConstBufferManager> {
	friend Singleton<ConstBufferManager>;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	std::vector<ComPtr<ID3D12Resource>> const_buffer_;

public:

	ConstBufferManager();
	~ConstBufferManager();

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	void Generate();

};
