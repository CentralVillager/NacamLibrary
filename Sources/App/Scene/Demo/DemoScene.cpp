#include "DemoScene.h"
#include "../../../Lib/Fbx/FbxLoader.h"
#include "../Sources/Lib/Sprite/NcmSprite.h"
#include "../Sources/Lib/Win32App/Win32App.h"
#include "../Sources/Lib/PreDraw/PreDraw.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"
#include "../Sources/Lib/Input/KeyboardInput.h"

using namespace std;

DemoScene::DemoScene()
{
	// �J�����̐���
	camera_ = make_unique<Camera>();

	// FBX���f���f�[�^�̐���
	/*fbx_model_ = make_unique<FbxModel>();

	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");*/
	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	fbx_obj_ = new FbxObject3d();

	texture_ = NcmSprite::LoadTex(L"Resources/Textures/effect1.png");
}

DemoScene::~DemoScene()
{}

void DemoScene::Initialize()
{
	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	FbxObject3d::SetCamera(camera_.get());

	// FBX�I�u�W�F�N�g�f�[�^�̏�����
	fbx_obj_->Initialize();
	fbx_obj_->SetModel(fbx_model_);
	fbx_obj_->SetScale({ 1.0f, 1.0f, 1.0f });
	fbx_obj_->SetPosition({ 0, 0, 0 });

	EaseArgs ease;
	ease.init_value = 0.0f;
	ease.total_move = Win32App::SIZE_.y;
	ease_ = NcmEasing::RegisterEaseData(ease);

	NcmSprite::SetAnchorPoint(texture_, { 0.0f, 0.5f });
	NcmSprite::SetPos(texture_, XMFLOAT2(0.0f, (float)(Win32App::CENTER_.y)));
}

void DemoScene::Finalize()
{}

void DemoScene::Update()
{
	// ��]����
	float rotation_y = fbx_obj_->GetRotation().y;
	rotation_y += 1.0f;
	fbx_obj_->SetRotation({ 0, rotation_y, 0 });

	// �X�V
	fbx_obj_->PlayAnimation(0, true);
	fbx_obj_->Update();

	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		NcmEasing::ResetTime(ease_);
		b = !b;
	}

	if (b)
	{
		NcmEasing::SetInitValue(ease_, (float)(Win32App::SIZE_.x));
		NcmEasing::SetTotalMove(ease_, -1280.0f);
	}
	else
	{
		NcmEasing::SetInitValue(ease_, 0);
		NcmEasing::SetTotalMove(ease_, 1280.0f);
	}

	NcmEasing::UpdateValue(ease_);

	NcmSprite::SetPos(texture_, XMFLOAT2(NcmEasing::GetValue(ease_), (float)(Win32App::CENTER_.y)));
}

void DemoScene::Draw()
{
	//fbx_obj_->Draw(cmd_list_.Get());

	using enum PipelineName;
	PreDraw::PreRender(Sprite);
	NcmSprite::DrawTex(texture_);
}

void DemoScene::DebugDraw()
{}