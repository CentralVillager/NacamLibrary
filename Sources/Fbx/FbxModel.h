#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

/// <summary>
/// �m�[�h�\����
/// </summary>
struct Node {
	// ���O
	std::string name_;
	// ���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	// ���[�J����]�p
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	// ���[�J���ړ�
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	// ���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	// �O���[�o���ό`�s��
	DirectX::XMMATRIX global_transform;
	// �e�m�[�h
	Node *parent = nullptr;
};

/// <summary>
/// FBX���f���N���X
/// </summary>
class FbxModel {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using stirng = std::string;
	template <class T> using vector = std::vector<T>;

public:
	// �t�����h�N���X
	friend class FbxLoader;

public:	// �萔
	static const int MAX_BONE_INDICES_ = 4;

public:
	// ���_�f�[�^�\����
	struct VertexData {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT bone_index[MAX_BONE_INDICES_];		// �{�[���ԍ�
		float bone_weight[MAX_BONE_INDICES_];	// �{�[���d��
	};

	// �{�[���\����
	struct Bone {
		// ���O
		std::string name;
		// �����p���̋t�s��
		DirectX::XMMATRIX inverse_initial_pose;
		// �N���X�^�[(FBX���̃{�[�����)
		FbxCluster *fbx_cluster;

		// �R���X�g���N�^
		Bone(const std::string &name) { this->name = name; }
	};

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vert_buff_;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> index_buff_;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> tex_buff_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vb_view = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ib_view = {};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> desc_heap_srv;

public:
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	/// <param name="device"></param>
	void CreateBuffers(ID3D12Device *device);

private:
	// ���f����
	std::string name_;
	// �m�[�h�z��
	std::vector<Node> nodes_;
	// ���b�V�������m�[�h
	Node *mesh_node_ = nullptr;
	// ���_�f�[�^�z��
	std::vector<VertexData> vertices_;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

	// �A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	// �f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	// �e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	DirectX::ScratchImage scratch_img = {};

	// �{�[���z��
	std::vector<Bone> bones_;

	// FBX�V�[��
	FbxScene *fbx_scene_ = nullptr;

public:
	~FbxModel();

	const XMMATRIX &GetModelTransform() { return mesh_node_->global_transform; }
	std::vector<Bone> &GetBones() { return bones_; }
	FbxScene *GetFbxScene() { return fbx_scene_; }

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmd_list"></param>
	void Draw(ID3D12GraphicsCommandList *cmd_list);
};
