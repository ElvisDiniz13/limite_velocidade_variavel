#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

#define BTN_A_PIN 5
#define DEBOUNCE_TIME_MS 200
#define INTERVAL 5000  // Intervalo de tempo em milissegundos (10s)


int contador = 0;

int main()
{



    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    int exceeded_limit = 0;


    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 5000);
    //i2c_init(i2c1, ssd1306_i2c_clock);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    //Inicializa os pinos dos botões como entrada
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
/*
    char *text[] = {
        "  Bem-vindos!   ",
        "  Embarcatech   "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
*/

// Parte do código para exibir a linha no display (algoritmo de Bresenham)
/**
    ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
    render_on_display(ssd, &frame_area);
*/

// Parte do código para exibir o bitmap no display
/*
    const uint8_t bitmap_128x64[] = { 
        0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x70, 0x9c, 0x03, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x30, 0x8e, 
        0x03, 0x00, 0xf0, 0xff, 0x3f, 0x00, 0x18, 0x86, 0x03, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x1c, 0x83, 
        0x03, 0x00, 0xf0, 0xff, 0x01, 0x00, 0x8e, 0x83, 0x03, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x86, 0x81, 
        0x03, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0xc7, 0x81, 0x03, 0x00, 0xf0, 0x03, 0x00, 0x80, 0xe3, 0x80, 
        0x03, 0x00, 0x70, 0x00, 0x00, 0x80, 0x63, 0x80, 0x03, 0x00, 0x10, 0x00, 0x00, 0xc0, 0x71, 0x80, 
        0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x31, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x80, 
        0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x38, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x18, 0x80, 
        0x03, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x1c, 0x80, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x7f, 0x1c, 0x80, 
        0x03, 0x00, 0x00, 0x00, 0xff, 0x3f, 0x0c, 0x80, 0x03, 0x00, 0x00, 0xc0, 0xff, 0x3f, 0x0c, 0x80, 
        0x03, 0x00, 0x00, 0xf8, 0xff, 0x3f, 0x0e, 0x80, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x3f, 0x0e, 0x80, 
        0x03, 0x00, 0x80, 0xff, 0xff, 0x3f, 0x06, 0x80, 0x03, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x80, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0x1d, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0x0f, 0x1c, 0x07, 0xff, 
        0xff, 0x0f, 0x00, 0xfe, 0x01, 0x1c, 0x07, 0xf8, 0xff, 0x00, 0x00, 0x7f, 0x00, 0x1c, 0x07, 0xe0, 
        0x3f, 0x00, 0x80, 0x3f, 0x00, 0x1c, 0x03, 0xc0, 0x0f, 0x00, 0xc0, 0x1f, 0x00, 0x1c, 0x03, 0x80, 
        0x07, 0x00, 0xe0, 0x0f, 0x00, 0x1c, 0x03, 0x80, 0x03, 0x00, 0xe0, 0x0f, 0x00, 0x1c, 0x03, 0x00, 
        0x01, 0x00, 0xf0, 0x07, 0x00, 0x1c, 0x03, 0x80, 0x01, 0x00, 0xf0, 0x07, 0x00, 0xfc, 0x03, 0x80, 
        0x01, 0x00, 0xfe, 0x07, 0x00, 0xff, 0x03, 0x80, 0x01, 0x00, 0xff, 0x03, 0x80, 0xff, 0x03, 0xc0, 
        0x01, 0x80, 0xff, 0x03, 0xc0, 0xff, 0x03, 0xe0, 0x01, 0x80, 0xff, 0x01, 0xe0, 0xff, 0x03, 0xf0, 
        0x01, 0x80, 0xff, 0x01, 0xf0, 0x7f, 0x07, 0xf8, 0x01, 0x00, 0x7e, 0x00, 0xf8, 0x1f, 0x07, 0xfe, 
        0x01, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x80, 
        0x03, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x06, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x18, 0x07, 0x80, 
        0x0f, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x80, 
        0xff, 0x00, 0x00, 0x00, 0x00, 0x38, 0x06, 0x80, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x38, 0x0e, 0x80, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0x33, 0xfe, 0xff, 
        0xff, 0x03, 0x00, 0xc0, 0xff, 0x73, 0xdc, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xff, 0x73, 0x1c, 0xfe, 
        0x1f, 0x00, 0x00, 0x00, 0xfc, 0x63, 0x18, 0xf8, 0x07, 0x00, 0x00, 0x00, 0xf8, 0x63, 0x18, 0xe0, 
        0x03, 0x00, 0x00, 0x00, 0xf0, 0xe3, 0x38, 0xc0, 0x03, 0x00, 0x00, 0x00, 0xe0, 0xe3, 0x38, 0xc0, 
        0x01, 0x00, 0x00, 0x00, 0xe0, 0xc3, 0x30, 0x80, 0x01, 0x00, 0x00, 0x00, 0xc0, 0xc7, 0x30, 0x80, 
        0x01, 0x80, 0x0f, 0x00, 0xc0, 0xff, 0x71, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x80, 0xff, 0x71, 0x80, 
        0x00, 0xe0, 0x3f, 0x00, 0x80, 0xff, 0x61, 0x80, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0xff, 0x63, 0x80, 
        0x00, 0x80, 0x7f, 0x00, 0x00, 0xfe, 0xe3, 0x00, 0x01, 0x00, 0x78, 0x00, 0x00, 0xf8, 0xc3, 0x80, 
        0x01, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xc3, 0x80, 0x01, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xc7, 0x81, 
        0x03, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x86, 0xc1, 0x07, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x86, 0xe1, 
        0x1f, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x8e, 0xf3, 0x7f, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x8e, 0xff, 
        0xff, 0x07, 0xf8, 0x3f, 0x00, 0x00, 0x0c, 0xff, 0xff, 0xff, 0xfb, 0xff, 0x01, 0x00, 0xfc, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x3f, 0x00, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0xff, 0x01, 0x00, 0xc0, 0xff, 0x03, 0x98, 0xff, 
        0x7f, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x18, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x18, 0xfe, 
        0x07, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x38, 0xee, 0x03, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x38, 0xce, 
        0x01, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x38, 0x8e, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x30, 0x8e, 
        0x01, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x30, 0x8e, 0x01, 0xc0, 0x0f, 0x00, 0xc0, 0xff, 0x30, 0x8c, 
        0x00, 0xe0, 0x3f, 0x00, 0x80, 0xff, 0x31, 0x8c, 0x00, 0xe0, 0x3f, 0x00, 0x80, 0xff, 0x33, 0x8c, 
        0x00, 0xc0, 0x7f, 0x00, 0x00, 0xff, 0x33, 0x8c, 0x01, 0x80, 0x7f, 0x00, 0x00, 0xfe, 0x31, 0x8c, 
        0x01, 0x00, 0x78, 0x00, 0x00, 0x00, 0x30, 0x8c, 0x01, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x30, 0x8c, 
        0x03, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x30, 0xcc, 0x07, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x30, 0xcc, 
        0x0f, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x30, 0xfc, 0x3f, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x30, 0xfc, 
        0xff, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x30, 0xfe, 0xff, 0x0f, 0xf8, 0x7f, 0x00, 0x00, 0xb0, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xe0, 0xff, 0xff, 
        0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
        0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xff, 
        0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xfb, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xe3, 
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0xc3, 0x03, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0x86, 0x81, 
        0x01, 0x00, 0xf0, 0xff, 0xff, 0x1f, 0x86, 0x81, 0x01, 0x00, 0xfc, 0xff, 0xff, 0x7f, 0xc7, 0x01, 
        0x01, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xc3, 0x00, 0x01, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xc3, 0x00, 
        0x01, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xe3, 0x00, 0x01, 0x00, 0xf8, 0xff, 0xff, 0xbf, 0x61, 0x80, 
        0x03, 0x00, 0xc0, 0xff, 0xff, 0xc7, 0x71, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30, 0xc0, 
        0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x38, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0xf0, 
        0x3f, 0x00, 0x00, 0x00, 0x00, 0x70, 0x1c, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0xfe, 
        0xff, 0x03, 0x00, 0x00, 0x00, 0x38, 0x8e, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x18, 0xfe, 0xff, 
        0xff, 0xff, 0x01, 0x00, 0x00, 0x1c, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xfe, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);

*/
    while(true) {
        printf("TEMPO %d\n", (to_ms_since_boot(get_absolute_time()) - start_time));
        if(gpio_get(BTN_A_PIN) == 0){
            sleep_ms(DEBOUNCE_TIME_MS); // Espera para evitar múltiplas leituras
            contador++;
            printf("O número n é %d.\n", contador);
          }

          if ((to_ms_since_boot(get_absolute_time()) - start_time >= INTERVAL) &&(contador != 0)) {
            if (contador >= 10) { //Quantidade para exibir a velocidade de 80 km/h
                exceeded_limit = 1;
            } 
            else if((contador > 5) && (contador < 10))  { //Quantidade para exibir a velocidade de 100 km/h
                exceeded_limit = 2;
            }
            else if(contador <= 5)  { //Quantidade para exibir a velocidade de 120 km/h
                exceeded_limit = 3;
            }else {
                exceeded_limit = 0;
                contador = 0;
            }

            printf("Resultado: %d (Total de cliques: %d)\n", exceeded_limit, contador);

            // Reiniciar contagem
            contador = 0;
            start_time = to_ms_since_boot(get_absolute_time());
        }
        if((exceeded_limit == 1) && (to_ms_since_boot(get_absolute_time()) - start_time >= INTERVAL)){
            printf("Contei 80\n");
            
            // Parte do código para exibir o bitmap de 80 km/h no display

            const uint8_t bitmap_128x64[] = { 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 
                0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
                0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 
                0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 0x0f, 0xf0, 0xff, 
                0xff, 0x01, 0xfe, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0x3f, 0xc0, 0xff, 
                0xff, 0x80, 0xff, 0xff, 0xff, 0x7f, 0x80, 0xff, 0x7f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 
                0x3f, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0x3f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfe, 
                0x1f, 0xf0, 0x1f, 0x3e, 0xf8, 0xff, 0x07, 0xfe, 0x1f, 0xf8, 0x0f, 0x1c, 0xf0, 0xff, 0x07, 0xfc, 
                0x1f, 0xf8, 0x07, 0x08, 0xe0, 0xff, 0x0f, 0xfc, 0x0f, 0xfc, 0x03, 0x00, 0xe0, 0xff, 0x0f, 0xfc, 
                0x0f, 0xfc, 0xc3, 0xc1, 0xc3, 0x07, 0x1e, 0xf8, 0x0f, 0xfe, 0xe3, 0xe3, 0xc7, 0x3f, 0x1f, 0xf8, 
                0x07, 0xfe, 0xe3, 0xe3, 0xc7, 0x1f, 0x1f, 0xf8, 0x07, 0xfe, 0xe3, 0xe3, 0xc7, 0x4f, 0x3e, 0xf8, 
                0x07, 0xfe, 0xc3, 0xc1, 0xc3, 0xff, 0x3f, 0xf0, 0x07, 0xfe, 0x03, 0x00, 0xe0, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x07, 0x08, 0xe0, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0x0f, 0x1c, 0xf0, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x1f, 0x3e, 0xf8, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf0, 0x07, 0xff, 0x7f, 0x00, 0xfe, 0x3f, 0x3e, 0xf0, 
                0x07, 0xfe, 0x1f, 0x00, 0xf8, 0x87, 0x3f, 0xf0, 0x07, 0xfe, 0x07, 0x00, 0xe0, 0xff, 0x3f, 0xf0, 
                0x07, 0xfe, 0x07, 0x00, 0xe0, 0x07, 0x3e, 0xf8, 0x07, 0xfe, 0x83, 0xff, 0xc1, 0xdf, 0x1f, 0xf8, 
                0x0f, 0xfe, 0xe3, 0xff, 0xc7, 0x1f, 0x1f, 0xf8, 0x0f, 0xfc, 0xe3, 0xff, 0xc7, 0x7f, 0x1e, 0xf8, 
                0x0f, 0xfc, 0xe3, 0xff, 0xc7, 0xff, 0x0f, 0xfc, 0x1f, 0xf8, 0x83, 0xff, 0xc1, 0xff, 0x0f, 0xfc, 
                0x1f, 0xf8, 0x07, 0x00, 0xe0, 0xff, 0x07, 0xfc, 0x1f, 0xf0, 0x07, 0x00, 0xe0, 0xff, 0x07, 0xfe, 
                0x3f, 0xf0, 0x1f, 0x00, 0xf8, 0xff, 0x03, 0xfe, 0x3f, 0xe0, 0x7f, 0x00, 0xfe, 0xff, 0x01, 0xff, 
                0x7f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x7f, 0x80, 0xff, 
                0xff, 0x01, 0xff, 0xff, 0xff, 0x3f, 0xc0, 0xff, 0xff, 0x01, 0xfe, 0xff, 0xff, 0x1f, 0xe0, 0xff, 
                0xff, 0x03, 0xf8, 0xff, 0xff, 0x0f, 0xf0, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 
                0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 
                0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 
                0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 
                0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

            ssd1306_t ssd_bm;
            ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
            ssd1306_config(&ssd_bm);

            ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
            contador = 0;
            exceeded_limit = 0;
          }

            if((exceeded_limit == 2)  && (to_ms_since_boot(get_absolute_time()) - start_time >= INTERVAL)){
            printf("Contei 100\n");
            
            // Parte do código para exibir o bitmap de 100km/h no display

            const uint8_t bitmap_128x64[] = { 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 
                0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
                0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 
                0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 0x03, 0xf8, 0xf1, 0xff, 0x0f, 0xf0, 0xff, 
                0xff, 0x01, 0xfe, 0xf0, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x01, 0xff, 0xf8, 0xff, 0x3f, 0xc0, 0xff, 
                0xff, 0x80, 0x7f, 0xf8, 0xff, 0x7f, 0x80, 0xff, 0x7f, 0xc0, 0x3f, 0xfc, 0xff, 0xff, 0x00, 0xff, 
                0x3f, 0xe0, 0x0f, 0x00, 0x80, 0xff, 0x01, 0xff, 0x3f, 0xf0, 0x0f, 0x00, 0x80, 0xff, 0x03, 0xfe, 
                0x1f, 0xf0, 0x0f, 0x00, 0x80, 0xff, 0x07, 0xfe, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc, 
                0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 
                0x0f, 0xfc, 0xff, 0xff, 0xff, 0x07, 0x1e, 0xf8, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0xf8, 
                0x07, 0xfe, 0xff, 0x00, 0xf8, 0x1f, 0x1f, 0xf8, 0x07, 0xfe, 0x3f, 0x00, 0xe0, 0x4f, 0x3e, 0xf8, 
                0x07, 0xfe, 0x1f, 0x00, 0xc0, 0xff, 0x3f, 0xf0, 0x07, 0xfe, 0x1f, 0xf8, 0x80, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x0f, 0xff, 0x87, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0x8f, 0xff, 0x8f, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x8f, 0xff, 0x8f, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0x1f, 0xff, 0x87, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x3f, 0xf0, 0x07, 0xff, 0x3f, 0x00, 0xc0, 0x3f, 0x3e, 0xf0, 
                0x07, 0xfe, 0x7f, 0x00, 0xf0, 0x87, 0x3f, 0xf0, 0x07, 0xfe, 0xff, 0x03, 0xfc, 0xff, 0x3f, 0xf0, 
                0x07, 0xfe, 0xff, 0xff, 0xff, 0x07, 0x3e, 0xf8, 0x07, 0xfe, 0xff, 0xff, 0xff, 0xdf, 0x1f, 0xf8, 
                0x0f, 0xfe, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0xf8, 0x0f, 0xfc, 0xff, 0x00, 0xf8, 0x7f, 0x1e, 0xf8, 
                0x0f, 0xfc, 0x3f, 0x00, 0xe0, 0xff, 0x0f, 0xfc, 0x1f, 0xf8, 0x1f, 0x00, 0xc0, 0xff, 0x0f, 0xfc, 
                0x1f, 0xf8, 0x1f, 0xf8, 0x80, 0xff, 0x07, 0xfc, 0x1f, 0xf0, 0x0f, 0xff, 0x87, 0xff, 0x07, 0xfe, 
                0x3f, 0xf0, 0x8f, 0xff, 0x8f, 0xff, 0x03, 0xfe, 0x3f, 0xe0, 0x8f, 0xff, 0x8f, 0xff, 0x01, 0xff, 
                0x7f, 0xc0, 0x1f, 0xff, 0x87, 0xff, 0x00, 0xff, 0xff, 0x80, 0x1f, 0x00, 0xc0, 0x7f, 0x80, 0xff, 
                0xff, 0x01, 0x3f, 0x00, 0xc0, 0x3f, 0xc0, 0xff, 0xff, 0x01, 0x7e, 0x00, 0xf0, 0x1f, 0xe0, 0xff, 
                0xff, 0x03, 0xf8, 0x03, 0xfc, 0x0f, 0xf0, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 
                0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 
                0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 
                0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 
                0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

            ssd1306_t ssd_bm;
            ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
            ssd1306_config(&ssd_bm);

            ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
            contador = 0;
            exceeded_limit = 0;
          }

            if((exceeded_limit == 3) && (to_ms_since_boot(get_absolute_time()) - start_time >= INTERVAL)){
            printf("Contei 120\n");
            
            // Parte do código para exibir o bitmap de 120km/h no display

            const uint8_t bitmap_128x64[] = { 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 
                0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
                0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 
                0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 0xff, 0x03, 0xf8, 0xf1, 0xff, 0x0f, 0xf0, 0xff, 
                0xff, 0x01, 0xfe, 0xf0, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x01, 0xff, 0xf8, 0xff, 0x3f, 0xc0, 0xff, 
                0xff, 0x80, 0x7f, 0xf8, 0xff, 0x7f, 0x80, 0xff, 0x7f, 0xc0, 0x3f, 0xfc, 0xff, 0xff, 0x00, 0xff, 
                0x3f, 0xe0, 0x0f, 0x00, 0x80, 0xff, 0x01, 0xff, 0x3f, 0xf0, 0x0f, 0x00, 0x80, 0xff, 0x03, 0xfe, 
                0x1f, 0xf0, 0x0f, 0x00, 0x80, 0xff, 0x07, 0xfe, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc, 
                0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 
                0x0f, 0xfc, 0xff, 0xff, 0xff, 0x07, 0x1e, 0xf8, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0xf8, 
                0x07, 0xfe, 0xff, 0xfd, 0x9f, 0x1f, 0x1f, 0xf8, 0x07, 0xfe, 0x7f, 0xfc, 0x87, 0x4f, 0x3e, 0xf8, 
                0x07, 0xfe, 0x3f, 0xfc, 0x83, 0xff, 0x3f, 0xf0, 0x07, 0xfe, 0x1f, 0xfc, 0x81, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x1f, 0xff, 0x80, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0x8f, 0x3f, 0x80, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x8f, 0x1f, 0x84, 0xdf, 0x3f, 0xf0, 0x07, 0xff, 0x8f, 0x1f, 0x86, 0x1f, 0x3e, 0xf0, 
                0x07, 0xff, 0x1f, 0x07, 0x87, 0xff, 0x3f, 0xf0, 0x07, 0xff, 0x1f, 0x80, 0x87, 0x3f, 0x3e, 0xf0, 
                0x07, 0xfe, 0x1f, 0xc0, 0x87, 0x87, 0x3f, 0xf0, 0x07, 0xfe, 0x3f, 0xe0, 0x87, 0xff, 0x3f, 0xf0, 
                0x07, 0xfe, 0xff, 0xff, 0xff, 0x07, 0x3e, 0xf8, 0x07, 0xfe, 0xff, 0xff, 0xff, 0xdf, 0x1f, 0xf8, 
                0x0f, 0xfe, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0xf8, 0x0f, 0xfc, 0xff, 0x00, 0xf8, 0x7f, 0x1e, 0xf8, 
                0x0f, 0xfc, 0x3f, 0x00, 0xe0, 0xff, 0x0f, 0xfc, 0x1f, 0xf8, 0x1f, 0x00, 0xc0, 0xff, 0x0f, 0xfc, 
                0x1f, 0xf8, 0x1f, 0xf8, 0x80, 0xff, 0x07, 0xfc, 0x1f, 0xf0, 0x0f, 0xff, 0x87, 0xff, 0x07, 0xfe, 
                0x3f, 0xf0, 0x8f, 0xff, 0x8f, 0xff, 0x03, 0xfe, 0x3f, 0xe0, 0x8f, 0xff, 0x8f, 0xff, 0x01, 0xff, 
                0x7f, 0xc0, 0x1f, 0xff, 0x87, 0xff, 0x00, 0xff, 0xff, 0x80, 0x1f, 0x00, 0xc0, 0x7f, 0x80, 0xff, 
                0xff, 0x01, 0x3f, 0x00, 0xc0, 0x3f, 0xc0, 0xff, 0xff, 0x01, 0x7e, 0x00, 0xf0, 0x1f, 0xe0, 0xff, 
                0xff, 0x03, 0xf8, 0x03, 0xfc, 0x0f, 0xf0, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xff, 0x03, 0xf8, 0xff, 
                0xff, 0x0f, 0x80, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0x0f, 0x00, 0xfe, 0xff, 
                0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 
                0xff, 0xff, 0x03, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0xff, 
                0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

            ssd1306_t ssd_bm;
            ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
            ssd1306_config(&ssd_bm);

            ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
            contador = 0;
            exceeded_limit = 0;
          }
        sleep_ms(50);
    }

    return 0;
}
