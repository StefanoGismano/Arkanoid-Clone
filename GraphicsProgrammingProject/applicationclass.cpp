////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	ballWasLaunched = false;
	isGameOver = false;
	aspectRatio = 800.0f / 600.0f;
	viewWidthUnits = 2.0f * tanf(XM_PIDIV4 * 0.5f) * 5.0f * aspectRatio;
	halfViewWidth = viewWidthUnits / 2.0f;
	viewHeightUnits = 2.0f * tanf(XM_PIDIV4 * 0.5f) * 5.0f;
	halfViewHeight = viewHeightUnits / 2.0f;
	ballStartOffset = 0.05f;
	totalBricks = 0;
	activeBricks = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_Paddle = new PaddleClass;
	result = m_Paddle->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "paddleTexture.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the paddle object.", L"Error", MB_OK);
		return false;
	}

	m_Ball = new BallClass;
	result = m_Ball->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "ballTexture.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ball object.", L"Error", MB_OK);
		return false;
	}

	int numBricksPerRow = 8;
	int numRows = 8;
	float brickSpacing = 0.05f;
	float startX = -2.3f;
	float y = 2.0f;
	totalBricks = activeBricks = numBricksPerRow * numRows;

	for (int i = 0; i < numRows; ++i) {

		for (int i = 0; i < numBricksPerRow; ++i)
		{
			BrickClass* brick = new BrickClass();
			if (i % 2 == 0) {
				bool result = brick->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "brickTexture.tga", 0, 0);
			}
			else {
				bool result = brick->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "brickTexture2.tga", 0, 0);
			}
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize a brick.", L"Error", MB_OK);
				return false;
			}

			float brickWidth = brick->GetHalfWidth() * 2;
			float x = startX + i * (brickWidth + brickSpacing);
			brick->SetPosition(x, y);

			m_Bricks.push_back(brick);
		}
		y -= 0.25;
	}

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Paddle)
	{
		m_Paddle->Shutdown();
		delete m_Paddle;
		m_Paddle = 0;
	}

	if (m_Ball)
	{
		m_Ball->Shutdown();
		delete m_Ball;
		m_Ball = 0;
	}

	for (auto brick : m_Bricks)
	{
		if (brick)
		{
			brick->Shutdown();
			delete brick;
		}
	}
	m_Bricks.clear();

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool ApplicationClass::Frame(float deltaTime, bool moveLeft, bool moveRight, bool launch, bool yes, bool no)
{
	bool result;
	if (!isGameOver) {
		m_Paddle->Update(deltaTime, moveLeft, moveRight);

		if (launch) {
			ballWasLaunched = true;
		}

		if (!ballWasLaunched)
		{
			m_Ball->SetPosition(m_Paddle->GetPosX(), m_Paddle->GetPosY() + m_Paddle->GetHalfHeight() + m_Ball->GetRadius() + ballStartOffset);
			m_Ball->UpdateWorldMatrix();
		}
		else {

			m_Ball->Update(deltaTime);
			int collisionPaddleType = CollisionBallPaddle(m_Ball->GetPosX(), m_Ball->GetPosY(), m_Ball->GetRadius(),
				m_Paddle->GetPosX(), m_Paddle->GetPosY(), m_Paddle->GetHalfWidth(), m_Paddle->GetHalfHeight());
			if (collisionPaddleType == 1)
			{
				m_Ball->BounceYRight();
			}
			else if (collisionPaddleType == 2) {
				m_Ball->BounceYLeft();
			}


			for (auto brick : m_Bricks)
			{
				if (!brick->IsActive())
					continue;

				int collisionBrickType = CollisionBallBrick(
					m_Ball->GetPosX(), m_Ball->GetPosY(), m_Ball->GetRadius(),
					brick->GetX(), brick->GetY(), brick->GetHalfWidth(), brick->GetHalfHeight()
				);

				if (collisionBrickType != 0)
				{
					brick->SetActive(false);
					activeBricks--;
					if (activeBricks == 0) {
						isGameOver = true;
					}
					if (collisionBrickType == 1)
						m_Ball->BounceY();
					else if (collisionBrickType == 2)
						m_Ball->BounceX();
					break;
				}
			}

			if (m_Ball->GetPosY() <= -halfViewHeight + m_Ball->GetRadius() / 2 - 0.1) {
				isGameOver = true;
			}
		}
	}
	else {
		if(yes) {
			ResetGame();
		}
		if (no) { return false; }
	}

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

void ApplicationClass::ResetGame() {
	for (auto brick : m_Bricks) {
		brick->SetActive(true);
	}
	activeBricks = totalBricks;
	m_Paddle->SetPosition(0);
	m_Ball->SetPosition(m_Paddle->GetPosX(), m_Paddle->GetPosY() + m_Paddle->GetHalfHeight() + m_Ball->GetRadius() + ballStartOffset);
	m_Ball->ResetVel();
	isGameOver = false;
	ballWasLaunched = false;
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Paddle->Render(m_Direct3D->GetDeviceContext());


	result = m_TextureShader->Render(
		m_Direct3D->GetDeviceContext(),
		m_Paddle->GetModel()->GetIndexCount(),
		m_Paddle->GetWorldMatrix(),
		viewMatrix,
		projectionMatrix,
		m_Paddle->GetModel()->GetTexture()
	);
	if (!result)
		return false;

	m_Ball->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_Direct3D->GetDeviceContext(),
		m_Ball->GetModel()->GetIndexCount(),
		m_Ball->GetWorldMatrix(),
		viewMatrix,
		projectionMatrix,
		m_Ball->GetModel()->GetTexture()
	);
	if (!result)
		return false;

	for (auto brick : m_Bricks)
	{
		if (!brick->IsActive()) continue;

		brick->Render(m_Direct3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_Direct3D->GetDeviceContext(),
			brick->GetModel()->GetIndexCount(),
			brick->GetWorldMatrix(),
			viewMatrix,
			projectionMatrix,
			brick->GetModel()->GetTexture()
		);
		if (!result) return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

int ApplicationClass::CollisionBallPaddle(float ballX, float ballY, float ballRadius, 
											float paddleX, float paddleY, float paddleHalfWidth, float paddleHalfHeight) {
	
	float closestX = max(paddleX - paddleHalfWidth, min(ballX, paddleX + paddleHalfWidth));
	float closestY = max(paddleY - paddleHalfHeight, min(ballY, paddleY + paddleHalfHeight));

	float dx = ballX - closestX;
	float dy = ballY - closestY;

	float distanceSquared = dx * dx + dy * dy;

	if(distanceSquared < (ballRadius * ballRadius))
	{
		if (ballX > paddleX) 
		{
			return 1;
		}
		else return 2;
	}
	return 0;
}

int ApplicationClass::CollisionBallBrick(float ballX, float ballY, float ballRadius,
	float brickX, float brickY, float brickHalfWidth, float brickHalfHeight)
{
	float closestX = max(brickX - brickHalfWidth, min(ballX, brickX + brickHalfWidth));
	float closestY = max(brickY - brickHalfHeight, min(ballY, brickY + brickHalfHeight));

	float dx = ballX - closestX;
	float dy = ballY - closestY;

	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared < (ballRadius * ballRadius))
	{
		
		if (abs(dx) > abs(dy))
		{
			return 2; // Horizontal bounce
		}
		else
		{
			return 1; // Vertical bounce
		}
	}

	return 0;
}
