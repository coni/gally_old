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

    http_Response response = http_Post(checkGetDeviceCodeUrl, "client_id=15ee6508-cdb3-44cc-a961-b1ddd46d0724&scope=user.read");
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
        response = http_Post(checkTokenUrl, payload);
        responseJson = cJSON_Parse(response.data);
        i = cJSON_GetObjectItemCaseSensitive(responseJson, "access_token");
        response = http_Post(checkTokenUrl, payload);
    } 
    while (!i);

    http_FreeResponse(response);
    token = i->valuestring;
    return token;
}
