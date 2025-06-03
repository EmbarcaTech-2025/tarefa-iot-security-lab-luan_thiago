#include <string.h>
#include <time.h>
#include <stdint.h> 
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "include/wifi_conn.h"
#include "include/mqtt_comm.h"
#include "include/xor_cipher.h"

int main() {
    stdio_init_all();

    // Conectar ao Wi-Fi
    connect_to_wifi("WIFI_SSID", "WIFI_PASSWORD");

    // Configurar o cliente MQTT
    mqtt_setup("bitdog1", "IP DO BROKER", "aluno", "senha123");


    //Adicione a seguinte linha ao código para usar a placa como subscriber
    //mqtt_comm_subscribe("escola/sala1/temperatura");

    // Loop principal
    while (true) {
        sleep_ms(5000);

        // Criar mensagem JSON com timestamp
        char mensagem[128];
        snprintf(mensagem, sizeof(mensagem), "{\"valor\":26.5,\"ts\":%lu}", time(NULL));

        // Criptografar a mensagem com XOR (chave 42)
        uint8_t criptografada[128];
        size_t len = strlen(mensagem);
        xor_encrypt((uint8_t *)mensagem, criptografada, len, 42);

        // Publicar a mensagem criptografada
        mqtt_comm_publish("escola/sala1/temperatura", criptografada, len);
    }

    return 0;
}
