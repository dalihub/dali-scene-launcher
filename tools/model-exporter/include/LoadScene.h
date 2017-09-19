#ifndef LOADSCENE_H
#define LOADSCENE_H

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

#include "Scene3D.h"

void GetSceneNodes(Scene3D &scene_data, Node3D *parent, const aiScene *scene, aiNode *aNode);
void GetSceneCameras( Scene3D &scene_data, const aiScene *scene );
void GetAnimations( Scene3D &scene_data, const aiScene *scene );

#endif // LOADSCENE_H
