#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// -----------------------------------------------------------------------------
// Definição dos pinos utilizados
// -----------------------------------------------------------------------------
#define SERVO_PIN 22  // Pino para controle do servomotor via PWM
#define LED_PIN   12  // Pino para controle do LED com efeito fade

// -----------------------------------------------------------------------------
// Configurações do PWM para o servomotor
// -----------------------------------------------------------------------------
#define SERVO_DIVISOR 125.0f  // Divisor do clock para obter 1 MHz (125 MHz / 125)
#define SERVO_WRAP 19999      // Wrap para gerar um período de 20 ms (50 Hz)

// -----------------------------------------------------------------------------
// Configurações do PWM para o LED (efeito fade)
// -----------------------------------------------------------------------------
#define LED_DIVISOR 125.0f    // Divisor do clock para o LED (1 MHz efetivo)
#define LED_WRAP 19999        // Período de 20 ms
#define LED_MIN 500           // Valor mínimo do PWM (brilho mínimo)
#define LED_MAX 2400          // Valor máximo do PWM (brilho máximo)
#define LED_PASSO 10          // Incremento/decremento do nível do LED em cada interrupção

// -----------------------------------------------------------------------------
// Função: manipulador_interrupcao_led
// Descrição: Função chamada a cada vez que o contador PWM do LED atinge o wrap.
//            Essa função ajusta o nível do PWM para o LED, criando um efeito de fade
//            (aumenta e diminui gradualmente o brilho do LED).
// -----------------------------------------------------------------------------
void manipulador_interrupcao_led() {
    // Obtém o slice PWM associado ao LED (GPIO 12)
    uint slice_led = pwm_gpio_to_slice_num(LED_PIN);
    // Limpa o flag de interrupção
    pwm_clear_irq(slice_led);

    // Variáveis estáticas para manter o estado do brilho e a direção da mudança
    static int brilho = LED_MIN;
    static bool crescendo = true;  // true: aumenta o brilho; false: diminui o brilho

    // Atualiza o valor do brilho conforme a direção atual
    if (crescendo) {
        brilho += LED_PASSO;
        if (brilho >= LED_MAX) {
            brilho = LED_MAX;
            crescendo = false; // Inverte a direção para diminuir o brilho
        }
    } else {
        brilho -= LED_PASSO;
        if (brilho <= LED_MIN) {
            brilho = LED_MIN;
            crescendo = true;  // Inverte a direção para aumentar o brilho
        }
    }
    // Aplica o novo valor de brilho ao LED
    pwm_set_gpio_level(LED_PIN, brilho);
}

// -----------------------------------------------------------------------------
// Função: main
// Descrição: Função principal que realiza as seguintes etapas:
//    1. Inicializa o sistema e configura os pinos para PWM.
//    2. Configura o PWM para o servomotor (GPIO 22) com uma frequência de 50 Hz.
//    3. Configura o PWM para o LED (GPIO 12) com efeito de fade através de interrupção.
//    4. Posiciona o servomotor em 180° (2400 µs), 90° (1470 µs) e 0° (500 µs),
//       com pausas de 5 segundos em cada posição.
//    5. Executa uma rotina de varredura suave do servomotor entre 0° e 180°.
// -----------------------------------------------------------------------------
int main() {
    // Inicializa o sistema padrão (STDIO, temporizadores, etc.)
    stdio_init_all();

    // ------------------------------------------------------------
    // Configuração do PWM para o servomotor (GPIO 22)
    // ------------------------------------------------------------
    // Define o pino do servomotor para a função PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    // Obtém o slice e o canal PWM correspondentes ao pino do servomotor
    uint slice_servo = pwm_gpio_to_slice_num(SERVO_PIN);
    uint canal_servo = pwm_gpio_to_channel(SERVO_PIN);

    // Configura o divisor de clock para o PWM do servomotor
    pwm_set_clkdiv(slice_servo, SERVO_DIVISOR);  // 125 MHz / 125 = 1 MHz
    // Configura o wrap para gerar um período de 20 ms (50 Hz)
    pwm_set_wrap(slice_servo, SERVO_WRAP);
    // Inicializa o nível PWM do servomotor (0 -> desligado)
    pwm_set_chan_level(slice_servo, canal_servo, 0);
    // Habilita o PWM para o servomotor
    pwm_set_enabled(slice_servo, true);

    // ------------------------------------------------------------
    // Configuração do PWM para o LED com efeito fade (GPIO 12)
    // ------------------------------------------------------------
    // Define o pino do LED para a função PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    // Obtém o slice PWM associado ao LED
    uint slice_led = pwm_gpio_to_slice_num(LED_PIN);
    // Configura o divisor de clock e o wrap para o LED
    pwm_set_clkdiv(slice_led, LED_DIVISOR);
    pwm_set_wrap(slice_led, LED_WRAP);
    // Inicializa o LED com o nível mínimo de brilho
    pwm_set_gpio_level(LED_PIN, LED_MIN);

    // Configuração da interrupção para o efeito fade do LED:
    // - Limpa o flag de interrupção
    pwm_clear_irq(slice_led);
    // - Habilita a interrupção para o slice do LED
    pwm_set_irq_enabled(slice_led, true);
    // - Configura o manipulador exclusivo para a interrupção de wrap do PWM
    irq_set_exclusive_handler(PWM_IRQ_WRAP, manipulador_interrupcao_led);
    // - Habilita a interrupção global
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // ------------------------------------------------------------
    // Controle do servomotor conforme os requisitos:
    // 1) Posiciona o servo em 180° (pulso de 2400 µs)
    // 2) Posiciona o servo em 90° (pulso de 1470 µs)
    // 3) Posiciona o servo em 0° (pulso de 500 µs)
    // ------------------------------------------------------------

    // Posiciona o servomotor em 180° (2400 µs)
    pwm_set_chan_level(slice_servo, canal_servo, 2400);
    printf("Servo posicionado em 180 graus (2400 µs)\n");
    // Aguarda 5 segundos (durante esse tempo, o LED continuará com efeito fade)
    sleep_ms(5000);

    // Posiciona o servomotor em 90° (1470 µs)
    pwm_set_chan_level(slice_servo, canal_servo, 1470);
    printf("Servo posicionado em 90 graus (1470 µs)\n");
    sleep_ms(5000);

    // Posiciona o servomotor em 0° (500 µs)
    pwm_set_chan_level(slice_servo, canal_servo, 500);
    printf("Servo posicionado em 0 graus (500 µs)\n");
    sleep_ms(5000);

    // ------------------------------------------------------------
    // Rotina de varredura suave do servomotor entre 0° e 180°
    // Incrementa ou decrementa o valor do pulso em passos de 5 µs com atraso de 10 ms.
    // Essa rotina gera um movimento contínuo e suave do braço do servomotor.
    // ------------------------------------------------------------
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
