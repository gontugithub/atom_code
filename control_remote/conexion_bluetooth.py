import pygame
import serial
import time

puerto_bt = '/dev/rfcomm0' 

try:
    arduino = serial.Serial(puerto_bt, 9600, timeout=1)
    print("¡Conectado al Bluetooth del Arduino!")
except:
    print(f"Error: No se pudo abrir {puerto_bt}. ¿Hiciste el sudo rfcomm bind?")
    exit()

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No se detecta el mando. Conéctalo por USB.")
    exit()

mando = pygame.joystick.Joystick(0)
mando.init()
print(f"Leyendo mando: {mando.get_name()}")

try:
    while True:
        pygame.event.pump()
        
        # Multiplicamos por 100 para que el Arduino reciba valores de -100 a 100
        # Invertimos el eje Y (multiplicando por -1) porque en los mandos "arriba" suele ser negativo
        eje_x = int(mando.get_axis(0) * 100)
        eje_y = int(mando.get_axis(1) * -100) 

        # Formato exacto que espera el Arduino: Xnn,Ynn\n
        datos = f"X{eje_x},Y{eje_y}\n"
        
        arduino.write(datos.encode())
        
        print(f"Enviando -> {datos}", end='')
        time.sleep(0.05) # 20 veces por segundo para que sea fluido

except KeyboardInterrupt:
    print("\nDeteniendo...")
    arduino.close()