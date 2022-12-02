#include "cJSON/cJSON.h"
#include <curl/curl.h>

char * download_lwjgl(char * lwjglVersion, char * path, CURL * session);
cJSON * getBaseLwjglManifest(char * path, CURL * session);
