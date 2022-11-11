#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char* mc_DownloadLibraries(cJSON **manifest, char *path);
char* mc_GetLwjglVersion(cJSON *manifest);
