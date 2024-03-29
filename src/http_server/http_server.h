#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>

#define TEST_SERVER_URL "http://t.zhuagewawa.com/admin/room/register.pusher"
#define OFFI_SERVER_URL "http://r.zhuagewawa.com/admin/room/register.pusher"

enum CONTROLLER_COVER_STATE {
    COVER_OFF   = 0x00,
    COVER_ON    = 0x01,
    COVER_NULL  = 0x0F,
};

#ifdef __cplusplus
extern "C" {
#endif

void* http_server(void* arg);

void error_die(const char* sc);
int startup(uint16_t* port);
int accept_request(int client);
// void* accept_request(void* pclient);

void thread_accept_request(void *param);
void try_accept_request(int socket);

#ifdef __cplusplus
}
#endif

#endif