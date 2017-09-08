#include "Camera3D.h"

Camera3D::Camera3D()
{
    fov = 60.0f;
    near = 60.0f;
    far = 60.0f;
    m_Matrix[1] = m_Matrix[2] = m_Matrix[3] = m_Matrix[4] = m_Matrix[6] = m_Matrix[7] = m_Matrix[8] = m_Matrix[9] = m_Matrix[11] = m_Matrix[12] = m_Matrix[13] = m_Matrix[14] = 0.0f;
    m_Matrix[0] = m_Matrix[5] = m_Matrix[10] = m_Matrix[15]  = 1.0f;
}

Camera3D::~Camera3D()
{

}

void Camera3D::SetMatrix(aiMatrix4x4 eMat)
{
    m_Matrix[0] = eMat.a1;
    m_Matrix[1] = eMat.b1;
    m_Matrix[2] = eMat.c1;
    m_Matrix[3] = eMat.d1;

    m_Matrix[4] = eMat.a2;
    m_Matrix[5] = eMat.b2;
    m_Matrix[6] = eMat.c2;
    m_Matrix[7] = eMat.d2;

    m_Matrix[8] = eMat.a3;
    m_Matrix[9] = eMat.b3;
    m_Matrix[10] = eMat.c3;
    m_Matrix[11] = eMat.d3;

    m_Matrix[12] = eMat.a4;
    m_Matrix[13] = eMat.b4;
    m_Matrix[14] = eMat.c4;
    m_Matrix[15] = eMat.d4;

}

void Camera3D::SetMatrix( Vector3 look_atVector, Vector3 upVector, Vector3 positionVector )
{
    Vector3 vZ = look_atVector;
    vZ.normalize();
    Vector3 vX; // f cross UP
    vX.x = ( upVector.y * look_atVector.z ) - ( upVector.z * look_atVector.y );
    vX.y = ( upVector.z * look_atVector.x ) - ( upVector.x * look_atVector.z );
    vX.z = ( upVector.x * look_atVector.y ) - ( upVector.y * look_atVector.x );
    vX.normalize();
    Vector3 vY;
    vY.x = ( vZ.y * vX.z ) - ( vZ.z * vX.y );
    vY.y = ( vZ.z * vX.x ) - ( vZ.x * vX.z );
    vY.z = ( vZ.x * vX.y ) - ( vZ.y * vX.x );
    vY.normalize();

    m_Matrix[0] = vX.x;
    m_Matrix[4] = vX.y;
    m_Matrix[8] = vX.z;

    m_Matrix[1] = vY.x;
    m_Matrix[5] = vY.y;
    m_Matrix[9] = vY.z;

    m_Matrix[2] =  vZ.x;
    m_Matrix[6] =  vZ.y;
    m_Matrix[10] = vZ.z;

    m_Matrix[3] = 0.0f;
    m_Matrix[7] = 0.0f;
    m_Matrix[11] = 0.0f;

    m_Matrix[12] = positionVector.x;
    m_Matrix[13] = positionVector.y;
    m_Matrix[14] = positionVector.z;
    m_Matrix[15] = 1.0f;
}
