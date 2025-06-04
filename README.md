# Tarefa: IoT Security Lab - EmbarcaTech 2025

Autores: Thiago Young de Azevedo e Luan Felipe Azzi

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, Maio de 2025

# Segurança em IoT com BitDogLab (C/C++)  
## Projeto de Comunicação Segura via MQTT com Raspberry Pi Pico W

### Descrição
Este projeto demonstra uma comunicação segura entre dispositivos IoT utilizando a placa **BitDogLab** com **Raspberry Pi Pico W**, implementada em **C** com o **SDK do Pico** e a biblioteca **lwIP** para MQTT. O foco principal está na **segurança da comunicação**, com autenticação, criptografia leve e proteção contra ataques como *sniffing* e *replay*.

---

### Funcionalidades Implementadas

- Conexão à rede Wi-Fi (modo estação – `cyw43_arch`)  
- Comunicação MQTT básica com publicações em tópicos  
- Autenticação simples no broker Mosquitto (usuário e senha)  
- Criptografia leve usando XOR (ofuscação de payload)  
- Proteção contra replay utilizando timestamp nas mensagens  

---

### Testes Realizados

#### Conexão e MQTT
- BitDogLab se conectou com sucesso à rede Wi-Fi local.
- Publicação em tópico `escola/sala1/temperatura` usando `mqtt_publish`.
- Comunicação entre duas placas, sendo uma publisher e outra subscriber utilizando o computador como broker.

#### Segurança
- Broker configurado com autenticação via `mosquitto_passwd`.
- Mensagens ofuscadas com XOR visivelmente cifradas no Wireshark.
- Subscriber validando timestamp para detectar mensagens repetidas (*replay attacks*).

---

### Discussão e Análise

#### Técnicas Escaláveis

| Técnica                        | Escalável? | Observações |
|-------------------------------|------------|-------------|
| MQTT com autenticação         |    Sim     | Compatível com ambientes reais |
| Criptografia XOR              |    Não     | Somente para fins didáticos |
| Proteção com timestamp        |    Sim     | Requer sincronização mínima |

#### Aplicação em Ambientes Escolares

- Utilizar um **broker central Mosquitto** com autenticação para todas as BitDogLab.
- Atribuir **IDs únicos** para cada dispositivo (`bitdog1`, `bitdog2`, ...).
- Criar uma estrutura de tópicos organizada por sala/laboratório.
- Substituir XOR.
- Criar dashboards web para visualização dos dados em tempo real.

---

### Requisitos

- 2x BitDogLab (com Pico W)
- Mosquitto Broker (instalado e configurado no desktop)
- VSCode com Pico SDK e CMake
- Wireshark (para monitorar os pacotes)
- Conexão Wi-Fi local

---

### Referências

- [Documentação Pico SDK - Networking](https://www.raspberrypi.com/documentation/pico-sdk/networking.html)  
- [lwIP MQTT API](https://www.nongnu.org/lwip/2_1_x/group__mqtt.html)  
- [Mosquitto MQTT Broker](https://mosquitto.org/)

---

## 📜 Licença
GNU GPL-3.0.
