#ifndef REQUESTS_H
#define REQUESTS_H


#include "lwip/apps/http_client.h"


//#include "lwip/dns.h"



extern const char *post_data;

typedef struct TLS_CLIENT_T_
{
    struct altcp_pcb *pcb;
    bool complete;
} TLS_CLIENT_T;

static struct altcp_tls_config *tls_config = NULL;


// Replace this with the actual CA certificate in PEM format
extern const char *ca_cert_pem;

// Function to make an HTTPS POST request
void https_get_request();


// Dummy callbacks / boiler plate code
static void result_fn(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err);
static err_t recv_fn(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t headers_done_fn(httpc_state_t *connection, void *arg, struct pbuf *hdr, u16_t hdr_len, u32_t content_len);


	

#endif // REQUESTS_H
