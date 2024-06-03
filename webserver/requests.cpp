// Documentation : https://www.unshiu.com/posts/pico-http-client-part-i-simple-client/
// https://www.unshiu.com/posts/pico-http-client-part-iii-mbedtls/
// https://mcuapplab.blogspot.com/2023/01/raspberry-pi-pico-w-c-sdk-lwip-ep-2.html?m=1
// implementation connects mbedtls to lwip
// class iplements two functons,
// (1) http unsecure connections and api calls
// To DO:
// (1) https secured connections via mbedtls
// (2) body received funtions

#include "requests.h"

const char *post_data = "{\"model\": \"text-davinci-003\", \"prompt\": \"Say hello world\", \"max_tokens\": 5}";

// Replace this with the actual CA certificate in PEM format
const char *ca_cert_pem =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEbmhR... (rest of the certificate)\n"
    "-----END CERTIFICATE-----\n";

// handles requests finished
static err_t headers_done_fn(httpc_state_t *connection, void *arg,
                             struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("in headers_done_fn\n");
    return ERR_OK;
}

static void result_fn(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf(">>> result_fn >>>\n");
    printf("httpc_result: %s\n",
           httpc_result == HTTPC_RESULT_OK                ? "HTTPC_RESULT_OK"
           : httpc_result == HTTPC_RESULT_ERR_UNKNOWN     ? "HTTPC_RESULT_ERR_UNKNOWN"
           : httpc_result == HTTPC_RESULT_ERR_CONNECT     ? "HTTPC_RESULT_ERR_CONNECT"
           : httpc_result == HTTPC_RESULT_ERR_HOSTNAME    ? "HTTPC_RESULT_ERR_HOSTNAME"
           : httpc_result == HTTPC_RESULT_ERR_CLOSED      ? "HTTPC_RESULT_ERR_CLOSED"
           : httpc_result == HTTPC_RESULT_ERR_TIMEOUT     ? "HTTPC_RESULT_ERR_TIMEOUT"
           : httpc_result == HTTPC_RESULT_ERR_SVR_RESP    ? "HTTPC_RESULT_ERR_SVR_RESP"
           : httpc_result == HTTPC_RESULT_ERR_MEM         ? "HTTPC_RESULT_ERR_MEM"
           : httpc_result == HTTPC_RESULT_LOCAL_ABORT     ? "HTTPC_RESULT_LOCAL_ABORT"
           : httpc_result == HTTPC_RESULT_ERR_CONTENT_LEN ? "HTTPC_RESULT_ERR_CONTENT_LEN"
                                                          : "*UNKNOWN*");
    printf("received %ld bytes\n", rx_content_len);
    printf("server response: %ld\n", srv_res);
    printf("err: %d\n", err);
    printf("<<< result_fn <<<\n");
}

static err_t recv_fn(void *arg, struct altcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    printf(">>> recv_fn >>>\n");
    if (p == NULL)
    {
        printf("p is NULL\n");
    }
    else
    {
        printf("p: %p\n", p);
        printf("next: %p\n", p->next);
        printf("payload: %p\n", p->payload);
        printf("len: %d\n", p->len);
    }
    printf("<<< recv_fn <<<\n");
    return ERR_OK;
}

void https_get_request(void)
{
    httpc_connection_t settings = {
        .use_proxy = 0,
        .result_fn = result_fn,
        .headers_done_fn = headers_done_fn};
    httpc_state_t *connection = NULL;

    err_t err = httpc_get_file_dns("neverssl.com", HTTP_DEFAULT_PORT, "/", &settings, recv_fn, NULL, &connection);
    printf("err = %d\n", err);
}
