# 🎓 Conceptos Fundamentales de WiFi

Esta guía explica los conceptos básicos de WiFi necesarios para entender el proyecto.

---

## 📡 ¿Qué es WiFi?

WiFi (Wireless Fidelity) es una tecnología que permite la transmisión de datos de forma inalámbrica usando ondas de radio.

### Estándar 802.11
WiFi está basado en el estándar IEEE 802.11:
- **802.11b/g/n**: 2.4 GHz (usado en este proyecto)
- **802.11a/n/ac/ax**: 5 GHz
- **802.11ax (WiFi 6)**: 2.4 GHz y 5 GHz

---

## 🌊 Canales WiFi 2.4 GHz

La banda de 2.4 GHz está dividida en **13 canales** (1-13):

| Canal | Frecuencia | Uso Común |
|-------|-----------|-----------|
| 1 | 2.412 GHz | ⭐ Muy usado |
| 6 | 2.437 GHz | ⭐ Muy usado |
| 11 | 2.462 GHz | ⭐ Muy usado |
| 2-5 | 2.417-2.432 GHz | Menos común |
| 7-10 | 2.442-2.457 GHz | Menos común |
| 12-13 | 2.467-2.472 GHz | No en USA |

### ¿Por qué 1, 6 y 11?
Son los únicos canales que **NO se solapan** entre sí, reduciendo interferencias.

---

## 📦 Tipos de Paquetes WiFi

### Management Frames (Gestión)
Controlan la conexión WiFi:
- **Beacon**: Anuncia la red (este proyecto los captura)
- **Probe Request**: Dispositivo busca redes
- **Probe Response**: Router responde a búsqueda
- **Authentication**: Solicitud de autenticación
- **Association**: Conexión a la red

### Control Frames (Control)
Controlan el acceso al medio:
- **RTS** (Request to Send)
- **CTS** (Clear to Send)
- **ACK** (Acknowledgement)

### Data Frames (Datos)
Contienen los datos reales de usuario.

---

## 📶 Beacon Frames

Los **beacons** son paquetes especiales que:
- Se transmiten ~10 veces por segundo
- Anuncian la existencia de la red
- NO están encriptados (siempre visibles)
- Contienen información de la red

### Información en un Beacon:
```
┌─────────────────────────────────┐
│ SSID: "MiWiFi"                  │
│ MAC (BSSID): aa:bb:cc:dd:ee:ff  │
│ Canal: 6                         │
│ Seguridad: WPA2                  │
│ Velocidades soportadas           │
│ Intervalo de beacon: 100ms       │
└─────────────────────────────────┘
```

---

## 🔑 Direcciones MAC (BSSID)

### ¿Qué es una MAC?
- **M**edia **A**ccess **C**ontrol Address
- Identificador único de cada dispositivo de red
- 48 bits (6 bytes) en formato hexadecimal
- Ejemplo: `a4:91:b1:2c:3d:4e`

### Formato:
```
a4:91:b1:2c:3d:4e
│  │  │  │  │  │
│  │  │  └──┴──┴─ Device ID (últimos 3 bytes)
└──┴──┴───────── Manufacturer ID (primeros 3 bytes)
```

### BSSID vs SSID:
- **SSID**: Nombre de la red ("MiWiFi") - puede repetirse
- **BSSID**: MAC del router - ÚNICO y no cambia

---

## 📊 RSSI (Señal)

**R**eceived **S**ignal **S**trength **I**ndicator

Mide la potencia de la señal WiFi en **dBm** (decibelios miliwatt):

| RSSI | Calidad | Descripción |
|------|---------|-------------|
| -30 dBm | ⭐⭐⭐⭐⭐ | Excelente |
| -50 dBm | ⭐⭐⭐⭐ | Muy buena |
| -60 dBm | ⭐⭐⭐ | Buena |
| -70 dBm | ⭐⭐ | Regular |
| -80 dBm | ⭐ | Débil |
| -90 dBm | ❌ | Muy débil/No usable |

### ¿Por qué valores negativos?
Porque se mide en escala logarítmica. Más cercano a 0 = mejor señal.

---

## 🔓 Modo Promiscuo

### Modo Normal
Tu WiFi solo escucha paquetes dirigidos a **TU MAC**.

```
Paquetes en el aire:
📦 Para: aa:bb:cc:dd:ee:ff (TÚ) ✅ Capturado
📦 Para: 11:22:33:44:55:66      ❌ Ignorado
📦 Para: 77:88:99:aa:bb:cc      ❌ Ignorado
```

### Modo Promiscuo (Monitor Mode)
Tu WiFi escucha **TODOS** los paquetes en el canal actual.

```
Paquetes en el aire:
📦 Para: aa:bb:cc:dd:ee:ff ✅ Capturado
📦 Para: 11:22:33:44:55:66 ✅ Capturado
📦 Para: 77:88:99:aa:bb:cc ✅ Capturado
```

### Analogía:
- **Modo Normal**: Solo escuchas cuando dicen tu nombre
- **Modo Promiscuo**: Escuchas TODAS las conversaciones en la habitación

---

## 🔍 ¿Qué puede ver un Sniffer?

### ✅ Puede ver:
- Nombres de redes (SSID)
- Direcciones MAC (BSSID)
- Canales usados
- Tipo de seguridad (WPA, WPA2, WEP)
- Señal de cada red
- Dispositivos conectados (con técnicas avanzadas)

### ❌ NO puede ver:
- Contraseñas WiFi (están encriptadas)
- Contenido de los datos (si hay encriptación WPA2)
- Información dentro de paquetes encriptados

---

## 🛡️ Tipos de Seguridad WiFi

| Tipo | Año | Seguridad | Estado |
|------|-----|-----------|--------|
| **Abierta** | - | ❌ Ninguna | Peligroso |
| **WEP** | 1999 | ❌ Débil | Obsoleto |
| **WPA** | 2003 | ⚠️ Media | Obsoleto |
| **WPA2** | 2004 | ✅ Fuerte | Actual |
| **WPA3** | 2018 | ⭐ Muy fuerte | Moderno |

---

## 📖 Términos Importantes

### AP (Access Point)
El **router WiFi** que crea la red.

### STA (Station)
Un **dispositivo cliente** (tu laptop, móvil, etc.)

### SSID
**S**ervice **S**et **ID**entifier - El nombre de la red.

### Hidden SSID
Red que no transmite su nombre en los beacons (aparece como `<oculto>`).

### Frame Control
Byte en el paquete que indica el **tipo y subtipo** de frame.

---

## 🔬 Estructura de un Paquete WiFi

```
┌──────────────────────────────────────────┐
│ Frame Control (2 bytes)                  │  ← Tipo de paquete
├──────────────────────────────────────────┤
│ Duration (2 bytes)                       │
├──────────────────────────────────────────┤
│ Address 1 (6 bytes) - Destino           │
├──────────────────────────────────────────┤
│ Address 2 (6 bytes) - Origen            │
├──────────────────────────────────────────┤
│ Address 3 (6 bytes) - BSSID             │  ← MAC del router
├──────────────────────────────────────────┤
│ Sequence Control (2 bytes)               │
├──────────────────────────────────────────┤
│ Payload (datos)                          │  ← SSID y otros datos
└──────────────────────────────────────────┘
```

---

## 💡 Preguntas Frecuentes

### ¿Por qué solo 2.4 GHz y no 5 GHz?
El ESP32 solo soporta WiFi 2.4 GHz. Para 5 GHz se necesita hardware diferente.

### ¿Puedo ver contraseñas WiFi?
**NO**. Las contraseñas están protegidas por el proceso de autenticación WPA2/WPA3.

### ¿Es legal hacer sniffing?
Depende de tu país. Generalmente es legal si:
- Es tu propia red
- Tienes autorización explícita
- Es en un entorno educativo controlado

### ¿Cuál es la diferencia con Wireshark?
Wireshark es más completo y analiza TODO el tráfico. Este sniffer solo muestra beacons (redes disponibles).

---

<div align="center">

**[← Volver al Tutorial](../tutorial/README.md)**

</div>
