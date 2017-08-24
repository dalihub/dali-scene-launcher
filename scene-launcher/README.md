
Step 1) Clone a project template

    A project template can be cloned from https://github.com/dalihub/dali-scene-template.git

        $ git clone https://github.com/dalihub/dali-scene-template.git

Step 2) Rename the project

    Rename the files
    com.samsung.dali-scene-template.manifest
    com.samsung.dali-scene-template.manifest-smack
    com.samsung.dali-scene-template.rule
    The files don't need to be updated. Just change the name.
    In the packaging folder rename the file
    com.samsung.dali-scene-template.spec
    Edit the packaging file and update the project's name.

Step 3) Copy resources

    Copy the texture assets into the folder ./resources/images/scenes/

    Copy the shader assets into the folder ./resources/shaders/scenes/

    Copy the model assets into the folder ./resources/models/scenes/

    At the present time a file with .dalipbr extension needs to be created with a file editor in the ./resources/models/scenes/ directory. It's a file with json format and describes where to find the different assets. Below is an example of the file's content.

    ///////////////////////////////////////////////
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

    {
      "models":
      [
        {
          "model":"scenes/watch_n_gon_bake.obj",
          "model-scale-factor":[ 3.5, 3.5, 3.5 ],
          "albedo-metalness":"scenes/Mat_1_Albedo_Metalness.png",
          "normal-roughness":"scenes/Mat_1_Normal_Roughness.png",
          "vertex-shader":"scenes/watchface_shader.vsh",
          "fragment-shader":"scenes/watchface_shader.fsh",
          "cubemap-specular":"scenes/papermill_pmrem.ktx",
          "cubemap-diffuse":"scenes/papermill_E_diffuse-64.ktx",
          "skybox-scale-factor":[ 4.5, 4.5, 4.5 ],
          "camera-default-fov":60.0,
          "camera-default-near":0.1,
          "camera-default-far":1000.0,
          "camera-default-position":[ 0.0, 0.0, 3.5 ]
        }
      ]
    }
    ///////////////////////////////////////////////

Step 4) Build

    To build on Desktop just build like dali-demo

        $ source setenv

        $ cd dali-scene-template/build/tizen

        $ cmake -DCMAKE_INSTALL_PREFIX=$DESKTOP_PREFIX

        $ make install

    PBR asset files are installed into the dali-env folder.



    To build for device use a gbs command line similar to the one used to build dali-demo

        $ gbs -c [path to the .gbs.conf file] build -A armv7l -P [profile] -B [path to the destination folder]

     The rpm package is generated and can be installed on target.



