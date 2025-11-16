import paho.mqtt.client as mqtt
import time
import random

# Configuración del broker y puerto
MQTT_BROKER = "mqtt.rendical.com"  # Reemplaza con la dirección de tu broker
MQTT_PORT = 1883                   # Puerto MQTT estándar (o el que use tu broker)

# Si tu broker requiere autenticación
USERNAME = "idat@scada.com"
PASSWORD = "idat@scada.com"
CLIENT_ID = "Colab"

# Topicos utilizados
# --- TOPICOS Y RANGOS DE VALORES ---
# Usamos un diccionario para gestionar la información de cada tópico
MEDIDORES = {
    "VOLTAJE": {
        "topic": "idat/medidor/voltaje",
        "min": 216,
        "max": 230,
        "unit": "V"
    },
    "CORRIENTE": {
        "topic": "idat/medidor/corriente",
        "min": 5,
        "max": 15,
        "unit": "A"
    },
    "POTENCIA": {
        "topic": "idat/medidor/potencia",
        "min": 1000,
        "max": 3500,
        "unit": "W"
    },
    "FRECUENCIA": {
        "topic": "idat/medidor/frecuencia",
        "min": 59,
        "max": 61,
        "unit": "Hz"
    },
    "TEMPERATURA": {
        "topic": "idat/medidor/temperatura",
        "min": 20,
        "max": 35,
        "unit": "°C"
    }
}

# --- CONTROL EXTERNO ---
MQTT_TOPIC_CONTROL = "idat/control/multiplicador"
# Variable global que almacena el factor de multiplicación (inicialmente 1)
multiplication_factor = 1

# --- 1. Funciones de Callback ---

def on_connect(client, userdata, flags, rc):
    """Se llama cuando el cliente recibe una respuesta de conexión del broker."""
    global multiplication_factor
    if rc == 0:
        print("✅ Conectado al broker MQTT exitosamente.")
        # Suscribirse al tópico de control después de la conexión
        client.subscribe(MQTT_TOPIC_CONTROL)
        print(f"📡 Suscrito al tópico de control: {MQTT_TOPIC_CONTROL}")
        multiplication_factor = 1 # Asegura que se reinicie al conectar
    else:
        print(f"❌ Fallo en la conexión. Código de resultado: {rc}")

def on_message(client, userdata, msg):
    """Se llama cuando se recibe un mensaje en un tópico suscrito."""
    global multiplication_factor
    if msg.topic == MQTT_TOPIC_CONTROL:
        try:
            control_value = int(msg.payload.decode().strip()) # Lee y convierte el valor a entero

            if control_value == 1:
                multiplication_factor = 10
                print("\n\n🔥 ¡ALARMA! CONTROL RECIBIDO: Multiplicador establecido a x10. 🔥\n")
            else:
                multiplication_factor = 1
                print("\n\n🟢 CONTROL RECIBIDO: Multiplicador establecido a x1 (Normal).\n")

        except ValueError:
            print("❌ Mensaje de control inválido. Debe ser '1' o '0'.")

# --- 2. Lógica Principal de Publicación ---
def run_publisher():

    # Crea el cliente
    client = mqtt.Client(client_id=CLIENT_ID)

    # Asume autenticación (comentar si no se usa)
    client.username_pw_set(USERNAME, PASSWORD)

    # Asigna todas las funciones de callback
    client.on_connect = on_connect
    client.on_message = on_message # <--- AÑADIDO: Manejar mensajes entrantes

    try:
        # Conecta al broker
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_start()

        print("\nIniciando publicación de telemetría...")
        print("Presiona el botón de 'Stop' (cuadrado) para detener el proceso.")

        # Variables para almacenar V e I para el cálculo de Potencia
        last_voltage = 0
        last_current = 0

        # Bucle de publicación
        while True:
            print("-" * 25)
            # Itera sobre el diccionario de medidores para publicar en cada tópico
            for nombre, data in MEDIDORES.items():

                # 1. Generación/Cálculo del valor base (SIN MULTIPLICAR)
                value = 0

                if nombre == "VOLTAJE":
                    value = random.randint(data["min"], data["max"])
                    last_voltage = value # GUARDAR valor de V

                elif nombre == "CORRIENTE":
                    value = round(random.uniform(data["min"], data["max"]), 2)
                    last_current = value # GUARDAR valor de I

                elif nombre == "POTENCIA":
                    # CÁLCULO: P = V * I. Se garantiza que V e I ya fueron generados.
                    value = round(last_voltage * last_current, 2)

                elif nombre == "FRECUENCIA":
                    value = round(random.uniform(data["min"], data["max"]), 1)

                else: # TEMPERATURA
                    value = random.randint(data["min"], data["max"])


                # 2. APLICAR EL FACTOR DE MULTIPLICACIÓN
                final_value = round(value * multiplication_factor, 2)

                # 3. Publicación
                # Convierte el valor final a una cadena de texto (payload)
                payload = str(final_value)

                # Publica el mensaje
                result = client.publish(data["topic"], payload, qos=1)

                # Confirma la publicación
                status = result[0]
                if status == 0:
                    status_text = f"Publicado: {payload} {data.get('unit', '')}"
                    if multiplication_factor == 10:
                        status_text += " (x10)" # Etiqueta para indicar multiplicación
                    print(f"{status_text} | Tópico: {data['topic']}")
                else:
                    print(f"❌ Error al publicar {nombre}. Estado: {status}")

            # Espera 1 segundo para el próximo ciclo de publicación de todos los tópicos
            time.sleep(1)

    except KeyboardInterrupt:
        print("\nProceso de publicación detenido por el usuario.")
    except Exception as e:
        print(f"\nOcurrió un error: {e}")
    finally:
        # Detiene el hilo de red y desconecta
        client.loop_stop()
        client.disconnect()
        print("Desconectado del broker.")


# Ejecutar la función
run_publisher()
