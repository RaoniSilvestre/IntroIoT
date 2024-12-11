# Logging de temperatura de sala

Este projeto foi desenvolvido utilizando o PlatformIO e tem como objetivo a realização de logs em uma sala de diversos ESP32 diferentes, sendo possível perceber uma mudança na temperatura ao longo da sala.

## Requisitos

Para rodar o projeto, é necessário o seguinte:

- PlatformIO (recomendado usar via VSCode)
- Um ESP32
- Acesso a um servidor MQTT (recomendo AdafruitIO)


## Estrutura do Projeto

### include/: Contém os cabeçalhos do projeto.

- `config.h`  : Definições gerais e configurações do sistema.
- `file_sys.h`: Funções para manipulação de arquivos.
- `mqtt_c.h`  : Declarações relacionadas à comunicação via MQTT.
- `ntp_s.h`   : Sincronização de tempo via NTP.
- `sensor.h`  : Funções para leitura de sensores.
- `wifi_h.h`  : Gerenciamento de conexão Wi-Fi.
- `secrets.example.h` : Exemplo de arquivo para armazenar credenciais sensíveis.

### src/: Contém a implementação do projeto.

- `main.cpp` : Arquivo principal que inicializa o sistema.
- `file_sys.cpp`, `mqtt_c.cpp`, `ntp_s.cpp`, `sensor.cpp`, `wifi_h.cpp`: Implementação das funcionalidades declaradas nos cabeçalhos correspondentes.

###  `platformio.ini`: Configurações do PlatformIO, incluindo a seleção da placa e bibliotecas.

## Configuração Inicial

1. Clone este repositório:

```bash
git clone <URL_DO_REPOSITORIO>
cd <PASTA_DO_REPOSITORIO>
```

2. Instale as dependências:

Certifique-se de que o PlatformIO (use pelo vscode namoral) instale automaticamente as bibliotecas listadas no platformio.ini ao abrir o projeto.

3. Configurar credenciais:

Renomeie o arquivo secrets.example.h para secrets.h e preencha as informações necessárias:

- SSID e senha da rede Wi-Fi.
- Chave do MQTT e user do MQTT.

4. Mais configurações:

- Atualize o `config.h` de acordo com o seu uso do MQTT.

5. Pegue seu ESP32 e seu DHT11:

- Conecte o VCC do DHT11 ao VCC do ESP32, o GND no GND
- Conecte o pino de dados do DHT11 a GPIO4 do ESP32 (se quiser mudar tbm vc que sabe)

## Execução do Projeto

1. Vá para a aba da extensão do Platformio
2. Clique em `Upload and Monitor`
3. Veja a magia acontecer!

## Funcionalidades

1. Conexão Wi-Fi

O código no arquivo `wifi_h.cpp` gerencia a conexão com a rede Wi-Fi configurada em `secrets.h.` Caso a conexão falhe, ele tenta reconectar automaticamente.

2. Sincronização de Tempo (NTP)

O arquivo `ntp_s.cpp` sincroniza o tempo do dispositivo com o servidor NTP configurado. Isso é utilizado para timestamp de logs no ESP32.

3. Leitura de Sensores

O arquivo `sensor.cpp` implementa a leitura de sensores conectados ao dispositivo.

4. Comunicação MQTT

O arquivo `mqtt_c.cpp` envia mensagens via protocolo MQTT, utilizando as credenciais configuradas.

5. Gerenciamento de Arquivos

O arquivo `file_sys.cpp` manipula arquivos locais no dispositivo, permitindo armazenar logs e valores enviados ao MQTT.

6. Multithreading

No arquivo `main.cpp` em vez de usar um loop normal, está sendo utilizado o multithreading para separar a parte de leitura de dados do sensor e o envio de dados ao MQTT.

## Funcionamento


https://drive.google.com/file/d/16oBbHonVXMyukKMgPK28yyqJM1QnP-bB/view?usp=drivesdk
