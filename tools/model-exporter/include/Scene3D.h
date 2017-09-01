#ifndef SCENE3D_H
#define SCENE3D_H

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

#include "Node3D.h"
#include <vector>

using namespace std;

class Scene3D
{
    public:
        Scene3D();
        void AddNode(Node3D *enode);
        virtual ~Scene3D();
        unsigned int GetNumNodes();
        Node3D *GetNode(unsigned int idx);
    protected:

    private:
        unsigned int index;
        vector<Node3D*> m_nodes;
};

#endif // SCENE3D_H
