/*
 * ============================================
 * NIVEL 5: SNIFFER COMPLETO CON COMANDOS
 * ============================================
 * 
 * OBJETIVO: Sniffer profesional e interactivo
 * - Comandos por Serial (start, stop, cambiar canal)
 * - Control de flujo con variables de estado
 * - Sistema completo funcional
 * 
 * CONCEPTOS NUEVOS:
 * - Entrada de usuario por Serial
 * - Máquina de estados (escaneando/detenido)
 * - Comandos de texto
 * 
 * EQUIVALENTE EN PYTHON:
 * comando = input().strip().lower()
 * if comando == "stop":
 *     detener_escaneo()
 * elif comando.isdigit():
 *     cambiar_canal(int(comando))
 * 
 * ¡Este es el código completo y funcional!
 */

#include <WiFi.h>
#include "esp_wifi.h"

// ==========================================
// ESTRUCTURA Y CONFIGURACIÓN
// ==========================================
struct Red {
  String mac;
  String ssid;
  int rssi;
  unsigned long ultima_vez;
};

#define MAX_REDES 50
#define INTERVALO_REFRESCO 2000
#define TIMEOUT_RED 15000

Red redes[MAX_REDES];
int total_redes = 0;
int paquetes_capturados = 0;

int canal = 0;
bool escaneando = false;  // ✨ Variable de estado
unsigned long ultimo_refresco = 0;

// ==========================================
// FUNCIONES AUXILIARES
// ==========================================

int buscarRedPorMAC(String mac) {
  for (int i = 0; i < total_redes; i++) {
    if (redes[i].mac == mac) return i;
  }
  return -1;
}

void limpiarRedesAntiguas() {
  unsigned long ahora = millis();
  for (int i = total_redes - 1; i >= 0; i--) {
    if (ahora - redes[i].ultima_vez > TIMEOUT_RED) {
      for (int j = i; j < total_redes - 1; j++) {
        redes[j] = redes[j + 1];
      }
      total_redes--;
    }
  }
}

void mostrarTabla() {
  for (int i = 0; i < 3; i++) Serial.println();
  
  Serial.println("╔══════════════════════════════════════════════════════════════════════╗");
  Serial.printf("║  SNIFFER WiFi - Canal %d | Redes: %d | Beacons: %d", 
                canal, total_redes, paquetes_capturados);
  
  int len = 35 + String(canal).length() + String(total_redes).length() + String(paquetes_capturados).length();
  for (int i = 0; i < 70 - len; i++) Serial.print(" ");
  Serial.println("║");
  
  Serial.println("╠══════════════════════════════════════════════════════════════════════╣");
  Serial.println("║  #  │ SSID (Red WiFi)          │ MAC Address       │ Señal (dBm)  ║");
  Serial.println("╠══════════════════════════════════════════════════════════════════════╣");
  
  if (total_redes == 0) {
    Serial.println("║                       ⏳ Esperando redes...                          ║");
  } else {
    for (int i = 0; i < total_redes; i++) {
      String ssid_display = redes[i].ssid;
      if (ssid_display.length() > 24) {
        ssid_display = ssid_display.substring(0, 21) + "...";
      }
      
      Serial.printf("║ %2d  │ %-24s │ %s │ %4d dBm     ║\n",
                    i + 1,
                    ssid_display.c_str(),
                    redes[i].mac.c_str(),
                    redes[i].rssi);
    }
  }
  
  Serial.println("╚══════════════════════════════════════════════════════════════════════╝");
  Serial.println("💡 Comandos: 's'=detener | 'r'=reiniciar | 1-13=cambiar canal");
  Serial.println();
}

void agregarOActualizarRed(String mac, String ssid, int rssi) {
  int indice = buscarRedPorMAC(mac);
  unsigned long ahora = millis();
  
  if (indice >= 0) {
    redes[indice].rssi = rssi;
    redes[indice].ultima_vez = ahora;
  } else {
    if (total_redes < MAX_REDES) {
      redes[total_redes].mac = mac;
      redes[total_redes].ssid = ssid;
      redes[total_redes].rssi = rssi;
      redes[total_redes].ultima_vez = ahora;
      total_redes++;
    }
  }
}

// ==========================================
// CALLBACK: CAPTURA DE PAQUETES
// ==========================================
void capturar_paquete(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;
  
  wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
  uint8_t *payload = paquete->payload;
  
  uint8_t fc = payload[0];
  uint8_t tipo = (fc >> 2) & 0x03;
  uint8_t subtipo = (fc >> 4) & 0x0F;
  
  if (tipo == 0 && subtipo == 8) {
    char mac[18];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    int len_ssid = payload[37];
    String ssid = "";
    
    if (len_ssid > 0 && len_ssid < 33) {
      for (int i = 0; i < len_ssid; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "<oculto>";
    }
    
    int rssi = paquete->rx_ctrl.rssi;
    
    agregarOActualizarRed(String(mac), ssid, rssi);
    paquetes_capturados++;
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║  NIVEL 5: Sniffer WiFi Completo     ║");
  Serial.println("╚══════════════════════════════════════╝\n");
  
  Serial.println("📋 Canales WiFi disponibles:");
  Serial.println("   • Canales 1-13: WiFi 2.4 GHz");
  Serial.println("   • Canales populares: 1, 6, 11");
  Serial.println("\n💡 Comandos disponibles:");
  Serial.println("   • Número (1-13): Escanear ese canal");
  Serial.println("   • 's' o 'stop': Detener escaneo");
  Serial.println("   • 'r' o 'reset': Reiniciar");
  Serial.println();
}

// ==========================================
// LOOP CON COMANDOS INTERACTIVOS
// ==========================================
void loop() {
  // ==========================================
  // ACTUALIZACIÓN AUTOMÁTICA (si está escaneando)
  // ==========================================
  if (escaneando) {
    unsigned long ahora = millis();
    
    if (ahora - ultimo_refresco >= INTERVALO_REFRESCO) {
      limpiarRedesAntiguas();
      mostrarTabla();
      ultimo_refresco = ahora;
    }
  }
  
  // ==========================================
  // ✨ PROCESAMIENTO DE COMANDOS
  // ==========================================
  if (Serial.available() > 0) {
    // Leer comando del usuario
    String comando = Serial.readStringUntil('\n');
    comando.trim();           // Eliminar espacios
    comando.toLowerCase();    // A minúsculas
    
    // ==========================================
    // COMANDO: DETENER (s/stop)
    // ==========================================
    if (comando == "s" || comando == "stop") {
      if (escaneando) {
        esp_wifi_set_promiscuous(false);
        
        Serial.println("\n⏸️  ESCANEO DETENIDO");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf("📊 Estadísticas finales:\n");
        Serial.printf("   • Canal: %d\n", canal);
        Serial.printf("   • Redes únicas: %d\n", total_redes);
        Serial.printf("   • Beacons capturados: %d\n", paquetes_capturados);
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        Serial.println("💡 Escribe 'r' para reiniciar o un canal (1-13)\n");
        
        escaneando = false;
      } else {
        Serial.println("⚠️  El escaneo ya está detenido\n");
      }
    }
    
    // ==========================================
    // COMANDO: REINICIAR (r/reset)
    // ==========================================
    else if (comando == "r" || comando == "reset") {
      if (escaneando) {
        esp_wifi_set_promiscuous(false);
      }
      
      Serial.println("\n🔄 Reiniciando...\n");
      
      escaneando = false;
      paquetes_capturados = 0;
      total_redes = 0;
      canal = 0;
      
      Serial.println("✅ Listo para escanear");
      Serial.println("💡 Escribe un canal (1-13) para comenzar\n");
    }
    
    // ==========================================
    // COMANDO: NÚMERO DE CANAL (1-13)
    // ==========================================
    else {
      int nuevo_canal = comando.toInt();
      
      if (nuevo_canal >= 1 && nuevo_canal <= 13) {
        if (escaneando) {
          esp_wifi_set_promiscuous(false);
          Serial.printf("\n⏹️  Deteniendo canal %d...\n", canal);
          delay(100);
        }
        
        canal = nuevo_canal;
        paquetes_capturados = 0;
        total_redes = 0;
        
        Serial.printf("\n✅ Canal %d seleccionado\n", canal);
        Serial.println("🔧 Configurando ESP32...\n");
        
        WiFi.mode(WIFI_STA);
        delay(100);
        WiFi.disconnect();
        delay(100);
        
        esp_wifi_set_promiscuous(true);
        delay(100);
        esp_wifi_set_promiscuous_rx_cb(&capturar_paquete);
        esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
        
        Serial.println("✅ Sniffer activado");
        Serial.printf("📡 Escuchando canal %d...\n", canal);
        Serial.println("⏳ Capturando redes...\n");
        
        escaneando = true;
        ultimo_refresco = millis();
      } else {
        Serial.println("❌ Comando inválido");
        Serial.println("💡 Usa: 1-13, 's' (stop), 'r' (reset)\n");
      }
    }
  }
  
  delay(10);
}

/*
 * ============================================
 * EXPLICACIÓN COMPLETA
 * ============================================
 * 
 * ¿CÓMO FUNCIONA EL SISTEMA DE COMANDOS?
 * 
 * 1. Serial.available() verifica si hay datos
 * 2. Serial.readStringUntil('\n') lee hasta Enter
 * 3. comando.trim() elimina espacios
 * 4. comando.toLowerCase() convierte a minúsculas
 * 5. if/else-if decide qué hacer
 * 
 * Equivalente Python:
 * if input_disponible():
 *     comando = input().strip().lower()
 *     if comando == "stop":
 *         detener()
 *     elif comando.isdigit():
 *         canal = int(comando)
 * 
 * ¿QUÉ ES UNA MÁQUINA DE ESTADOS?
 * - Variable que indica el "estado" actual
 * - En este caso: escaneando (True/False)
 * - Controla qué acciones son válidas
 * 
 * Estados posibles:
 * - escaneando = false → puede iniciar
 * - escaneando = true → puede detener
 * 
 * ¿POR QUÉ USAR String.toInt()?
 * - Convierte texto a número
 * - "6" → 6
 * - "abc" → 0 (retorna 0 si no es número)
 * 
 * FLUJO COMPLETO DEL PROGRAMA:
 * 
 * 1. Usuario escribe "6"
 * 2. Serial.available() > 0 → hay datos
 * 3. comando = "6"
 * 4. nuevo_canal = 6
 * 5. Validar: 6 está entre 1-13 ✓
 * 6. Activar modo promiscuo
 * 7. Registrar callback
 * 8. Establecer canal 6
 * 9. escaneando = true
 * 10. Loop actualiza tabla cada 2 seg
 * 11. Callback captura beacons automáticamente
 * 12. Usuario escribe "s"
 * 13. Desactivar modo promiscuo
 * 14. escaneando = false
 * 15. Loop ya no actualiza tabla
 * 
 * MEJORAS POSIBLES:
 * - Escanear todos los canales automáticamente
 * - Guardar redes en archivo SD
 * - Detectar tipo de seguridad (WPA, WPA2, etc.)
 * - Mostrar gráficos de señal
 * - Enviar datos por WiFi a un servidor
 * - Detectar dispositivos conectados
 * 
 * ¡FELICIDADES!
 * Has completado el tutorial de WiFi Sniffer.
 * Ahora entiendes cómo funciona el modo promiscuo,
 * cómo capturar y decodificar paquetes WiFi,
 * y cómo crear programas interactivos en C/Arduino.
 */
