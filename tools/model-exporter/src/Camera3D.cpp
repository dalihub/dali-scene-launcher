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

void Camera3D::MultLookAtMatrix( Vector3 look_atVector, Vector3 upVector, Vector3 positionVector )
{
    Vector3 vZ = look_atVector; // f
    vZ.normalize();
    Vector3 vX; // s = f cross UP
    vX.x = ( vZ.y * upVector.z ) - ( vZ.z * upVector.y );
    vX.y = ( vZ.z * upVector.x ) - ( vZ.x * upVector.z );
    vX.z = ( vZ.x * upVector.y ) - ( vZ.y * upVector.x );
    vX.normalize(); // s
    Vector3 vY; // u = s cross f
    vY.x = ( vX.y * vZ.z ) - ( vX.z * vZ.y );
    vY.y = ( vX.z * vZ.x ) - ( vX.x * vZ.z );
    vY.z = ( vX.x * vZ.y ) - ( vX.y * vZ.x );
    vY.normalize();

    float lookMat[16];
    float camMatrix[16];

    lookMat[0] = vX.x;
    lookMat[4] = vX.y;
    lookMat[8] = vX.z;

    lookMat[1] = vY.x;
    lookMat[5] = vY.y;
    lookMat[9] = vY.z;

    lookMat[2] =  -vZ.x;
    lookMat[6] =  -vZ.y;
    lookMat[10] = -vZ.z;

    lookMat[3] = 0.0f;
    lookMat[7] = 0.0f;
    lookMat[11] = 0.0f;

    lookMat[12] = positionVector.x;
    lookMat[13] = positionVector.y;
    lookMat[14] = positionVector.z;
    lookMat[15] = 1.0f;


    memcpy( camMatrix, m_Matrix, sizeof(float) * 16 );

    m_Matrix[ 0 ] = ( camMatrix[0] * lookMat[0] ) + ( camMatrix[4] * lookMat[1] ) + ( camMatrix[8] * lookMat[2] ) + ( camMatrix[12] * lookMat[3] );
    m_Matrix[ 1 ] = ( camMatrix[1] * lookMat[0] ) + ( camMatrix[5] * lookMat[1] ) + ( camMatrix[9] * lookMat[2] ) + ( camMatrix[13] * lookMat[3] );
    m_Matrix[ 2 ] = ( camMatrix[2] * lookMat[0] ) + ( camMatrix[6] * lookMat[1] ) + ( camMatrix[10] * lookMat[2] ) + ( camMatrix[14] * lookMat[3] );
    m_Matrix[ 3 ] = ( camMatrix[3] * lookMat[0] ) + ( camMatrix[7] * lookMat[1] ) + ( camMatrix[11] * lookMat[2] ) + ( camMatrix[15] * lookMat[3] );

    m_Matrix[ 4 ] = ( camMatrix[0] * lookMat[4] ) + ( camMatrix[4] * lookMat[5] ) + ( camMatrix[8] * lookMat[6] ) + ( camMatrix[12] * lookMat[7] );
    m_Matrix[ 5 ] = ( camMatrix[1] * lookMat[4] ) + ( camMatrix[5] * lookMat[5] ) + ( camMatrix[9] * lookMat[6] ) + ( camMatrix[13] * lookMat[7] );
    m_Matrix[ 6 ] = ( camMatrix[2] * lookMat[4] ) + ( camMatrix[6] * lookMat[5] ) + ( camMatrix[10] * lookMat[6] ) + ( camMatrix[14] * lookMat[7] );
    m_Matrix[ 7 ] = ( camMatrix[3] * lookMat[4] ) + ( camMatrix[7] * lookMat[5] ) + ( camMatrix[11] * lookMat[6] ) + ( camMatrix[15] * lookMat[7] );

    m_Matrix[ 8 ] = ( camMatrix[0] * lookMat[8] ) + ( camMatrix[4] * lookMat[9] ) + ( camMatrix[8] * lookMat[10] ) + ( camMatrix[12] * lookMat[11] );
    m_Matrix[ 9 ] = ( camMatrix[1] * lookMat[8] ) + ( camMatrix[5] * lookMat[9] ) + ( camMatrix[9] * lookMat[10] ) + ( camMatrix[13] * lookMat[11] );
    m_Matrix[ 10 ] = ( camMatrix[2] * lookMat[8] ) + ( camMatrix[6] * lookMat[9] ) + ( camMatrix[10] * lookMat[10] ) + ( camMatrix[14] * lookMat[11] );
    m_Matrix[ 11 ] = ( camMatrix[3] * lookMat[8] ) + ( camMatrix[7] * lookMat[9] ) + ( camMatrix[11] * lookMat[10] ) + ( camMatrix[15] * lookMat[11] );

    m_Matrix[ 12 ] = ( camMatrix[0] * lookMat[12] ) + ( camMatrix[4] * lookMat[13] ) + ( camMatrix[8] * lookMat[14] ) + ( camMatrix[12] * lookMat[15] );
    m_Matrix[ 13 ] = ( camMatrix[1] * lookMat[12] ) + ( camMatrix[5] * lookMat[13] ) + ( camMatrix[9] * lookMat[14] ) + ( camMatrix[13] * lookMat[15] );
    m_Matrix[ 14 ] = ( camMatrix[2] * lookMat[12] ) + ( camMatrix[6] * lookMat[13] ) + ( camMatrix[10] * lookMat[14] ) + ( camMatrix[14] * lookMat[15] );
    m_Matrix[ 15 ] = ( camMatrix[3] * lookMat[12] ) + ( camMatrix[7] * lookMat[13] ) + ( camMatrix[11] * lookMat[14] ) + ( camMatrix[15] * lookMat[15] );


}
