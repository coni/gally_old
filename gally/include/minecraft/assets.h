#include "cjson/cJSON.h"

char* mc_GetAssetIndex(cJSON *manifest);
cJSON * mc_GetAssetsManifest(cJSON *manifest, char *path);
int mc_DownloadAssets(cJSON *manifest, char * assetsPath);
