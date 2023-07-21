import nap
import math


class Control:

    def __init__(self, entity):
    
        self.cc = entity.findComponentByID("DataControlComponent")
        self.cc.resizeData(1)
        
    def update(self, elapsedTime, deltaTime):
        x = math.sin(elapsedTime * 4)
        y = math.cos(elapsedTime * 2)
        z = math.sin(elapsedTime * 0.25) * 5
        self.cc.setData(0, nap.vec3(x, y, z))
        
    def destroy():
        pass
