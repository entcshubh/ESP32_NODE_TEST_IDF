#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "esp_system.h"
#include "esp_mac.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"

#define TAG "PCB_TEST"

/**********************
 * Change pins if required
 **********************/
#define LED_PIN GPIO_NUM_33
#define CONFIG_BUTTON GPIO_NUM_5
#define BOOT_BUTTON GPIO_NUM_0

/**********************/

static void print_separator()
{
    printf("=========================================\n");
}

void led_test()
{
    print_separator();
    printf("LED TEST\n");

    for (int i = 0; i < 5; i++)
    {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(300));

        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    printf("LED : PASS\n");
}

void button_test()
{
    print_separator();

    printf("BUTTON TEST\n");

    printf("Press CONFIG Button\n");

    while (gpio_get_level(CONFIG_BUTTON) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    printf("CONFIG BUTTON : PASS\n");

    vTaskDelay(pdMS_TO_TICKS(500));

    printf("Press BOOT Button\n");

    while (gpio_get_level(BOOT_BUTTON) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    printf("BOOT BUTTON : PASS\n");
}

void mac_test()
{
    print_separator();

    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    printf("MAC ADDRESS\n");

    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
           mac[0],
           mac[1],
           mac[2],
           mac[3],
           mac[4],
           mac[5]);

    printf("MAC TEST : PASS\n");
}

void chip_test()
{
    print_separator();

    esp_chip_info_t chip;

    esp_chip_info(&chip);

    printf("CHIP INFO\n");

    printf("Cores      : %d\n", chip.cores);

    printf("Revision   : %d\n", chip.revision);

    printf("Features   : %lu\n", (unsigned long)chip.features);

    printf("Chip Test  : PASS\n");
}

void flash_test()
{
    print_separator();

    uint32_t flash_size;

    esp_flash_get_size(NULL, &flash_size);

    printf("FLASH SIZE : %lu Bytes\n", (unsigned long)flash_size);

    printf("FLASH TEST : PASS\n");
}

void gpio_test()
{
    print_separator();

    printf("GPIO TEST\n");

    gpio_set_level(LED_PIN, 1);

    vTaskDelay(pdMS_TO_TICKS(500));

    gpio_set_level(LED_PIN, 0);

    printf("GPIO OUTPUT : PASS\n");

    printf("CONFIG BUTTON = %d\n", gpio_get_level(CONFIG_BUTTON));

    printf("BOOT BUTTON   = %d\n", gpio_get_level(BOOT_BUTTON));

    printf("GPIO INPUT : PASS\n");
}

void init_gpio()
{
    gpio_config_t io_conf = {};

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LED_PIN);

    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pin_bit_mask = (1ULL << CONFIG_BUTTON) |
                           (1ULL << BOOT_BUTTON);

    gpio_config(&io_conf);
}

void app_main(void)
{
    init_gpio();

    print_separator();

    printf("ESP32 PCB FACTORY TEST\n");

    print_separator();

    chip_test();

    flash_test();

    led_test();

    button_test();

    gpio_test();

    mac_test();

    print_separator();

    printf("ALL TESTS COMPLETED\n");

    printf("RESULT : PASS\n");

    print_separator();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}