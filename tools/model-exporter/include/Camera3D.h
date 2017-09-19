#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "Vector3.h"
#include <assimp/scene.h>

class Camera3D
{
    public:
        Camera3D();
        virtual ~Camera3D();
        float fov;
        float near;
        float far;
        float m_Matrix[16];
        void MultLookAtMatrix( Vector3 look_atVector, Vector3 upVector, Vector3 positionVector );
        void SetMatrix(aiMatrix4x4 eMat);
    protected:

    private:
};

#endif // CAMERA3D_H
