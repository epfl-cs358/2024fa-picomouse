import pygame
from sys import exit

SCREEN_WIDTH, SCREEN_HEIGHT = 1000, 1000
CELL_SIZE = 125

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('PicoMouse')
clock = pygame.time.Clock()

bg_surface = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
bg_surface.fill('White')

undisc_cell = pygame.image.load('ui/images/no_walls.png').convert()
final_top_l = pygame.image.load('ui/images/fin_topL.png').convert()
final_top_r = pygame.image.load('ui/images/fin_topR.png').convert()
final_bot_l = pygame.image.load('ui/images/fin_botL.png').convert()
final_bot_r = pygame.image.load('ui/images/fin_botR.png').convert()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

    for i in range (0, 8):
        for j in range (0, 8):
            screen.blit(undisc_cell, (i*CELL_SIZE, j*CELL_SIZE))
            if (i == 6):
                if (j == 6):
                    screen.blit(final_top_l, (i*CELL_SIZE, j*CELL_SIZE))
                elif (j == 7):
                    screen.blit(final_bot_l, (i*CELL_SIZE, j*CELL_SIZE))
            elif (i == 7):
                if (j == 6):
                    screen.blit(final_top_r, (i*CELL_SIZE, j*CELL_SIZE))
                elif (j == 7):
                    screen.blit(final_bot_r, (i*CELL_SIZE, j*CELL_SIZE))
    pygame.display.update()
    clock.tick(60)