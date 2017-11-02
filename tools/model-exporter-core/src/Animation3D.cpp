#include "Animation3D.h"


NodeAnimation3D::NodeAnimation3D()
{
    //ctor
}

NodeAnimation3D::~NodeAnimation3D()
{
    //dtor
}


Animation3D::Animation3D()
:TicksPerSecond(1.0f),
 Duration(0.0f),
 Name("loaded")
{
    //ctor
}

Animation3D::~Animation3D()
{
    //dtor
}

bool Animation3D::HasAnimations()
{
    bool flag = false;
    for( unsigned int i=0; i < AnimNodesList.size(); i++ )
    {
        if( ( AnimNodesList[i].Rotations.size() + AnimNodesList[i].Positions.size() + AnimNodesList[i].Scales.size() ) > 0 )
        {
            flag = true;
        }
    }
    return flag;
}
