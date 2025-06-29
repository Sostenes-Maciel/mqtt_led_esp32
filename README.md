Projeto: Controle de LED via MQTT com ESP32

Este projeto demonstra como utilizar o ESP32 para acionar um LED através de comandos MQTT. O ESP32 se conecta a uma rede Wi-Fi e a um broker MQTT público (HiveMQ), escutando um tópico específico. Quando recebe "1", o LED é aceso; quando recebe "0", o LED é apagado.

📡 Funcionamento

O ESP32 se conecta à rede Wi-Fi.

Em seguida, se inscreve no tópico MQTT:

/ifpe/ads/embarcados/esp32/led

Quando recebe:

"1" → Liga o LED (GPIO 2)

"0" → Desliga o LED

⚙️ Componentes Utilizados

ESP32 DevKit

LED (conectado ao GPIO 2)

Broker MQTT público: mqtt://broker.hivemq.com:1883

🛆 Tecnologias

ESP-IDF v5.4

FreeRTOS

MQTT (com esp-mqtt)

Wi-Fi STA Mode

GPIO

🧪 Testes

Você pode testar o envio de mensagens MQTT usando ferramentas como:

MQTT Explorer

HiveMQ Web Client

Tópico:

/ifpe/ads/embarcados/esp32/led

Payloads:

"1" → acende o LED

"0" → apaga o LED

🚀 Como executar

1. Clone o repositório

git clone https://github.com/Sostenes-Maciel/mqtt_led_esp32.git
cd mqtt_led_esp32

2. Compile e grave o projeto

Requer o ESP-IDF corretamente instalado e configurado

idf.py set-target esp32
idf.py build
idf.py flash
idf.py monitor

3. Configure sua rede Wi-Fi

No código (main.c), altere para os dados da sua rede:

#define WIFI_SSID "SEU_SSID"
#define WIFI_PASS "SUA_SENHA"

4. Resultado Esperado

O terminal mostrará algo como:

WiFi conectado com IP!
Conectado ao broker MQTT!
Tópico: /ifpe/ads/embarcados/esp32/led
Mensagem: 1
LED ACESO
Mensagem: 0
LED APAGADO

📁 Estrutura do Projeto

mqtt_led_esp32/
├── main/
│   └── main.c
├── CMakeLists.txt
├── sdkconfig
├── README.md

📚 Autor

Sóstenes Maciel Estudante de Análise e Desenvolvimento de Sistemas 
