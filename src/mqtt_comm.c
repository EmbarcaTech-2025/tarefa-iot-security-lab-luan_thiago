#include "lwip/apps/mqtt.h"
#include "include/mqtt_comm.h"
#include "lwipopts.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "include/xor_cipher.h"

static mqtt_client_t *client;
static uint32_t ultima_timestamp_recebida = 0;

// Função auxiliar para extrair valor e timestamp da mensagem JSON
static int parse_message(const char *msg, float *valor, uint32_t *timestamp) {
    return sscanf(msg, "{\"valor\":%f,\"ts\":%lu}", valor, timestamp);
}

// Callbacks
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("Mensagem recebida no tópico: %s (tamanho total: %u)\n", topic, tot_len);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    uint8_t decifrada[128];
    if (len >= sizeof(decifrada)) len = sizeof(decifrada) - 1;

    // Descriptografar com XOR
    xor_encrypt(data, decifrada, len, 42);
    decifrada[len] = '\0';

    // Parse do JSON da mensagem
    float valor;
    uint32_t nova_ts;
    if (sscanf((char *)decifrada, "{\"valor\":%f,\"ts\":%lu}", &valor, &nova_ts) != 2) {
        printf("Erro no parse da mensagem: %s\n", decifrada);
        return;
    }

    // Verificação contra replay
    if (nova_ts > ultima_timestamp_recebida) {
        ultima_timestamp_recebida = nova_ts;
        printf("Nova leitura: %.2f (ts: %lu)\n", valor, nova_ts);
        
    } else {
        printf("Replay detectado (ts: %lu <= %lu)\n", nova_ts, ultima_timestamp_recebida);
    }
}


static void mqtt_sub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Inscrição no tópico feita com sucesso.\n");
    } else {
        printf("Erro ao se inscrever no tópico: %d\n", result);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Conectado ao broker MQTT com sucesso!\n");
        // Configura os callbacks para mensagens recebidas
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);
    } else {
        printf("Falha ao conectar ao broker, código: %d\n", status);
    }
}

void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass) {
    ip_addr_t broker_addr;
    
    if (!ip4addr_aton(broker_ip, &broker_addr)) {
        printf("Erro no IP\n");
        return;
    }

    client = mqtt_client_new();
    if (client == NULL) {
        printf("Falha ao criar o cliente MQTT\n");
        return;
    }

    struct mqtt_connect_client_info_t ci = {
        .client_id = client_id,
        .client_user = user,
        .client_pass = pass
    };

    mqtt_client_connect(client, &broker_addr, 1883, mqtt_connection_cb, NULL, &ci);
}

void mqtt_comm_subscribe(const char *topic) {
    if (client == NULL) {
        printf("Cliente MQTT não inicializado\n");
        return;
    }

    // Registra os callbacks de publicação e dados recebidos
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);

    // Inscreve-se no tópico
    mqtt_subscribe(client, topic, 0, mqtt_sub_request_cb, NULL);
}


static void mqtt_pub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Publicação MQTT enviada com sucesso!\n");
    } else {
        printf("Erro ao publicar via MQTT: %d\n", result);
    }
}

void mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len) {
    if (client == NULL) {
        printf("Cliente MQTT não inicializado\n");
        return;
    }

    err_t status = mqtt_publish(
        client,
        topic,
        data,
        len,
        0,
        0,
        mqtt_pub_request_cb,
        NULL
    );

    if (status != ERR_OK) {
        printf("Publicação MQTT falhou ao ser enviada: %d\n", status);
    }
}