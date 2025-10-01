#include "paddleclass.h"

PaddleClass::PaddleClass()
{
    m_Model = 0;
    m_PosX = 0.0f;
    m_PosY = -2.0f;
    m_Speed = 2.0f;
    halfWidth = 0.5f;
    halfHeight = 0.05f;
    aspectRatio = 800.0f / 600.0f;
    viewWidthUnits = 2.0f * tanf(XM_PIDIV4 * 0.5f) * 5.0f * aspectRatio;
    halfViewWidth = viewWidthUnits / 2.0f;
}

PaddleClass::~PaddleClass()
{
}

bool PaddleClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* texture)
{
    m_Model = new ModelClass;

    ModelClass::VertexType vertices[] =
    {
        {{-halfWidth, -halfHeight, 0.0f}, {0.0f, 1.0f}},
        {{-halfWidth,  halfHeight, 0.0f}, {0.0f, 0.0f}},
        {{ halfWidth,  halfHeight, 0.0f}, {1.0f, 0.0f}},
        {{ halfWidth, -halfHeight, 0.0f}, {1.0f, 1.0f}},
    };

    unsigned long indices[] = { 0, 1, 2, 0, 2, 3 };

    bool result = m_Model->Initialize(device, context, vertices, 4, indices, 6, (char*)texture);
    if (!result)
        return false;

    UpdateWorldMatrix();
    return true;
}

void PaddleClass::Shutdown()
{
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }
}

void PaddleClass::Update(float deltaTime, bool moveLeft, bool moveRight)
{

    if (moveLeft)
        m_PosX -= m_Speed * deltaTime;
    if (moveRight)
        m_PosX += m_Speed * deltaTime;

    if (m_PosX < -halfViewWidth + halfWidth)
        m_PosX = -halfViewWidth + halfWidth;

    if (m_PosX > halfViewWidth - halfWidth)
        m_PosX = halfViewWidth - halfWidth;

    UpdateWorldMatrix();
}

void PaddleClass::UpdateWorldMatrix()
{
    m_WorldMatrix = XMMatrixTranslation(m_PosX, m_PosY, 0.0f);
}

void PaddleClass::Render(ID3D11DeviceContext* context)
{
    m_Model->Render(context);
}

XMMATRIX PaddleClass::GetWorldMatrix()
{
    return m_WorldMatrix;
}

ModelClass* PaddleClass::GetModel()
{
    return m_Model;
}

float PaddleClass::GetPosX() {
    return m_PosX;
}

float PaddleClass::GetPosY() {
    return m_PosY;
}

float PaddleClass::GetHalfWidth() {
    return halfWidth;
}

float PaddleClass::GetHalfHeight() {
    return halfHeight;
}

void PaddleClass::SetPosition(float x) {
    m_PosX = x;
}

void PaddleClass::SetSpeed(float x) {
    m_Speed = x;
}
float PaddleClass::GetSpeed() {
    return m_Speed;
}