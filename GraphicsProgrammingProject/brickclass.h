#ifndef _BRICKCLASS_H_
#define _BRICKCLASS_H_

#include "modelclass.h"
#include <DirectXMath.h>
using namespace DirectX;

class BrickClass
{
public:
    BrickClass();
    ~BrickClass();

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* texture, float x, float y);
    void Shutdown();
    void Render(ID3D11DeviceContext* context);

    void SetActive(bool active);
    void SetPosition(float x, float y);
    bool IsActive() const;

    XMMATRIX GetWorldMatrix();
    ModelClass* GetModel();

    float GetX() const;
    float GetY() const;
    float GetHalfWidth() const;
    float GetHalfHeight() const;

private:
    void UpdateWorldMatrix();

private:
    ModelClass* m_Model;
    XMMATRIX m_WorldMatrix;

    float m_PosX, m_PosY;
    float halfWidth, halfHeight;
    bool isActive;
};

#endif