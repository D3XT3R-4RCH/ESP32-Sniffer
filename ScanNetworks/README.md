# 🛡️ ESP32 Pentesting - Herramientas de Seguridad WiFi

> Suite de herramientas educativas para auditorías de seguridad WiFi usando ESP32

[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/license-MIT-orange.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-yellow.svg)](https://en.wikipedia.org/wiki/C%2B%2B)

---

## 📖 Descripción

Repositorio educativo que contiene herramientas de pentesting WiFi implementadas en ESP32. Diseñado para aprender seguridad de redes inalámbricas, programación en C/Arduino y conceptos de networking.

**⚠️ ADVERTENCIA**: Uso exclusivamente educativo. Solo en redes propias o con autorización explícita.

---

## 🎯 Proyectos Incluidos

### 📡 WiFi Sniffer Tutorial
**Tutorial progresivo de 5 niveles** para aprender a crear un sniffer WiFi desde cero.

**Características:**
- ✅ Captura de beacons WiFi
- ✅ Detección de redes sin duplicados
- ✅ Tabla actualizable en tiempo real
- ✅ Control interactivo por comandos
- ✅ Eliminación automática de redes antiguas

**[📚 Ver Tutorial Completo →](./tutorial/)**

**Niveles:**
1. **Hola Mundo WiFi** - Inicialización básica
2. **Capturar Beacons** - Primer sniffer funcional
3. **Sin Duplicados** - Lista de redes únicas
4. **Tabla Actualizable** - Display profesional
5. **Sniffer Completo** - Sistema interactivo completo

---

## 🚀 Inicio Rápido

### Requisitos Previos

**Hardware:**
- ESP32 (cualquier modelo: WROOM-32, DevKit V1, etc.)
- Cable USB para programación

**Software:**
- [Arduino IDE](https://www.arduino.cc/en/software) 1.8.x o 2.x
- Driver USB-Serial (CP210x o CH340)
- Soporte ESP32 para Arduino

### Instalación Rápida

1. **Instalar Arduino IDE** desde https://www.arduino.cc/en/software

2. **Agregar soporte ESP32:**
   - Abrir Arduino IDE
   - `Archivo` → `Preferencias`
   - En "URLs de gestor de placas adicionales":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - `Herramientas` → `Placa` → `Gestor de placas`
   - Buscar "ESP32" e instalar

3. **Seleccionar placa:**
   - `Herramientas` → `Placa` → `ESP32 Dev Module`
   - `Herramientas` → `Puerto` → (seleccionar tu puerto COM)

4. **Abrir tutorial:**
   - `Archivo` → `Abrir` → `tutorial/01_hola_wifi/01_hola_wifi.ino`

---

## 📚 Estructura del Proyecto

```
ScanNetworks/
│
├── README.md                      # Este archivo
│
├── tutorial/                      # 📚 Tutorial progresivo (RECOMENDADO)
│   ├── README.md                  # Guía completa
│   ├── 01_hola_wifi/              # Nivel 1: Básico
│   ├── 02_capturar_beacons/       # Nivel 2: Captura
│   ├── 03_sin_duplicados/         # Nivel 3: Filtrado
│   ├── 04_tabla_actualizable/     # Nivel 4: Display
│   └── 05_sniffer_completo/       # Nivel 5: Completo
│
├── sniffer_v1/                    # Versión simple inicial
└── sniffer_v2/                    # Versión avanzada actual
```

---

## 🎓 ¿Por dónde empezar?

### Para Principiantes
Si eres nuevo en C/Arduino o microcontroladores:
👉 **Empieza por el [Tutorial Nivel 1](./tutorial/)**

Aprenderás paso a paso:
- Conceptos básicos de C
- Cómo funciona WiFi
- Programación de ESP32
- Comparaciones con Python

### Para Usuarios Avanzados
Si ya conoces C/Arduino:
👉 **Ve directo al [Nivel 5 - Sniffer Completo](./tutorial/05_sniffer_completo/)**

---

## 🔧 Características del WiFi Sniffer

| Característica | Descripción |
|---|---|
| **Captura de Beacons** | Detecta todas las redes WiFi cercanas |
| **Sin Duplicados** | Muestra cada red solo una vez (por MAC) |
| **Actualización Automática** | Tabla que se refresca cada 2 segundos |
| **Limpieza Automática** | Elimina redes que no se ven en 15 seg |
| **Control Interactivo** | Comandos: start, stop, cambiar canal |
| **Todos los Canales** | Soporta canales 1-13 (2.4 GHz) |

### Comandos Disponibles

```
6        → Escanear canal 6
s/stop   → Detener escaneo
r/reset  → Reiniciar
1-13     → Cambiar a otro canal
```

### Ejemplo de Salida

```
╔══════════════════════════════════════════════════════════════════════╗
║  SNIFFER WiFi - Canal 6 | Redes: 5 | Beacons: 142                   ║
╠══════════════════════════════════════════════════════════════════════╣
║  #  │ SSID (Red WiFi)          │ MAC Address       │ Señal (dBm)  ║
╠══════════════════════════════════════════════════════════════════════╣
║  1  │ MiWiFi_Casa              │ a4:91:b1:2c:3d:4e │  -45 dBm     ║
║  2  │ Vecino_5G                │ c8:3a:35:11:22:33 │  -67 dBm     ║
║  3  │ <oculto>                 │ 00:11:22:33:44:55 │  -72 dBm     ║
╚══════════════════════════════════════════════════════════════════════╝
```

---

## 🛠️ Solución de Problemas

### El ESP32 no se detecta
- Instalar drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- Probar otro cable USB
- Presionar botón **BOOT** al subir código

### No se capturan paquetes
- Verificar velocidad serial: **115200 baudios**
- Probar canal 6 u 11
- Presionar botón **RESET** en el ESP32

### Errores de compilación
- Actualizar ESP32 board manager
- Verificar que la placa sea "ESP32 Dev Module"
- Asegurarse de que solo hay un archivo .ino por carpeta

---

## 📊 Comparación: Python vs C/Arduino

| Concepto Python | Equivalente C/Arduino |
|---|---|
| `print("Hola")` | `Serial.println("Hola");` |
| `time.sleep(1)` | `delay(1000);` |
| `def funcion():` | `void funcion() { }` |
| `lista = []` | `Array lista[10];` |
| `if condicion:` | `if (condicion) { }` |
| `for i in range(10):` | `for (int i=0; i<10; i++)` |

---

## ⚖️ Consideraciones Legales

⚠️ **IMPORTANTE**:
- Proyecto exclusivamente **educativo**
- Solo usar en **redes propias** o con autorización
- Capturar tráfico ajeno puede ser **ilegal**
- El autor **NO se responsabiliza** por mal uso

---

## 🤝 Contribuir

¡Las contribuciones son bienvenidas!

1. Fork el repositorio
2. Crear rama: `git checkout -b feature/mejora`
3. Commit: `git commit -m 'Agrega mejora X'`
4. Push: `git push origin feature/mejora`
5. Abrir Pull Request

---

## 📚 Recursos Adicionales

### Documentación
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [WiFi API Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

### Herramientas Relacionadas
- [Wireshark](https://www.wireshark.org/) - Análisis de tráfico
- [Aircrack-ng](https://www.aircrack-ng.org/) - Suite WiFi
- [Kismet](https://www.kismetwireless.net/) - Detector de redes

---

## 📜 Licencia

Este proyecto está bajo la **Licencia MIT**.

```
MIT License - Copyright (c) 2025
Se permite uso, copia y modificación con las condiciones del MIT.
```

---

## 👨‍💻 Autor

**Jose**  
Creado con ❤️ para la comunidad de aprendizaje  
Año: 2025

---

<div align="center">

**¿Te gustó el proyecto? ¡Dale una ⭐!**

[📚 Tutorial](./tutorial/) • [🐛 Issues](../../issues) • [💡 Discussions](../../discussions)

</div>
