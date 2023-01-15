#include "cjson/cJSON.h"
#include <curl/curl.h>
#include "launcher.h"

char* mc_DownloadJre(cJSON* manifest, GamePath gamePath);
char* mc_GetJreComponent(cJSON* manifest);
char* mc_DownloadJreComponent(char* component, GamePath gamePath);
int mc_GetJreSize(cJSON* jreManifest);
int mc_GetJreSizeVersion(char* version, GamePath gamePath);
