#include "Model.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

DirectXBase *Model::dx_base_;
ComPtr<ID3D12Device> Model::device_;
UINT Model::descriptor_handle_increment_size_;

Model::Model() {
}

void Model::StaticInitialize() {
	dx_base_ = DirectXBase::GetInstance();
	device_ = dx_base_->GetDevice().Get();
}

void Model::LoadObjModel(const std::string &directory_path, const std::string &file_name, const std::string &material_name) {
	InitializeDescriptorHeap();
	LoadModel(directory_path, file_name);
	LoadMaterial(directory_path, material_name);
}

void Model::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptor_heap_));//����
	if (FAILED(result)) {
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptor_handle_increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::LoadModel(const std::string &directoryPath, const std::string &filename) {
	HRESULT result = S_FALSE;

	// �t�@�C���X�g���[��
	std::ifstream file;
	// obj�t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// ���_���W
	vector<XMFLOAT3> positions;
	// �@���x�N�g��
	vector<XMFLOAT3> normals;
	// �e�N�X�`��UV
	vector<XMFLOAT2> texcoords;

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪����擾
		string key;
		getline(line_stream, key, ' ');

		// �擪������mtllib�Ȃ�}�e���A��
		if (key == "mtl") {
			// �}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		// �擪������	v�Ȃ璸�_���W
		if (key == "v") {
			// X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

			// ���_���W�ɒǉ�
			/*VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);*/
		}

		// �擪������	vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			// U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		// �擪������vn�Ȃ�x�N�g��
		if (key == "vn") {
			// x,y,z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f") {
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short index_position, index_normal, index_texcoord;

				index_stream >> index_position;
				index_stream.seekg(1, ios_base::cur);	// �X���b�V�����΂�
				index_stream >> index_texcoord;
				index_stream.seekg(1, ios_base::cur);	// �X���b�V�����΂�
				index_stream >> index_normal;

				// ���_�f�[�^�̒ǉ�
				VertexData vertex{};
				vertex.position = positions[index_position - 1];
				vertex.normal = normals[index_normal - 1];
				vertex.uv = texcoords[index_texcoord - 1];
				vertices_.emplace_back(vertex);

				// �C���f�b�N�X�f�[�^�̒ǉ�
				indices_.emplace_back(static_cast<unsigned short>(indices_.size()));

				// ���_�C���f�b�N�X�ɒǉ�
				//indices.emplace_back(index_position - 1);
			}
		}
	}
	file.close();

	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices_.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	DirectXBase *dx_base_ = DirectXBase::GetInstance();

	// ���_�o�b�t�@����
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@����
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexData *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = index_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&indexMap));
	if (SUCCEEDED(result)) {
		// �S�C���f�b�N�X�ɑ΂���
		std::copy(indices_.begin(), indices_.end(), indexMap);
		index_buffer_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeVB;
	vertex_buffer_view_.StrideInBytes = sizeof(vertices_[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R16_UINT;
	index_buffer_view_.SizeInBytes = sizeIB;
}

bool Model::LoadTexture(const std::string &directoryPath, const std::string &filename) {
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	string filepath = directoryPath + filename;

	// ���j�R�[�h������ɕϊ�����
	wchar_t w_filepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(),
										  -1, w_filepath, _countof(w_filepath));

	result = LoadFromWICFile(w_filepath, WIC_FLAGS_NONE, &metadata, scratchImg);

	if (FAILED(result)) {
		return result;
	}

	const Image *img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&tex_data_.texture_buffer));
	if (FAILED(result)) {
		return result;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = tex_data_.texture_buffer->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result)) {
		return result;
	}

	// �T�C�Y���擾
	descriptor_handle_increment_size_ = dx_base_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �V�F�[�_���\�[�X�r���[�쐬
	tex_data_.cpu_desc_handle_SRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptor_handle_increment_size_);
	tex_data_.gpu_desc_handle_SRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptor_handle_increment_size_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = tex_data_.texture_buffer->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dx_base_->GetDevice()->CreateShaderResourceView(tex_data_.texture_buffer.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
													&srvDesc, //�e�N�X�`���ݒ���
													tex_data_.cpu_desc_handle_SRV
	);

	return true;
}

void Model::LoadMaterial(const std::string &directoryPath, const std::string &filename) {
	// �t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// ��s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// ��s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());	// �擪�̕������폜
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			// �}�e���A�����ǂݍ���
			line_stream >> material_data_.name_;
		}

		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material_data_.ambient.x;
			line_stream >> material_data_.ambient.y;
			line_stream >> material_data_.ambient.z;
		}

		// �擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material_data_.diffuse.x;
			line_stream >> material_data_.diffuse.y;
			line_stream >> material_data_.diffuse.z;
		}

		// �擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material_data_.specular.x;
			line_stream >> material_data_.specular.y;
			line_stream >> material_data_.specular.z;
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material_data_.texture_file_name;
			// �e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material_data_.texture_file_name);
		}
	}
}
