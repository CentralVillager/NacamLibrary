#pragma once
#include "BackBoard.h"
#include "Singleton.h"
#include "AbstractUniqueObject.h"
#include "Model.h"
#include <memory>
#include <array>

class BackBoardManager : public Singleton<BackBoardManager> {
	friend Singleton<BackBoardManager>;

	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ボードの最大数
	const static short BOARD_NUM_ = 10;

private:

	// ボード
	std::array<BackBoard, BOARD_NUM_> back_board_;

public:

	BackBoardManager();
	~BackBoardManager();

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

};
