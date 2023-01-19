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
