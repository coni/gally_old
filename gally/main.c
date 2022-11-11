#include <stdio.h>

#include "src/minecraft/versions.h"
#include "src/minecraft/client.h"
#include "src/minecraft/mainclass.h"
#include "src/minecraft/classpath.h"

#include "src/utils.h"
#include "src/cjson/cJSON.h"

int main()
{
    cJSON* mainManifest = mc_GetMainManifest("versions");
    cJSON* manifest = mc_GetManifest(mainManifest, "versions", "1.19");
    mc_DownloadClient(manifest, "versions", "1.19");
    mc_DownloadLibraries(manifest, "libraries");

    return 0;
}
