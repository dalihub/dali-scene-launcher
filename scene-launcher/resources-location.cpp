/**
 * This file defines the location dali-scene-launcher resources.
 * It is used for builds where an application resource directory is NOT explicity defined.
 */

#include <cstdlib>

int dali_demo_RESOURCE_DIR = setenv("DALI_APPLICATION_PACKAGE",  "/data/git_dali_3dformat/dali-env/opt/share/com.samsung.dali-scene-launcher/res" , 1);
