#pragma once
#include <Windows.h>
#include "../Singleton/Singleton.h"
#include <wrl.h>
#include <d3d12.h>
#include "../DirectXBase/DirectXBase.h"

class DrawProc : public Singleton<DrawProc>
{
	friend Singleton<DrawProc>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	// �w�i�F
	static float clear_color_[4];

	// �o�b�N�o�b�t�@�ԍ�
	static UINT back_buffer_index_;

	static ComPtr<ID3D12GraphicsCommandList> command_list_;

public:

	static void StaticInitialize();

	/// <summary>
	/// �o�b�N�o�b�t�@����
	/// </summary>
	/// <param name="dx_base"></param>
	static void PreDraw(DirectXBase *dx_base);

	/// <summary>
	/// �o�b�N�o�b�t�@�`��
	/// </summary>
	/// <param name="dx_base"></param>
	static void PostDraw(DirectXBase *dx_base);

	// �f�v�X�o�b�t�@�̃N���A
	static void ClearDepthBuffer();
};
