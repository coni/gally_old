#include "cjson/cJSON.h"
#include <curl/curl.h>

int compareLwjglVersion(char* new, char* old);
char** mc_DownloadLibraries(cJSON *manifest, char *path);
char* mc_GetLwjglVersion(cJSON *manifest);
