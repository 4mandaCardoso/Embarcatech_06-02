#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// Define os pinos a serem utilizados
#define SERVO_GPIO 22  // GPIO para controle do servomotor via PWM
#define LED_GPIO   12  // GPIO para o LED RGB (experimento BitDogLab)

// =============================================================================
// Função: aguarda_com_led
// Descrição: Realiza uma espera pelo tempo indicado (em milissegundos)
//            e, durante esse intervalo, alterna o estado do LED (GPIO 12)
//            a cada 500 ms para demonstrar a execução paralela de tarefas.
// Parâmetro:
//    tempo_ms - tempo total de espera em milissegundos.
// =============================================================================
void aguarda_com_led(uint32_t tempo_ms) {
    uint32_t inicio = to_ms_since_boot(get_absolute_time());
    uint32_t ultimo_toggle = inicio;
    bool estado_led = false;
    
    while ((to_ms_since_boot(get_absolute_time()) - inicio) < tempo_ms) {
        uint32_t agora = to_ms_since_boot(get_absolute_time());
        if ((agora - ultimo_toggle) >= 500) {  // Alterna o LED a cada 500 ms
            estado_led = !estado_led;
            gpio_put(LED_GPIO, estado_led);
            ultimo_toggle = agora;
        }
        sleep_ms(10);  // Pequena pausa para não sobrecarregar o loop
    }
}

// =============================================================================
// Função: main
// Descrição: Função principal que realiza:
//    - Inicialização do ambiente e das interfaces (stdIO, GPIO, PWM)
//    - Configuração do PWM na GPIO 22 para uma frequência de 50 Hz
//    - Controle do servomotor para posicionamentos em 180°, 90° e 0°,
//      aguardando 5 segundos em cada posição (com o LED piscando)
//    - Execução de uma rotina contínua de varredura suave do servo entre 0° e 180°
// =============================================================================
int main() {
    // Inicializa o sistema padrão (STDIO, temporizadores, etc.)
    stdio_init_all();
    
    // Configuração do pino do servomotor (GPIO 22) para função PWM
    gpio_set_function(SERVO_GPIO, GPIO_FUNC_PWM);
    
    // Obtem o número do slice e canal PWM associado à GPIO 22
    uint slice_servo = pwm_gpio_to_slice_num(SERVO_GPIO);
    uint canal_servo = pwm_gpio_to_channel(SERVO_GPIO);
    
    /*
      Configuração do PWM:
        - O clock base do PWM é de 125 MHz (do RP2040).
        - Para obter uma frequência de 50 Hz (período de 20 ms),
          configuramos o divisor do clock para 125.0, o que gera um
          clock efetivo de 1 MHz (125 MHz / 125 = 1 MHz).
        - Com um contador (wrap) de 19999, temos:
              1 MHz / (19999 + 1) = 50 Hz.
        - Dessa forma, cada tick equivale a 1 µs, possibilitando o uso
          direto dos valores de pulso em microsegundos.
    */
    pwm_set_clkdiv(slice_servo, 125.0f);    // Configura divisor: 125 MHz / 125 = 1 MHz
    pwm_set_wrap(slice_servo, 19999);         // Período de 20 ms (20000 µs)
    pwm_set_chan_level(slice_servo, canal_servo, 0); // Inicialmente, sem pulso
    pwm_set_enabled(slice_servo, true);       // Habilita o PWM
    
    // Configuração do LED RGB (GPIO 12) como saída digital
    gpio_init(LED_GPIO);
    gpio_set_dir(LED_GPIO, GPIO_OUT);
    
    // =============================================================================
    // Controle do servomotor conforme os requisitos:
    // -----------------------------------------------------------------------------
    // 1) Posição de 180°: pulso de 2400 µs (aprox. 12% do ciclo)
    // -----------------------------------------------------------------------------
    pwm_set_chan_level(slice_servo, canal_servo, 2400);
    printf("Posicionando o servomotor em 180 graus (2400 µs)\n");
    aguarda_com_led(5000); // Aguarda 5 segundos com atualização do LED
    
    // -----------------------------------------------------------------------------
    // 2) Posição de 90°: pulso de 1470 µs (aprox. 7,35% do ciclo)
    // -----------------------------------------------------------------------------
    pwm_set_chan_level(slice_servo, canal_servo, 1470);
    printf("Posicionando o servomotor em 90 graus (1470 µs)\n");
    aguarda_com_led(5000); // Aguarda 5 segundos com atualização do LED
    
    // -----------------------------------------------------------------------------
    // 3) Posição de 0°: pulso de 500 µs (aprox. 2,5% do ciclo)
    // -----------------------------------------------------------------------------
    pwm_set_chan_level(slice_servo, canal_servo, 500);
    printf("Posicionando o servomotor em 0 graus (500 µs)\n");
    aguarda_com_led(5000); // Aguarda 5 segundos com atualização do LED
    
    // =============================================================================
    // Rotina de varredura suave do servomotor entre 0° e 180°:
    //    Incrementa ou decrementa o pulso em passos de 5 µs com atraso de 10 ms.
    //    Essa rotina proporciona uma movimentação suave e contínua do braço.
    // =============================================================================
    while (true) {
        // Varredura crescente: de 0° (500 µs) até 180° (2400 µs)
        for (int pulso = 500; pulso <= 2400; pulso += 5) {
            pwm_set_chan_level(slice_servo, canal_servo, pulso);
            sleep_ms(10);
        }
        // Varredura decrescente: de 180° (2400 µs) até 0° (500 µs)
        for (int pulso = 2400; pulso >= 500; pulso -= 5) {
            pwm_set_chan_level(slice_servo, canal_servo, pulso);
            sleep_ms(10);
        }
    }
    
    return 0;
}
