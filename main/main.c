#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "npk_sensor.h"
#include <stdio.h>
#include <string.h>

static const char* TAG = "DEBUG_APP";
static bool s_soil_sensor_initialized = false;

// 业务数据读取封装
esp_err_t tool_get_data_soil_execute(const char* input_json, char* output, size_t output_size)
{
    ESP_LOGI(TAG, "Executing soil data retrieval...");
    if (!s_soil_sensor_initialized) {
        snprintf(output, output_size, "Error: Sensor not initialized.");
        return ESP_FAIL;
    }

    npk_sensor_data_t sensor_data;
    esp_err_t ret = npk_sensor_read(&sensor_data);

    if (ret != ESP_OK || !sensor_data.valid) {
        snprintf(output, output_size, "Error: Sensor read failed or CRC timeout. Check connection.");
        return ESP_FAIL;
    }

    snprintf(output, output_size,
        "{"
        "\"temperature_celsius\": %.1f, "
        "\"humidity_percent\": %.1f, "
        "\"ec_us_cm\": %d, "
        "\"nitrogen_mg_kg\": %d, "
        "\"phosphorus_mg_kg\": %d, "
        "\"potassium_mg_kg\": %d"
        "}",
        sensor_data.temperature, sensor_data.humidity,
        sensor_data.ec, sensor_data.nitrogen,
        sensor_data.phosphorus, sensor_data.potassium);

    ESP_LOGI(TAG, "Data successfully retrieved.");
    return ESP_OK;
}

void app_main(void)
{
    char output_buffer[512];
    npk_sensor_config_t sensor_config = {
        .uart_num = UART_NUM_2,
        .tx_io_num = GPIO_NUM_17,
        .rx_io_num = GPIO_NUM_16,
        .baud_rate = 9600,
        .sensor_addr = 0x01,
        .reg_start_addr = 0x00,
        .reg_count = 7
    };

    if (npk_sensor_init(&sensor_config) == ESP_OK) {
        s_soil_sensor_initialized = true;
    } else {
        ESP_LOGE(TAG, "Sensor UART Init Failed!");
        return;
    }
    const uint8_t reset_addr_cmd[] = {
        0x00, 0x10, 0x00, 0x80, 0x00, 0x01, 0x02, 0x00, 0x01, 0x75, 0xC0
    };

    // 清空缓存，发送改地址指令
    uart_flush_input(UART_NUM_2);
    uart_write_bytes(UART_NUM_2, (const char*)reset_addr_cmd, sizeof(reset_addr_cmd));
    uart_wait_tx_done(UART_NUM_2, pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "Sent Force-Address-01 Command to register 0x0080");

    // 给传感器一点时间写入EEPROM并生效
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1) {
        ESP_LOGI(TAG, "正在调用工具函数读取数据...");
        memset(output_buffer, 0, sizeof(output_buffer));

        esp_err_t ret = tool_get_data_soil_execute("{}", output_buffer, sizeof(output_buffer));

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "工具调用成功！");
            printf("\n[JSON 输出]: %s\n\n", output_buffer);
        } else {
            ESP_LOGE(TAG, "工具调用失败，错误信息: %s", output_buffer);
        }
        ESP_LOGI(TAG, "等待 5 秒后进行下一次读取...\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}