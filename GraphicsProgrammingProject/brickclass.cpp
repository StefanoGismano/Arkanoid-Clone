#include "brickclass.h"

BrickClass::BrickClass() {
    m_Model = 0;
    m_PosX = 0;
    m_PosY = 0;
    halfWidth = 0.3f;
    halfHeight = 0.1f;
    isActive = true;
}

BrickClass::~BrickClass()
{
}

bool BrickClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* texture, float x, float y)
{
    m_Model = new ModelClass;
    m_PosX = x;
    m_PosY = y;

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

void BrickClass::Shutdown()
{
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }
}

void BrickClass::Render(ID3D11DeviceContext* context)
{
    if (isActive)
        m_Model->Render(context);
}

void BrickClass::UpdateWorldMatrix()
{
    m_WorldMatrix = XMMatrixTranslation(m_PosX, m_PosY, 0.0f);
}

XMMATRIX BrickClass::GetWorldMatrix()
{
    return m_WorldMatrix;
}

ModelClass* BrickClass::GetModel()
{
    return m_Model;
}

void BrickClass::SetActive(bool active)
{
    isActive = active;
}

bool BrickClass::IsActive() const
{
    return isActive;
}

float BrickClass::GetX() const { return m_PosX; }
float BrickClass::GetY() const { return m_PosY; }
float BrickClass::GetHalfWidth() const { return halfWidth; }
float BrickClass::GetHalfHeight() const { return halfHeight; }

void BrickClass::SetPosition(float x, float y) {
    m_PosX = x;
    m_PosY = y;
    UpdateWorldMatrix();
}