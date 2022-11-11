#include <stdio.h>

#include "src/cjson/cJSON.h"
#include "src/minecraft/versions.h"
#include "src/minecraft/client.h"
#include "src/utils.h"

int main()
{
    cJSON* mainManifest = mc_GetMainManifest("versions");
    cJSON* manifest = mc_GetManifest(mainManifest, "versions", "1.19");
    mc_DownloadClient(manifest, "versions", "1.19");

    return 0;
}
