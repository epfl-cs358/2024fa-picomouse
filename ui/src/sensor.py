from typing import Callable, Tuple
import math
from parser import*
class Sensor:
    
    def __init__(self, sensor_id:int = None) -> None:
        self.sensor_id:int = sensor_id
        self.last_update:int = 0
        self.callback: Callable = lambda _=2 : ()
    

class TOF(Sensor, Abstract_parser):

    def __init__(self, sensor_id:int=None, d1:int=0, orig_angle:float=0, sensor_angle:float=0, sensor_uncertainty:float=0) -> None:
        super().__init__(sensor_id)
        self.d2:float = 0
        ## distance between the sensor and the centre of the mouse
        self.d1 = d1 
        ## angle of the mouse y axis and the sensor position
        self.orig_angle = orig_angle 
        ## angle between the sensor orientation and the line from (0,0) mouse to the centre of the sensor
        self.sensor_angle = sensor_angle 

        self.uncertainty = sensor_uncertainty
    
    def update_value(self, new_distance:int, update_time:int) -> None:
        assert(new_distance >= 0)
        if(update_time >= self.last_update):
            self.d2 = new_distance
            self.callback()

    def calculate_distance_from_origin(self)-> Tuple[float, float]: 
        d3= math.sqrt(
            self.d1**2 + self.d2**2 - 2*self.d1*self.d2* math.cos(math.pi - self.sensor_angle)
        )

        angle = math.acos((self.d2**2 + d3**2 - self.d1**2)/ (2*self.d2*d3))

        return (d3, angle)

    def from_stream(self, byte_buffer:bytes):
        message_len = int.from_bytes(byte_buffer[0:SIZE_OF_INT])

        return 