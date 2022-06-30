#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXTex.h>
#include "DirectXBase.h"
#include <unordered_map>

class Model {
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ���_�f�[�^
	struct VertexData {
		XMFLOAT3 position;	// xyz���W
		XMFLOAT3 normal;	// �@���x�N�g��
		XMFLOAT2 uv;		// uv���W
	};

	// �}�e���A���f�[�^
	struct MaterialData {
		std::string name_;				// �}�e���A����
		XMFLOAT3 ambient;				// �A���r�G���g�e���x
		XMFLOAT3 diffuse;				// �f�B�t���[�Y�e���x
		XMFLOAT3 specular;				// �X�y�L�����[�e���x
		float alpha;					// �A���t�@
		std::string texture_file_name;	// �e�N�X�`���t�@�C����

		// �R���X�g���N�^
		MaterialData() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	}material_data_;

	// �}�e���A���萔�o�b�t�@�p�f�[�^�\����
	struct MaterialConstBufferData {
		XMFLOAT3 ambient;	// �A���r�G���g�W��
		float pad1;			// �p�f�B���O
		XMFLOAT3 diffuse;	// �f�B�t���[�Y�W��
		float pad2;			// �p�f�B���O
		XMFLOAT3 specular;	// �X�y�L�����[�W��
		float alpha;		// �A���t�@
	};

	// �e�N�X�`��
	struct TextureData {
		// �e�N�X�`���o�b�t�@
		ComPtr<ID3D12Resource> texture_buffer;
		// �}�e���A���p�̒萔�o�b�t�@
		MaterialConstBufferData const_buffer;
		// �f�X�N���v�^�e�[�u��
		ComPtr<ID3D12RootSignature> rootSignature;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_SRV;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_SRV;
	}tex_data_;

	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smooth_data_;

private:
	static DirectXBase *dx_base_;
	static ComPtr<ID3D12Device> device_;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptor_handle_increment_size_;

	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertex_buffer_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> index_buffer_;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
	// ���_�f�[�^�z��
	std::vector<VertexData> vertices_;
	// �C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

public:
	Model();

	MaterialData &GetMaterialData() { return material_data_; }
	ComPtr<ID3D12Resource> &GetVB() { return vertex_buffer_; }
	ComPtr<ID3D12Resource> &GetIB() { return index_buffer_; }
	D3D12_VERTEX_BUFFER_VIEW GetVBView() { return vertex_buffer_view_; }
	D3D12_INDEX_BUFFER_VIEW GetIBView() { return index_buffer_view_; }
	const vector<VertexData> &GetVertices() { return vertices_; }
	vector<unsigned short> GetIndices() { return indices_; }
	size_t GetNumIndices() { size_t num = indices_.size(); return num; }
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptor_heap_; }
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuDescHandleSRV() { return tex_data_.gpu_desc_handle_SRV; }

	static void StaticInitialize();

	/// <summary>
	/// Obj�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="directory_path"></param>
	/// <param name="file_name"></param>
	/// <returns></returns>
	void LoadObjModel(const std::string &directory_path, const std::string &file_name, const std::string &material_name);

	/// <summary>
	/// ������
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadModel(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	bool LoadTexture(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string &directoryPath, const std::string &filename);
};
