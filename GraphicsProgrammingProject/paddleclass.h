#ifndef _PADDLECLASS_H_
#define _PADDLECLASS_H_

#include "modelclass.h"
#include <DirectXMath.h>
using namespace DirectX;

class PaddleClass
{
public:
    PaddleClass();
    ~PaddleClass();

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* texture);
    void Shutdown();
    void Update(float deltaTime, bool moveLeft, bool moveRight);
    void Render(ID3D11DeviceContext* context);

    XMMATRIX GetWorldMatrix();
    ModelClass* GetModel();

    float GetPosX();
    float GetPosY();
    float GetHalfWidth();
    float GetHalfHeight();
    float GetSpeed();
    void SetPosition(float x);
    void SetSpeed(float x);

private:
    ModelClass* m_Model;
    float m_PosX;
    float m_PosY;
    float m_Speed;
    XMMATRIX m_WorldMatrix;

    float aspectRatio;
    float viewWidthUnits;
    float halfViewWidth;
    float halfWidth;
    float halfHeight;

    void UpdateWorldMatrix();
};

#endif
