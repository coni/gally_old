#include <stdlib.h>

char* accessToken();
char* xblToken(char* accessToken);
void xstsToken(char* xbToken, char** xstsToken, char** uhs);
char* mcToken(char* xstsToken, char* uhs);
char* mc_GetUUID(char* username);
