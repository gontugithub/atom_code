# 🤖 ATOM_CODE: Sistema de Control Robótico

Proyecto de robótica modular para la asignatura de taller I, que integra control remoto mediante mando de PlayStation (PS4/PS5), comunicación Bluetooth con modulo HC-05 y identificación del robot rival para sumo

---

## 📁 Estructura del Repositorio

```
atom_code/
├── control_remote/
│   ├── conexion_bluetooth.py      # Script pricipal
│   └── control_nano.ino           # Firmware para Arduino (Nano/Uno)
├── line_follower.ino              # Seguidor de líneas con sensores QTR
├── sumo_bot.ino                   # Firmware especializado para combate
├── sumo_bot_2xy.ino               # Variante de sumo bot
├── requirements.txt               # Dependencias de Python
├── Readme.md                      # Este archivo
└── .gitignore                     # Archivos a excluir del repositorio
```

### Descripción de Archivos

| Archivo | Descripción |
|---------|-------------|
| `control_remote/conexion_bluetooth.py` | Script en Python para procesar entrada del mando PS4/PS5 y transmitir vía Bluetooth |
| `control_remote/control_nano.ino` | Firmware para Arduino que gestiona motores y sensores |
| `line_follower.ino` | Lógica para seguidor de líneas utilizando sensores QTR |
| `sumo_bot.ino` | Firmware especializado para combate de sumo robótico |
| `sumo_bot_2xy.ino` | Variante mejorada de sumo bot |
| `requirements.txt` | Librerías de Python necesarias |

---

## 🛠️ Configuración del Software (PC con Linux)

### 1. Preparar el Entorno de Python

La carpeta `.venv` no se incluye en el repositorio. Para recrearla:

```bash
# Crear el entorno virtual
python3 -m venv .venv

# Activarlo
source .venv/bin/activate

# Instalar dependencias
pip install -r requirements.txt
```

### 2. Configurar el Enlace Bluetooth (HC-05)

Para que Linux reconozca el módulo como un puerto serie:

**Vincular el dispositivo:**
```bash
bluetoothctl trust 00:00:13:01:92:98
bluetoothctl pair 00:00:13:01:92:98
# Usar PIN 1234 si se solicita en la terminal
```

**Mapear puerto serie:**
```bash
sudo rfcomm bind 0 00:00:13:01:92:98
sudo chmod 666 /dev/rfcomm0
```

---

## 🔌 Conexiones de Hardware (Pinout)

### Motores & Bluetooth

| Componente | Pin Arduino | Función |
|-----------|-------------|---------|
| HC-05 TXD | 2 | RX Software Serial |
| HC-05 RXD | 13 | TX Software Serial |
| L298N ENA | D3 | Velocidad Motor A (PWM) |
| L298N IN1/IN2 | D4 / D5 | Dirección Motor A |
| L298N ENB | D9 | Velocidad Motor B (PWM) |
| L298N IN3/IN4 | D8 / D7 | Dirección Motor B |

### Sensor Ultrasónico (HC-SR04)

| Sensor | Pin Arduino | Función |
|--------|-------------|---------|
| VCC | 5V | Alimentación |
| Trig | D11 | Disparo |
| Echo | D12 | Eco |
| GND | GND | Tierra |

---

## 🚀 Ejecución del Proyecto

### Paso 1: Cargar el código en Arduino
Sube `control_remote/sumo_bot2xy.ino` a tu placa Arduino mediante el IDE de Arduino.

### Paso 2: Conectar el mando
Enchufa tu mando de PlayStation (PS4/PS5) vía USB a la PC.

### Paso 3: Iniciar el control remoto
```bash
source .venv/bin/activate
python3 control_remote/conexion_bluetooth.py
```

---

## 📋 Requisitos

- **Hardware:**
  - Arduino Nano/Uno
  - Módulo Bluetooth HC-05
  - Driver Motor L298N
  - Sensor Ultrasónico HC-SR04
  - Sensores QTR pololu 8 sensores (para line_follower)
  - Mando de PlayStation (PS4/PS5)

- **Software:**
  - Python 3.7+
  - Arduino IDE
  - Librerías de Python (ver `requirements.txt`)

---

## 🎮 Características Principales

- ✅ Control remoto con mando de PlayStation
- ✅ Comunicación Bluetooth HC-05
- ✅ Evasión de obstáculos con sensores ultrasónicos
- ✅ Seguidor de líneas automático
- ✅ Modo combate sumo robótico
- ✅ Gestión modular del código

---

## 📝 Notas Importantes

- El entorno virtual (`.venv`) debe crearse localmente en cada PC
- Para usar Bluetooth en Linux, asegúrate de tener permisos de usuario sobre los puertos
- El PIN del módulo HC-05 por defecto es **1234**
- La dirección MAC puede variar según tu dispositivo Bluetooth
