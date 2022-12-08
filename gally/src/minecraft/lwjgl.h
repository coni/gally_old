#include "cJSON/cJSON.h"
#include <curl/curl.h>

cJSON * mc_GetBaseLwjglManifest(char* path);
char* mc_DownloadLwjgl(char* lwjglVersion, char* path);
