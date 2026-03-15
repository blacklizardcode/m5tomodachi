#include "esp_log.h"
#include "esp_http_server.h"


esp_err_t download_get_handler(httpd_req_t *req) {
    FILE *f = fopen("/filesystem/character.txt", "rb");
    if (!f) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "character file not found");
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "text/plain");

    char buffer[512];
    size_t read_bytes = fread(buffer, 1, sizeof(buffer), f);
    fclose(f);

    if (read_bytes == 0) {
        httpd_resp_send(req, "", 0);
        return ESP_OK;
    }

    httpd_resp_send(req, buffer, read_bytes);
    return ESP_OK;
}

esp_err_t upload_post_handler(httpd_req_t *req) {
    FILE *f = fopen("/filesystem/character.txt", "wb");
    if (!f) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char buf[1024];
    int received;
    while ((received = httpd_req_recv(req, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, received, f);
    }

    fclose(f);
    httpd_resp_sendstr(req, "File uploaded successfully");
    return ESP_OK;
}

httpd_uri_t download_uri = {
    .uri = "/character",
    .method = HTTP_GET,
    .handler = download_get_handler,
    .user_ctx = NULL
};

httpd_uri_t upload_uri = {
    .uri = "/character",
    .method = HTTP_POST,
    .handler = upload_post_handler,
    .user_ctx = NULL
};

void webserver_task(void *pvParameters) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI("WEBSERVER", "Server started");
        httpd_register_uri_handler(server, &download_uri);
        httpd_register_uri_handler(server, &upload_uri);
    } else {
        ESP_LOGE("WEBSERVER", "Failed to start server");
        vTaskDelete(NULL); // stop task if server fails
        return;
    }

    // Keep task alive
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1s delay
    }
}