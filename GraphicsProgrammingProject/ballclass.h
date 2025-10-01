#ifndef _BALLCLASS_H_
#define _BALLCLASS_H_

#include <DirectXMath.h>
using namespace DirectX;

#include "modelclass.h"

class BallClass
{
public:

    BallClass();
    ~BallClass();

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* texture);
    void Shutdown();

    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* context);

    XMMATRIX GetWorldMatrix();
    ModelClass* GetModel();
    float GetPosX();
    float GetPosY();
    float GetRadius();
    void BounceX();
    void BounceY();
    void BounceYRight();
    void BounceYLeft();
    void SetPosition(float x, float y);
    void ResetVel();
    void UpdateWorldMatrix();

private:

    ModelClass* m_Model;

    float m_PosX, m_PosY;
    float m_VelX, m_VelY;
    float m_Radius;
    float m_Speed;
    bool m_IsActive;
    float aspectRatio;
    float viewWidthUnits;
    float halfViewWidth;
    float viewHeightUnits;
    float halfViewHeight;

    XMMATRIX m_WorldMatrix;
};

#endif