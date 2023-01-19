#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "utils.h"

char* accessToken()
{
    char* token = NULL;
    char* device_code = NULL;
    char* user_code = NULL;
    char* message = NULL;
    int expires_in = -1;

    cJSON* i = NULL;
    cJSON* responseJson = NULL;
    char checkTokenUrl[] = "https://login.microsoftonline.com/consumers/oauth2/v2.0/token";
    char checkGetDeviceCodeUrl[] = "https://login.microsoftonline.com/consumers/oauth2/v2.0/devicecode";

    http_Response response = http_Post(checkGetDeviceCodeUrl, "client_id=15ee6508-cdb3-44cc-a961-b1ddd46d0724&scope=XboxLive.signin", "Content-Type: application/x-www-form-urlencoded");
    responseJson = cJSON_Parse(response.data);

    if (responseJson)
    {
        i = cJSON_GetObjectItemCaseSensitive(responseJson, "device_code");
        device_code = i->valuestring;

        i = cJSON_GetObjectItemCaseSensitive(responseJson, "user_code");
        user_code = i->valuestring;

        i = cJSON_GetObjectItemCaseSensitive(responseJson, "message");
        message= i->valuestring;

        i = cJSON_GetObjectItemCaseSensitive(responseJson, "expires_in");
        expires_in = i->valueint;
    }

    if (user_code == NULL && device_code == NULL)
        return NULL;

    char* tmp_payload = "grant_type=urn:ietf:params:oauth:grant-type:device_code&client_id=15ee6508-cdb3-44cc-a961-b1ddd46d0724&device_code="; // FAUT AJOUTER ICI
    char* payload = malloc(sizeof(char) * (strlen(tmp_payload) + strlen(device_code) + 1));
    strcpy(payload, tmp_payload);
    strcat(payload, device_code);

    printf("%s\n", message);
    http_FreeResponse(response);

    do 
    {
        msleep(expires_in);
        response = http_Post(checkTokenUrl, payload, "Content-Type: application/x-www-form-urlencoded");
        responseJson = cJSON_Parse(response.data);
        i = cJSON_GetObjectItemCaseSensitive(responseJson, "access_token");
    } 
    while (!i);

    http_FreeResponse(response);
    token = i->valuestring;
    return token;
}

char* xblToken(char* accessToken)
{
    char* token = NULL;
    char* value_token = malloc(sizeof(char) * (strlen(accessToken) + 3));
    strcpy(value_token, "d=");
    strcat(value_token, accessToken);

    cJSON* payload = cJSON_CreateObject();

    cJSON* tmp_i = NULL;
    cJSON* tmp = cJSON_CreateString("http://auth.xboxlive.com");
    cJSON_AddItemToObject(payload, "RelyingParty", tmp);

    tmp = cJSON_CreateString("JWT");
    cJSON_AddItemToObject(payload, "TokenType", tmp);

    tmp = cJSON_CreateObject();

    tmp_i = cJSON_CreateString("RPS");
    cJSON_AddItemToObject(tmp, "AuthMethod", tmp_i);

    tmp_i = cJSON_CreateString("user.auth.xboxlive.com");
    cJSON_AddItemToObject(tmp, "SiteName", tmp_i);

    tmp_i = cJSON_CreateString(value_token);
    cJSON_AddItemToObject(tmp, "RpsTicket", tmp_i);

    cJSON_AddItemToObject(payload, "Properties", tmp);

    char* data = cJSON_Print(payload);
    http_Response response = http_Post("https://user.auth.xboxlive.com/user/authenticate", data, "Content-Type: application/json");
    cJSON* responseJson = cJSON_Parse(response.data);
    tmp = cJSON_GetObjectItemCaseSensitive(responseJson, "Token");
    if (tmp)
        token = tmp->valuestring;
    
    free(value_token);
    return token;
}

void xstsToken(char* xbToken, char** xstsToken, char** uhs)
{
    cJSON* tmp_i = NULL;

    cJSON* payload = cJSON_CreateObject();

    cJSON* tmp = cJSON_CreateString("rp://api.minecraftservices.com/");
    cJSON_AddItemToObject(payload, "RelyingParty", tmp);

    tmp = cJSON_CreateString("JWT");
    cJSON_AddItemToObject(payload, "TokenType", tmp);

    tmp = cJSON_CreateObject();

    tmp_i = cJSON_CreateString("RETAIL");
    cJSON_AddItemToObject(tmp, "SandboxId", tmp_i);

    tmp_i = cJSON_CreateArray();
    cJSON_AddItemToArray(tmp_i, cJSON_CreateString(xbToken));
    cJSON_AddItemToObject(tmp, "UserTokens", tmp_i);
    cJSON_AddItemToObject(payload, "Properties", tmp);

    http_Response response = http_Post("https://xsts.auth.xboxlive.com/xsts/authorize", cJSON_Print(payload), "Content-Type: application/json");
    cJSON* responseJson = cJSON_Parse(response.data);
    tmp = cJSON_GetObjectItemCaseSensitive(responseJson, "Token");
    *xstsToken = tmp->valuestring;
    tmp = cJSON_GetObjectItemCaseSensitive(responseJson, "DisplayClaims");
    tmp = cJSON_GetObjectItemCaseSensitive(tmp, "xui");
    tmp = cJSON_GetArrayItem(tmp, 0); 
    tmp = cJSON_GetObjectItemCaseSensitive(tmp, "uhs");
    *uhs = tmp->valuestring;
}

char* mcToken(char* xstsToken, char* uhs)
{
    cJSON* responseJson = NULL;
    cJSON* tmp = NULL;

    size_t len_identityToken = (strlen(xstsToken) + strlen(uhs) + 11);
    char* identityToken = malloc(sizeof(char) * len_identityToken);
    snprintf(identityToken, len_identityToken, "XBL3.0 x=%s;%s", uhs, xstsToken);
    cJSON* payload = cJSON_CreateObject();

    tmp = cJSON_CreateString(identityToken);
    cJSON_AddItemToObject(payload, "identityToken", tmp);

    http_Response response = http_Post("https://api.minecraftservices.com/authentication/login_with_xbox", cJSON_Print(payload), "Content-Type: application/json");
    responseJson = cJSON_Parse(response.data);
    tmp = cJSON_GetObjectItemCaseSensitive(responseJson, "access_token");

    return tmp->valuestring;
}

char* mc_GetUUID(char* username)
{
    char* url = malloc(sizeof(char) * (strlen(username) + 49));
    strcpy(url, "https://api.mojang.com/users/profiles/minecraft/");
    strcat(url, username);
    http_Response response = http_Get(url); 
    cJSON* responseJson = cJSON_Parse(response.data);
    cJSON* tmp = cJSON_GetObjectItemCaseSensitive(responseJson, "id");
    username = tmp->valuestring;
    cJSON_free(responseJson);
    return username;
}
