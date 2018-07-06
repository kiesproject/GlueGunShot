import serial
import sys
import pygame
import random

argv = sys.argv
argc = len(argv)

def read_serial(s):
    while True:

        line = s.readline()
        red = line[37:41]

        # print(red.strip())

        if(int(red,base=16) > 300):
            end = pygame.time.get_ticks() #count end

            pygame.mixer.music.load('tenaga.mp3') #sound(tenaga)
            pygame.mixer.music.play()

            # print screen
            sysfont = pygame.font.SysFont(None,80)
            screen = pygame.display.set_mode((400,300))
            pygame.display.set_caption("Clear Time!")
            screen.fill((255,255,255))
            screen.blit(sysfont.render(str((end-start)/1000)+"s",False,(0,0,0)),(90,150)) #end-start = Clear Time
            screen.blit(sysfont.render("Game!",False,(0,0,0)),(150,100))

        pygame.display.update()

if argc != 2:
    s = serial.Serial("/dev/ttyUSB0",115200,timeout=10)
else :
    s = serial.Serial(argv[1],115200,timeout=10)

pygame.init()

pygame.mixer.music.load('zebi.wav') #sound(zebi)
pygame.mixer.music.play()

pygame.time.wait(random.randint(10000,13000)) #stay(10s-13s)

pygame.mixer.music.load('kibi.mp3') #sound(kibi)
pygame.mixer.music.play()

start = pygame.time.get_ticks() #count start

while True:
    try:
        read_serial(s)
    except KeyboardInterrupt:
        break
    except:
        continue
s.close()
