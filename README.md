## PROJETO - Controle de Servomotor e LED com Efeito Fade no RP2040  
**(ATIVIDADE: Controle de Servomotor por PWM)**

───────────────────────────────────────────────

*Idealizadora do Projeto:*  
**AMANDA CARDOSO LOPES**

───────────────────────────────────────────────

## OBJETIVO DO PROJETO

───────────────────────────────────────────────

Este projeto tem como finalidade demonstrar o uso do módulo PWM do microcontrolador RP2040 (Raspberry Pi Pico W) para controlar a posição de um servomotor e, simultaneamente, criar um efeito visual de fade em um LED.  
  
O código desenvolvido em linguagem C, utilizando o Pico SDK, cumpre os seguintes requisitos:
  
1. **Controle de Servomotor:**  
   - Configurar o PWM na GPIO 22 para operar a aproximadamente 50 Hz (período de 20 ms).  
   - Posicionar o servomotor em 180° (2400 µs), 90° (1470 µs) e 0° (500 µs) com pausas de 5 segundos em cada posição.  
   - Executar uma varredura suave, movendo o braço do servomotor entre 0° e 180° com incrementos de 5 µs a cada 10 ms.
  
2. **Efeito Fade no LED:**  
   - Utilizar a GPIO 12 para controlar um LED via PWM, criando um efeito de fade que aumenta e diminui gradualmente a intensidade do LED.  
   - O efeito é implementado por meio de uma rotina de interrupção, que ajusta o brilho do LED de forma contínua, independentemente do controle do servomotor.

───────────────────────────────────────────────

## FUNCIONALIDADES DO PROJETO

───────────────────────────────────────────────

1. **Configuração do PWM para o Servomotor:**  
   - O servomotor (simulado no Wokwi) é controlado via PWM na GPIO 22, com ajustes precisos de largura de pulso para definir a posição desejada (180°, 90° e 0°).

2. **Posicionamento do Servomotor:**  
   - O código posiciona o servomotor em 180° (pulso de 2400 µs), 90° (pulso de 1470 µs) e 0° (pulso de 500 µs), com uma pausa de 5 segundos em cada posição para visualização.

3. **Varredura Suave do Servomotor:**  
   - Após os posicionamentos fixos, o servomotor executa uma varredura suave entre os ângulos extremos, demonstrando um movimento contínuo e gradual.

4. **Efeito Fade no LED:**  
   - O LED (GPIO 12) é controlado via PWM com um efeito de fade: o brilho aumenta e diminui de forma suave utilizando uma rotina de interrupção (handler), que altera o nível do PWM periodicamente.
   - Esse efeito demonstra a capacidade de multitarefa do RP2040, onde o controle do LED ocorre paralelamente ao movimento do servomotor.

───────────────────────────────────────────────

## COMPONENTES UTILIZADOS

───────────────────────────────────────────────

### Elementos do Projeto
- **Microcontrolador:** Raspberry Pi Pico W (RP2040)
- **Servomotor:** Micro servo padrão (simulado no Wokwi)
- **LED:** LED RGB (simulado, utilizado para o efeito fade)
- **Ambiente de Desenvolvimento:**  
  - VS Code integrado com o Pico SDK  
  - Simulador de eletrônica online Wokwi

───────────────────────────────────────────────

## COMO EXECUTAR O PROJETO?

───────────────────────────────────────────────

### Via VS Code

1. **Configuração Inicial:**
   - vá ao vs code
   - Clone o repositório (ou inicie um novo repositório local):
     ```sh
     git init
     git clone https://github.com/4mandaCardoso/Embarcatech_06-02.git
     ```
   - Certifique-se de que o ambiente do Pico SDK esteja configurado corretamente no VS Code.

2. **Compile e verifique se na pasta "build" consta um arquivo .uf2**

3. **Observação do experimento**
   - Caso não possua os periféricos necessários, clique no arquivo "diagram.json", para efetuar a simulação, caso possua, conecte o Raspberry Pi Pico W no modo de boot (pressionando o botão BOOTSEL) e arraste o arquivo .UF2 para o diretório da placa.


### Opção alternativa (Simulação no navegador)
- Outra possibilidade alternativa é ir diretamente no wokwi web e apenas simular, para isso, estou disponibilizando o link do Wokwi abaixo:
🔗 https://wokwi.com/projects/422456637945210881

───────────────────────────────────────────────

## VÍDEO DE DEMONSTRAÇÃO

───────────────────────────────────────────────

### Link do Vídeo:
🔗 https://drive.google.com/drive/folders/1VYKZcZhyShI15_mlftmWBO9v3ALxqLJZ?usp=sharing

───────────────────────────────────────────────

## CONCLUSÃO

───────────────────────────────────────────────

Este projeto foi fundamental para aprofundar meu entendimento sobre a aplicação do PWM em sistemas embarcados. Ao controlar tanto um servomotor quanto um LED de forma independente, foi possível demonstrar a capacidade do RP2040 para executar múltiplas tarefas simultaneamente. O efeito fade no LED evidencia como o PWM pode modular a intensidade luminosa, enquanto o controle do servomotor demonstra precisão no posicionamento através da variação dos pulsos. Esse trabalho integra hardware e software de maneira prática, contribuindo para meu aprendizado e consolidação dos conceitos de controle digital e multitarefa em microcontroladores.

───────────────────────────────────────────────

*Desenvolvido por: Amanda Cardoso Lopes*


teste no wokwi : https://wokwi.com/projects/422456637945210881
