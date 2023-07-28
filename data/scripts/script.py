from swarm import *
from nap import vec3
import math

class Control:

    def __init__(self, entity):
        self.sourcesCount = 50
        
        addVec3Field("position")
        addFloatField("displayScale")
        addFloatField("displayIntensity")

        
        addVec3Parameter("testvec3", 0, 10, vec3(1,2,3))
        addFloatParameter("speed", 0.01, 10, 1)
        addFloatParameter("test", 0, 10, 5)

        
        
        init(entity, self.sourcesCount)
        
    def update(self, elapsedTime, deltaTime):
        for i in range(self.sourcesCount):
            x = 30 * math.sin(getFloat("speed") * 0.1 * elapsedTime * (0.4 + 0.1 * i))
            y = 5 * math.cos(getFloat("speed") * 0.1 * elapsedTime * (10 + 0.05 * i)) + x * -0.2
            z = 30 * math.sin(getFloat("speed") * 0.1 * elapsedTime * (0.2 + 0.07 * i))
            scale = 0.5 + 0.25 * math.sin(elapsedTime * (0.1 + 0.03 * i))
            intensity = math.sin(elapsedTime + i * 0.5) * 1
            setFloat(i, "displayIntensity", intensity)
            setVec3(i, "position", vec3(x, y, z))
            setFloat(i, "displayScale", scale)
            
