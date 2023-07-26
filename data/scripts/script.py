from swarm import *
from nap import vec3
import math

class Control:

    def __init__(self, entity):
        self.sourcesCount = 50
        
        addVec3Field("position")
        addVec3Field("direction")
        addFloatField("gain")
        
        addVec3Parameter("param1", 0, 10, vec3(1,2,3))
        addFloatParameter("test", 0, 10, 5)
        
        init(entity, self.sourcesCount)
        
    def update(self, elapsedTime, deltaTime):
        whaha
        for i in range(self.sourcesCount):
            x = 30 * math.sin(elapsedTime * (0.4 + 0.1 * i))
            y = 5 + 1.0 * math.cos(elapsedTime * (10 + 0.0 * i)) + x * -0.2
            z = 10 * math.sin(elapsedTime * (0.2 + 0.07 * i))
            g = 0.2 + 0.1 * math.sin(elapsedTime * (0.1 + 0.03 * i))
            setVec3(i, "position", vec3(x, y, z))
            setFloat(i, "gain", g)
