//#pragma once
//#include "Singleton.h"
//#include "DirectXBase.h"
//#include <wrl.h>
//#include "Sprite.h"
//
//class ResourceLoader : public Singleton<ResourceLoader> {
//	friend Singleton<ResourceLoader>;
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//private:
//
//	static DirectXBase *dx_base_;
//	static ComPtr<ID3D12Device> device_;
//
//public:
//
//	unsigned short LoadGraph(const wchar_t *filename);
//	unsigned short LoadObj(const wchar_t *filename);
//	unsigned short LoadSound(const wchar_t *filename);
//
//};
