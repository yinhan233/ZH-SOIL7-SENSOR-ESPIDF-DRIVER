#pragma once

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 传感器数据结果结构体
typedef struct {
    float temperature;
    float humidity;
    uint16_t ec;
    uint16_t nitrogen;
    uint16_t phosphorus;
    uint16_t potassium;
    bool valid;
} npk_sensor_data_t;

// 传感器配置结构体
typedef struct {
    uart_port_t uart_num; // UART端口号
    gpio_num_t tx_io_num; // TX引脚
    gpio_num_t rx_io_num; // RX引脚
    gpio_num_t rs485_ctrl_io; // RS485控制引脚
    uint32_t baud_rate; // 波特率
    uint8_t sensor_addr; // 传感器 Modbus 地址
    uint8_t reg_start_addr; // 起始寄存器地址
    uint8_t reg_count; // 读取寄存器数量
} npk_sensor_config_t;

/**
 * @brief 初始化 NPK 传感器驱动
 *
 * @param config 传感器配置结构体指针
 * @return esp_err_t
 */
esp_err_t npk_sensor_init(const npk_sensor_config_t* config);

/**
 * @brief 读取传感器数据
 *
 * @param data 用于存储读取到的数据的结构体指针
 * @return esp_err_t
 *         - ESP_OK: 读取成功
 *         - ESP_FAIL: 读取失败 (CRC错误、超时或数据长度不对)
 *         - ESP_ERR_INVALID_ARG: 参数错误
 */
esp_err_t npk_sensor_read(npk_sensor_data_t* data);

#ifdef __cplusplus
}
#endif
