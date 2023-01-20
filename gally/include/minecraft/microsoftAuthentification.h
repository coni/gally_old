#include <stdlib.h>

cJSON* accessToken();
cJSON* xblToken(char* accessToken);
cJSON* xstsToken(char* xbToken);
cJSON* minecraftToken(char* xstsToken, char* uhs);
char* mc_GetUUID(char* username);
