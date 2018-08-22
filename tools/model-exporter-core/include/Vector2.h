#ifndef VECTOR2_H
#define VECTOR2_H

/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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

struct Vector2
{
        Vector2()
        :x(0.0f),
         y(0.0f)
         {

         }

        union
        {
            struct
            {
                float x;
                float y;
            };
            float data[2];
        };
};

#endif // VECTOR2_H
