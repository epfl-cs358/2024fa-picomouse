import pygame
from sys import exit

SCREEN_WIDTH, SCREEN_HEIGHT = 1000, 1000
CELL_SIZE = 125
LEFT, UP, RIGHT, DOWN = 1, 2, 3, 4

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('PicoMouse')
clock = pygame.time.Clock()

# BG
bg_surface = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
bg_surface.fill('White')

# Cells
undisc_cell = pygame.image.load('ui/images/no_walls.png').convert()
final_top_l = pygame.image.load('ui/images/fin_topL.png').convert()
final_top_r = pygame.image.load('ui/images/fin_topR.png').convert()
final_bot_l = pygame.image.load('ui/images/fin_botL.png').convert()
final_bot_r = pygame.image.load('ui/images/fin_botR.png').convert()
wall0000 = pygame.image.load('ui/images/wall0000.png').convert()
wall0001 = pygame.image.load('ui/images/wall0001.png').convert()
wall0010 = pygame.image.load('ui/images/wall0010.png').convert()
wall0011 = pygame.image.load('ui/images/wall0011.png').convert()
wall0100 = pygame.image.load('ui/images/wall0100.png').convert()
wall0101 = pygame.image.load('ui/images/wall0101.png').convert()
wall0110 = pygame.image.load('ui/images/wall0110.png').convert()
wall0111 = pygame.image.load('ui/images/wall0111.png').convert()
wall1000 = pygame.image.load('ui/images/wall1000.png').convert()
wall1001 = pygame.image.load('ui/images/wall1001.png').convert()
wall1010 = pygame.image.load('ui/images/wall1010.png').convert()
wall1011 = pygame.image.load('ui/images/wall1011.png').convert()
wall1100 = pygame.image.load('ui/images/wall1100.png').convert()
wall1101 = pygame.image.load('ui/images/wall1101.png').convert()
wall1110 = pygame.image.load('ui/images/wall1110.png').convert()

# Mouse
mouse_surface = pygame.image.load('ui/images/mouse.png').convert_alpha() # in a UP state
mouse_surface_up = mouse_surface
mouse_surface_left = pygame.transform.rotate(mouse_surface, 90)
mouse_surface_right = pygame.transform.rotate(mouse_surface, -90)
mouse_surface_down = pygame.transform.rotate(mouse_surface, 180)
mouse_rect = mouse_surface.get_rect(topleft = (0,0))
mouse_orientation = UP


def set_orientation(direction, current_center):
    if direction == UP:
        mouse_surface = mouse_surface_up
        mouse_rect = mouse_surface.get_rect(center = current_center)
        mouse_orientation = UP
    elif direction == LEFT:
        mouse_surface = mouse_surface_left
        mouse_rect = mouse_surface.get_rect(center = current_center)
        mouse_orientation = LEFT
    elif direction == RIGHT:
        mouse_surface = mouse_surface_right
        mouse_rect = mouse_surface.get_rect(center = current_center)
        mouse_orientation = RIGHT
    elif direction == DOWN:
        mouse_surface = mouse_surface_down
        mouse_rect = mouse_surface.get_rect(center = current_center)
        mouse_orientation = DOWN    
    return mouse_surface, mouse_rect, mouse_orientation

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                if mouse_rect.left != 0:
                    mouse_rect.x -= 125
                mouse_surface, mouse_rect, mouse_orientation = set_orientation(LEFT, mouse_rect.center)
            if event.key == pygame.K_UP:
                if mouse_rect.top != 0:
                    mouse_rect.y -= 125
                mouse_surface, mouse_rect, mouse_orientation = set_orientation(UP, mouse_rect.center)
            if event.key == pygame.K_RIGHT:
                if mouse_rect.right != SCREEN_WIDTH:
                    mouse_rect.x += 125
                mouse_surface, mouse_rect, mouse_orientation = set_orientation(RIGHT, mouse_rect.center)
            if event.key == pygame.K_DOWN:
                if mouse_rect.bottom != SCREEN_HEIGHT:
                    mouse_rect.y += 125
                mouse_surface, mouse_rect, mouse_orientation = set_orientation(DOWN, mouse_rect.center)
            

    for i in range (0, 8):
        for j in range (0, 8):
            screen.blit(undisc_cell, (i*CELL_SIZE, j*CELL_SIZE))
            if (i == 0 and j == 0):
                screen.blit(wall0000, (0,0))
            elif (i == 6):
                if (j == 6):
                    screen.blit(final_top_l, (i*CELL_SIZE, j*CELL_SIZE))
                elif (j == 7):
                    screen.blit(final_bot_l, (i*CELL_SIZE, j*CELL_SIZE))
            elif (i == 7):
                if (j == 6):
                    screen.blit(final_top_r, (i*CELL_SIZE, j*CELL_SIZE))
                elif (j == 7):
                    screen.blit(final_bot_r, (i*CELL_SIZE, j*CELL_SIZE))
    
    # Mouse 
    screen.blit(mouse_surface, mouse_rect)

    pygame.display.update()
    clock.tick(60)