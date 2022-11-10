#include "src/cjson/cJSON.h"
#include "src/minecraft/versions.h"
#include "src/utils.h"

int main()
{
    cJSON *mainManifest = mc_GetMainManifest("versions/");
    mc_GetManifest(&mainManifest, "versions", "1.19");
    //MinecraftManifest_get_version_manifest()

    return 0;
}
