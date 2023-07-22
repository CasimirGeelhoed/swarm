from swarm import *
from nap import vec3
import math

class Control:

    def __init__(self, entity):
                        
        self.sourcesCount = 20
        
        addVec3Field("position")
        addFloatField("gain")
        
        addVec3Parameter("param1", 0, 10, vec3(1,2,3))
        addFloatParameter("param2", 0, 10, 5)
        
        init(entity, self.sourcesCount)
        
    def update(self, elapsedTime, deltaTime):
    
    
        for i in range(self.sourcesCount):
            x = 10 * math.sin(elapsedTime * (0.4 + 0.1 * i))
            y = 5 + 1.0 * math.cos(elapsedTime * (10 + 0.0 * i)) + x * -0.2
            z = 10 * math.sin(elapsedTime * (0.2 + 0.07 * i))
            g = 0.2 + 0.1 * math.sin(elapsedTime * (0.1 + 0.03 * i))
            setVec3(i, "position", vec3(x, y, z))
            setFloat(i, "gain", g)

    
        # mock up, testing..
#        x = math.sin(elapsedTime * 0.5) * 1
#        y = math.cos(elapsedTime * 0.1) * 1
#        z = math.sin(elapsedTime * 0.25) * 1
#        g = math.sin(elapsedTime * 0.1)
#        setVec3(0, "position", vec3(x, y, z))
#        setVec3(1, "position", vec3(y, x, z))
#        setVec3(2, "position", vec3(z, x, y))
#        setVec3(3, "position", vec3(z * 2, x, x))
#
#        setFloat(0, "gain", g / 10.0)
#        setFloat(1, "gain", g / 10.0)
#        setFloat(2, "gain", g / 10.0)
#        setFloat(3, "gain", 0.1)
#
#        setVec3(4, "position", getVec3("param1"))
#        setFloat(4, "gain", 0.1)

