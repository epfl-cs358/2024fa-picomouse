from sensor import TOF
from maze import Maze
class Mouse():
    def __init__(self):
        self.left_TOF = TOF()
        self.midle_left_TOF = TOF()
        self.right_TOF = TOF()
        self.midle_left_TOF = TOF()

        self.midle_TOF = TOF()
        self.current_cell = (0,0)
        self.estimate_position = (0,0)
    

    def retrieve_position(maze:Maze):
        pass



    def place_wall():
        pass