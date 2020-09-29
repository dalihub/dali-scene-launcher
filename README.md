# dali-scene-launcher

Demonstrates scene loading and displaying functionality based on libdli and the DALi libraries.

## Prequisites

- [libdli](https://github.com/dalihub/libdli) and its dependencies (please refer to its README.md;

## Build instructions

1, build libdli;
2,

  - Windows: refer to the VS2017 solution in build/windows;
  - Linux: run build/tizen/build.sh;
  - Tizen:

    $ gbs build -A ${target_arch}

    - for debug, add: --define "%enable_debug 1"
    - for SMACK-enabled targets, add: --define "%enable_dali_smack_rules 1"

3, **NOTE** (Windows): copy the _release_ pthreadVC3.dll from windows-dependencies, to your build directory (build/windows/$(Configuration)). The one built by VCPKG causes scene-launcher to fail to start correctly (0xc000007b). This needs to be investigated.

## Miscellany

- scene-launcher expects its models in ${prefix}/com.samsung.scene-launcher/models;
- you may start scene-launcher with the name of the model you want to display (i.e. file name without ".dli"):

  $ scene-launcher bottle

- F5 reloads the scene;
- Esc quits scene launcher;
