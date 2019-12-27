/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "http2_upload_api.h"
#include "wrappers.h"


#define HTTP2_ONLINE_SERVER_URL       "a1wOVhf0PmQ.iot-as-http2.cn-shanghai.aliyuncs.com"
#define HTTP2_ONLINE_SERVER_PORT      443
#define HTTP2_PRODUCT_KEY             "a1wOVhf0PmQ"
#define HTTP2_DEVICE_NAME             "T10YogSxts4YVtugH5at"
#define HTTP2_DEVICE_SECRET           "9pl920NeLA4D7vMgnGW8koCN8TaScglw"


#define EXAMPLE_TRACE(fmt, ...)                        \
    do {                                               \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__);                \
        HAL_Printf("%s", "\r\n");                      \
    } while (0)

static int upload_end = 0;
static char g_upload_id[50] = {0};

void upload_file_result(const char *file_path, int result, void *user_data)
{
    upload_end++;
    EXAMPLE_TRACE("=========== file_path = %s, result = %d, finish num = %d ===========", file_path, result, upload_end);
}

void upload_id_received_handle(const char *file_path, const char *upload_id, void *user_data)
{
    EXAMPLE_TRACE("=========== file_path = %s, upload_id = %s ===========", file_path, upload_id);

    if (upload_id != NULL) {
        memset(g_upload_id, 0, sizeof(g_upload_id));
        strncpy(g_upload_id, upload_id, sizeof(g_upload_id) - 1);
    }
}

static int http2_stream_test(char **argv, int argc)
{
    int ret;
    http2_upload_conn_info_t conn_info;
    http2_upload_result_cb_t result_cb;
    void *handle;
    int goal_num = 0;
    int i;

    memset(&conn_info, 0, sizeof(http2_upload_conn_info_t));
    conn_info.product_key = HTTP2_PRODUCT_KEY;
    conn_info.device_name = HTTP2_DEVICE_NAME;
    conn_info.device_secret = HTTP2_DEVICE_SECRET;
    conn_info.url = HTTP2_ONLINE_SERVER_URL;
    conn_info.port = HTTP2_ONLINE_SERVER_PORT;

    memset(&result_cb, 0, sizeof(http2_upload_result_cb_t));
    result_cb.upload_completed_cb = upload_file_result;
    result_cb.upload_id_received_cb = upload_id_received_handle;

    handle = IOT_HTTP2_UploadFile_Connect(&conn_info, NULL);
    if (handle == NULL) {
        return -1;
    }

    for (i = 1; i < argc; i++) {
        http2_upload_params_t fs_params;
        memset(&fs_params, 0, sizeof(fs_params));
        fs_params.file_path = argv[i];
        fs_params.upload_len = 1024 * 1024 * 100;
        fs_params.opt_bit_map = UPLOAD_FILE_OPT_BIT_OVERWRITE;

        ret = IOT_HTTP2_UploadFile_Request(handle, &fs_params, &result_cb, NULL);
        if (ret == 0) {
            goal_num++;
        }
    }
    while (upload_end != goal_num) {
        HAL_SleepMs(200);
    }

    ret = IOT_HTTP2_UploadFile_Disconnect(handle);
    EXAMPLE_TRACE("close connect %d\n", ret);
    return 0;
}

#if defined(__MBED__)

/* Stringize */
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

/* Patch to run this example
 *
 * 1. Meet stack overflow with the thread 'https_io'. The configured stack size
 *    is 6KiB, but actually it uses up to 18KiB stack. Need to modify device SDK
 *    manually for changing this stack size.
 * 2. Need 128KiB heap. So most platforms cannot run except NUC472 with 1MiB external
 *    SRAM.
 * 3. Need to set up filesystem to upload file test.
 */
#endif

int main(int argc, char **argv)
{
    int ret;

#if defined(__MBED__)
    /* Create one file on the fly for http2 upload test */
    static const char *argv_[] = {
        "/bin/http2_example_uploadfile",                                        // Dummy
        "/" STR(MBED_CONF_USER_FILESYSTEM_MOUNT_POINT) "/upload_data_01.bin"    // Create on the fly
    };
    argv = argv_;
    argc = 1 + sizeof(argv_)/sizeof(argv_[0]);

    FILE *fh = fopen(argv[1], "wb");
    if (fh == NULL) {
        HAL_Printf("fopen(%s, \"wb\") failed", argv[1]);
        return -1;
    }

    const char upload_data_01[] = "Upload by http2_example_uploadfile";
    size_t n = fwrite(upload_data_01, 1, sizeof(upload_data_01) - 1, fh);
    if (n != (sizeof(upload_data_01) - 1)) {
        HAL_Printf("fwrite(...) failed with %d", n);
        fclose(fh);
        return -1;
    }

    fclose(fh);
#endif

    if (argc < 2) {
        HAL_Printf("no file name input!\n");
        return 0;
    }

    ret = http2_stream_test(argv, argc);
    return ret;
}
