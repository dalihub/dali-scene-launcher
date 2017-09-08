#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

struct Vector3
{
        Vector3()
        :x(0.0f),
         y(0.0f),
         z(0.0f)
         {

         }
        void normalize()
        {
            float temp = sqrtf( x*x + y*y + z*z );
            x /= temp;
            y /= temp;
            y /= temp;
        }
        float x;
        float y;
        float z;
};

#endif // VECTOR3_H
