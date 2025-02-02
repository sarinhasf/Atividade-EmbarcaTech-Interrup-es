#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "interrupt.pio.h"

#define DEBOUNCE_DELAY 50 //  Definição do tempo de debounce (ajustado para 50ms)

#define NUM_PIXELS 25 //número de LEDs


#define OUT_PIN 7 //pino de saída para comunicação com leds

// Definição dos LEDs RGB
#define RLED_PIN 13 // pino led red
#define GLED_PIN 11 // pino led green
#define BLED_PIN 12 // pino led blue

// Definição dos botões
#define BTNA_PIN 5 //botão a
#define BTNB_PIN 6 // botão b

// Variáveis para debounce
int contador = 0;
static volatile uint32_t last_time = 0; // Armazena o tempo da última interrupção do botão

void init_all() {
    gpio_init(RLED_PIN);
    gpio_set_dir(RLED_PIN, GPIO_OUT);
    gpio_put(RLED_PIN, 0);

    gpio_init(GLED_PIN);
    gpio_set_dir(GLED_PIN, GPIO_OUT);
    gpio_put(GLED_PIN, 0);

    gpio_init(BLED_PIN);
    gpio_set_dir(BLED_PIN, GPIO_OUT);
    gpio_put(BLED_PIN, 0);

    gpio_init(BTNA_PIN);
    gpio_set_dir(BTNA_PIN, GPIO_IN);
    gpio_pull_up(BTNA_PIN);

    gpio_init(BTNB_PIN);
    gpio_set_dir(BTNB_PIN, GPIO_IN);
    gpio_pull_up(BTNB_PIN);
}
//função para controlar os leds RGB
void get_led(bool R, bool G, bool B) {
    gpio_put(RLED_PIN, R);
    gpio_put(GLED_PIN, G);
    gpio_put(BLED_PIN, B);
}


// Matriz representando os numeros em leds 5x5

double numeros[10][25] = {
{ // desenho do numero 0
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 1
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 2
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 3
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 4
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0
},
{ // desenho do numero 5
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 6
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 7
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
},
{ // desenho do numero 8
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // desenho do numero 9
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0
}

};

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para exibir o desenho na matriz de leds
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor vermelha para cada LED
        valor_led = matrix_rgb(0.0, 0.1*desenho[24 - i], 0.0); // Apenas o valor vermelho está ativo com intensidade baixa
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }

}

// Inicializa o sistema de clock
void inicializar_clock() {
    bool ok = set_sys_clock_khz(100000, false);
    if (ok) {
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

// Configura a PIO para comunicação com a matriz de LEDs
void configurar_pio(PIO pio, uint *offset, uint *sm) {
    *offset = pio_add_program(pio, &interrupt_program);
    *sm = pio_claim_unused_sm(pio, true);
    interrupt_program_init(pio, *sm, *offset, OUT_PIN);
}

// Função para exibir um número na matriz de LEDs
void print_valor(int valor, PIO pio, uint sm){

    uint32_t valor_led;
    if (valor <= 9 && valor >= 0){
        for (int16_t i = 0; i < NUM_PIXELS; i++) {
            // Define a cor vermelha para cada LED
            valor_led = matrix_rgb(0.0, 0.1*(numeros[valor][24 - i]), 0.0); // Apenas o valor vermelho está ativo com intensidade baixa
            pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
        }
    } else if (valor < 0) {
        printf("Valor incompatível.\n"); // caso valor seja abaixo de 0
        contador = 0;
    } else if (valor > 9){
        printf("Valor incompatível.\n"); //caso valor seja acima de 9
        contador = 9;
    }
}

PIO pio = pio0;
uint32_t valor_led;
uint offset, sm;

// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events){

    uint32_t current_time = to_us_since_boot(get_absolute_time()); //captura tempo atual
        if (current_time - last_time > DEBOUNCE_DELAY){
            last_time = current_time; // atualiza o tempo da ultima interrupção válida
            
            if (gpio == BTNA_PIN && contador < 9){ //se pressionar o botão A
            contador++; // incrementa no contador
            print_valor(contador, pio, sm); //mostra o valor numerico nos leds
            printf("Contador = %i\n", contador); // imprime no terminal o valor do contador
            }   
            else if (gpio == BTNB_PIN && contador > 0){ //se pressionar o botão B
            contador-- ; //decrementa no contador
            print_valor(contador, pio, sm); // mostra o valor numerico nos leds
            printf("Contador = %i\n", contador); // imprime no terminal o valor do contador
            }
    }
    else{
       return; // Ignora se estiver dentro do tempo de debounce 
    }

}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    inicializar_clock();

    double r = 0.0, b = 0.0, g = 0.0;

    configurar_pio(pio, &offset, &sm);

    printf("Sistema inicializado. Aguardando entrada...\n");

    // Configuração dos botões como interrupções
    gpio_set_irq_enabled_with_callback(BTNA_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTNB_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Iniciando o sistema escrevendo 0 na matriz de LEDs
    print_valor(0, pio, sm);

    // Rotina inicial do programa para teste
    gpio_put(RLED_PIN, 1);
    sleep_ms(300);
    gpio_put(RLED_PIN, 0);

    while (true) {
    
       // Repetição para que o LED vermelho pisque 5 vezes por segundo.
       for (int i = 0; i < 5; i++){
            gpio_put(RLED_PIN, 1);
            sleep_ms(100);
            gpio_put(RLED_PIN, 0);
            sleep_ms(100);
       }
    }
    return 0;
}