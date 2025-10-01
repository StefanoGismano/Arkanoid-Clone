#include "ballclass.h"

BallClass::BallClass()
{
    m_Model = 0;
    m_PosX = 1.0f;
    m_PosY = 0.0f;
    m_VelX = 0.0f;
    m_VelY = 0.0f;
    m_Speed = 2.0f;
    m_Radius = 0.05f;
    aspectRatio = 800.0f / 600.0f;
    viewWidthUnits = 2.0f * tanf(XM_PIDIV4 * 0.5f) * 5.0f * aspectRatio;
    halfViewWidth = viewWidthUnits / 2.0f;
    viewHeightUnits = 2.0f * tanf(XM_PIDIV4 * 0.5f) * 5.0f;
    halfViewHeight = viewHeightUnits / 2.0f;
}

BallClass::~BallClass()
{
}


bool BallClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* texture)
{
    m_Model = new ModelClass;

    float r = m_Radius;

    m_VelY = m_Speed;

    ModelClass::VertexType vertices[] =
    {
        {{-r, -r, 0.0f}, {0.0f, 1.0f}},
        {{-r,  r, 0.0f}, {0.0f, 0.0f}},
        {{ r,  r, 0.0f}, {1.0f, 0.0f}},
        {{ r, -r, 0.0f}, {1.0f, 1.0f}},
    };

    unsigned long indices[] = { 0, 1, 2, 0, 2, 3 };

    bool result = m_Model->Initialize(device, context, vertices, 4, indices, 6, (char*)texture);
    if (!result)
        return false;

    UpdateWorldMatrix();
    return true;
}

void BallClass::Shutdown()
{
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }
}

void BallClass::Update(float deltaTime)
{

    m_PosX += m_VelX * deltaTime;
    m_PosY += m_VelY * deltaTime;

    if (m_PosX <= -halfViewWidth + m_Radius / 2) 
    {
        m_PosX = -halfViewWidth + m_Radius / 2 + 0.001;
        m_VelX = -m_VelX;
    }
    if (m_PosX >= halfViewWidth - m_Radius / 2) 
    {
        m_PosX = halfViewWidth - m_Radius / 2 - 0.001;
        m_VelX = -m_VelX;
    }
    if (m_PosY >= halfViewHeight - m_Radius / 2)
    {
        m_PosY = halfViewHeight - m_Radius / 2 - 0.001;
        m_VelY = -m_VelY;
    }

    UpdateWorldMatrix();
}

void BallClass::UpdateWorldMatrix()
{
    m_WorldMatrix = XMMatrixTranslation(m_PosX, m_PosY, 0.0f);
}

void BallClass::Render(ID3D11DeviceContext* context)
{
    m_Model->Render(context);
}

XMMATRIX BallClass::GetWorldMatrix()
{
    return m_WorldMatrix;
}

ModelClass* BallClass::GetModel()
{
    return m_Model;
}

float BallClass::GetPosX() {
    return m_PosX;
}

float BallClass::GetPosY() {
    return m_PosY;
}

float BallClass::GetRadius() {
    return m_Radius;
}

void BallClass::BounceX() {
    m_VelX = -m_VelX;
}

void BallClass::BounceY() {
    m_VelY = -m_VelY;
}

void BallClass::BounceYRight() {
    m_VelY = -m_VelY;
    m_VelX = fabsf(m_Speed);
}

void BallClass::BounceYLeft() {
    m_VelY = -m_VelY;
    m_VelX = -fabsf(m_Speed);
}

void BallClass::SetPosition(float x, float y) {
    m_PosX = x;
    m_PosY = y;
}

void BallClass::ResetVel() {
    m_VelX = 0;
    m_VelY = fabs(m_VelY);
}