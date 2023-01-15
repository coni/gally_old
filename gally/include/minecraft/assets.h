#include "cjson/cJSON.h"
#include "launcher.h"

char* mc_GetAssetIndex(cJSON *manifest);
cJSON * mc_GetAssetsManifest(cJSON *manifest, GamePath gamePath);
int mc_DownloadAssets(cJSON *manifest, GamePath gamePath);
int mc_GetAssetsSize(cJSON *manifest);
int mc_GetAssetsSizeVersion(char* version, GamePath gamePath);
