import serial
import RPi.GPIO as GPIO
import time
import pygame
import sys


# Initialize Pygame
pygame.init()

screen_width = 640
screen_height = 480
screen = pygame.display.set_mode((screen_width, screen_height))
x = 0
y = 0
ser=serial.Serial("/dev/ttyACM0",9600)  #change ACM number as found from ls /dev/tty/ACM*
ser.baudrate=9600
def blink(pin):
	GPIO.output(pin,GPIO.HIGH)  
	time.sleep(1)  
	GPIO.output(pin,GPIO.LOW)  
	time.sleep(1)  
	return

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
# Set up the game loop
clock = pygame.time.Clock()
while True:
	read_ser=ser.readline()
	print(read_ser)
	if(read_ser=="Hello From Arduino!"):
		blink(11)
	for event in pygame.event.get():
        	if event.type == pygame.QUIT:
            		pygame.quit()
            		sys.exit()
	x += 1
	y += 1
	# Render the graphics
	screen.fill((255, 255, 255))
	pygame.draw.rect(screen, (0, 0, 255), (x, y, 200, 200))
	# Update the display
	pygame.display.flip()
	# Cap the frame rate
	clock.tick(60)
