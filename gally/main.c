#include "src/cjson/cJSON.h"
#include "src/minecraft/versions.h"
#include "src/utils.h"

int main()
{
    cJSON *versionsManifest = mc_GetMainManifest("versions/");

    //MinecraftManifest_get_version_manifest()

    return 0;
}