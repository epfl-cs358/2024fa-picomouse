import numpy as np
from typing import Tuple
import matplotlib.pyplot as plt


class Maze():
    def __init__(self):
        self.cell_width = 16
        self.wall_width = 1
        self.maze_width = 5
        self.maze_height = 5
        self.start = (0,0)
        self.end = (1,1)

        self.vertical_walls = np.ones([self.maze_height, self.maze_width+1], dtype=bool)
        self.horizontal_walls = np.ones([self.maze_height+1, self.maze_width], dtype=bool)


    def place_vertical_wall(self, position:Tuple[int, int])-> None:
        x,y = position
        if(x < 0 or  x > self.maze_width or y < 0 or y > self.maze_height):
            return
        self.vertical_walls[y,x] = True

    def place_horizontal_wall(self, position:Tuple[int, int])-> None:
        x,y = position
        if(x < 0 or  x > self.maze_width or y < 0 or y > self.maze_height):
            return
        self.horizontal_walls[y,x] = True

    def draw(self):
        ## draw vertical walls
        for y in range(self.maze_height):
            for x in range(self.maze_width+1):
                if(self.vertical_walls[y,x]):
                    scaled_x = x*(self.cell_width)
                    scaled_y = y*(self.cell_width)
                    xs = [scaled_x, scaled_x+self.wall_width, scaled_x+self.wall_width, scaled_x]
                    ys = [scaled_y, scaled_y, scaled_y+self.cell_width, scaled_y+self.cell_width]
                    plt.fill(xs, ys, color='black')

        for y in range(self.maze_height+1):
            for x in range(self.maze_width):
                if(self.horizontal_walls[y,x]):
                    scaled_x = x*(self.cell_width)
                    scaled_y = y*(self.cell_width)
                    ys = [scaled_y, scaled_y+self.wall_width, scaled_y+self.wall_width, scaled_y]
                    xs = [scaled_x, scaled_x, scaled_x+self.cell_width, scaled_x+self.cell_width]
                    plt.fill(xs, ys, color='black')