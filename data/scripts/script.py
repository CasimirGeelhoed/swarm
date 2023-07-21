import nap
import math


class Control:

    def __init__(self, entity):
    
        self.cc = entity.findComponentByID("DataControlComponent")
        self.cc.resizeData(3)
        
    def update(self, elapsedTime, deltaTime):
        x = math.sin(elapsedTime * 5)
        y = math.cos(elapsedTime * 15)
        z = math.sin(elapsedTime * 0.25) * 5
        self.cc.setData(0, nap.vec3(x, y, z))
        self.cc.setData(1, nap.vec3(y, x, z))
        self.cc.setData(2, nap.vec3(z, x, y))

        
    def destroy():
        pass
