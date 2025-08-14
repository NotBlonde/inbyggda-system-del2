
#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "temp_driver.h"

#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_GPIO    4        // ändra vid behov
#define I2C_SCL_GPIO    5        // ändra vid behov
#define I2C_FREQ_HZ     100000   // 100 kHz
#define I2C_TIMEOUT_MS  1000

#define TMP102_ADDR     0x48
#define TMP102_REG_TEMP 0x00

static const char *TAG = "TMP102";

static int tmp102_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
        .clk_flags = 0
    };
    esp_err_t err = i2c_param_config(I2C_PORT, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c_param_config failed: %d", err);
        return -3;
    }
    err = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "i2c_driver_install failed: %d", err);
        return -3;
    }
    // Ping sensorn: skriv registeradress
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (TMP102_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, TMP102_REG_TEMP, true);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(I2C_PORT, cmd, I2C_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (err == ESP_ERR_TIMEOUT) return -1;
    if (err == ESP_FAIL) return -2; // NACK
    if (err != ESP_OK) return -3;
    return 0;
}

static int tmp102_read_raw(int16_t *raw) {
    if (!raw) return -3;

    esp_err_t err;
    // Skriv registeradress
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (TMP102_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, TMP102_REG_TEMP, true);
    // Repeated START + läs 2 bytes
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (TMP102_ADDR << 1) | I2C_MASTER_READ, true);
    uint8_t data[2] = {0};
    i2c_master_read(cmd, data, 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &data[1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(I2C_PORT, cmd, I2C_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (err == ESP_ERR_TIMEOUT) return -1;
    if (err == ESP_FAIL) return -2; // NACK / bus error
    if (err != ESP_OK) return -3;

    // Packa 12-bit tvåkomplement
    int16_t value = (int16_t)((data[0] << 8) | data[1]);
    value >>= 4; // 12-bit värde i bit[11:0]

    // Signförläng om negativt (bit 11 är sign)
    if (value & 0x0800) {
        value |= 0xF000;
    }
    *raw = value;
    return 0;
}

static float tmp102_raw_to_celsius(int16_t raw) {
    return (float)raw * 0.0625f;
}

temp_sensor_t TMP102 = {
    .init = tmp102_init,
    .read_raw = tmp102_read_raw,
    .raw_to_celsius = tmp102_raw_to_celsius
};
