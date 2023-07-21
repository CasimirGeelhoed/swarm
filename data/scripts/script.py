import nap
import math


class Control:

    def __init__(self, entity):
    
        self.cc = entity.findComponentByID("DataControlComponent")
        
        vec3Fields = ["position"]
        floatFields = ["gain"]
        sourcesCount = 3
        
        self.cc.initData(vec3Fields, floatFields, sourcesCount)
        
    def update(self, elapsedTime, deltaTime):
        x = math.sin(elapsedTime * 5)
        y = math.cos(elapsedTime * 15)
        z = math.sin(elapsedTime * 0.25) * 5
        g = math.sin(elapsedTime * 10) + 1.0
        self.cc.setVec3(0, "position", nap.vec3(x, y, z))
        self.cc.setVec3(1, "position", nap.vec3(y, x, z))
        self.cc.setVec3(2, "position", nap.vec3(z, x, y))
        self.cc.setFloat(0, "gain", g)
        self.cc.setFloat(1, "gain", g)
        self.cc.setFloat(2, "gain", g)

        
    def destroy():
        pass
