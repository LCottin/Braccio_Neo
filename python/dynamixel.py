import pypot.dynamixel as pt
from pypot.dynamixel import autodetect_robot 
import time


#affiche le port connecte
print(pt.get_available_ports())

#ouvre une communication avzc le moteur
dxl_io = pt.DxlXL320Motor(pt.get_available_ports()[0])

dxl_io.goto_position(100, 100)