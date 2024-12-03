from abc import ABC, abstractmethod
from typing import Callable

SIZE_OF_INT = 4
BIG_ENDIAN = 'big'
LITTLE_ENDIAN = 'little'

ENDIANNESS = BIG_ENDIAN
UNSIGNED = False


class Abstract_parser(ABC):
    @abstractmethod
    def from_stream(self, byte_buffer:bytes) -> object:
        pass

class input_multiplexer:
    def __init__(self):
        self.map: dict[str, Callable] = {}

    def add_entry(self, id:str, callback:Callable):
        assert(id not in self.map)
        self.map[id] = callback

    def parse_message():
        