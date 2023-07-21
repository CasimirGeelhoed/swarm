import nap
import math


class Control:

    def __init__(self, entity):
    
        self.cc = entity.findComponentByID("DataControlComponent")
        self.cc.resizeData(1)
        
    def update(self, elapsedTime, deltaTime):
        self.cc.setData(0, nap.vec3(math.sin(elapsedTime), 0, 0))
        
    def destroy():
        pass
