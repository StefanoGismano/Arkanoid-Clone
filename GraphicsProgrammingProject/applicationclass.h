////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "paddleclass.h"
#include "inputclass.h"
#include "ballclass.h"
#include "brickclass.h"
#include <vector>


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float deltaTime, bool moveLeft, bool moveRight, bool launch, bool yes, bool no);

private:
	bool Render();
	int CollisionBallPaddle(float BallX, float BallY, float BallRadius, 
		float PaddleX, float PaddleY, float PaddleHalfWidth, float PaddleHalfHeight);
	int CollisionBallBrick(float ballX, float ballY, float ballRadius,
		float brickX, float brickY, float brickHalfWidth, float brickHalfHeight);
	void ResetGame();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TextureShaderClass* m_TextureShader;
	PaddleClass* m_Paddle;
	BallClass* m_Ball;
	std::vector<BrickClass*> m_Bricks;
	bool ballWasLaunched;
	bool isGameOver;
	float aspectRatio;
	float viewWidthUnits;
	float halfViewWidth;
	float viewHeightUnits;
	float halfViewHeight;
	float ballStartOffset;
	int totalBricks;
	int activeBricks;
};

#endif