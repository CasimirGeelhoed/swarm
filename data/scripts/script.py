from base import *
from nap import vec3
import math

class Control:

    def __init__(self, entity):
            
        vec3Fields = ["position"]
        floatFields = ["gain"]
        sourcesCount = 5
        
        vec3Parameters = [("param1", 0, 10, vec3(1,2,3))]
        floatParameters = [("param2", 0, 10, 5)]
        
        initController(entity)
        initParameters(vec3Parameters, floatParameters)
        initData(vec3Fields, floatFields, sourcesCount)
        
    def update(self, elapsedTime, deltaTime):
        # mock up, testing..
        x = math.sin(elapsedTime * 0.5) * 1
        y = math.cos(elapsedTime * 0.1) * 1
        z = math.sin(elapsedTime * 0.25) * 1
        g = math.sin(elapsedTime * 0.1) + 1.0
        setVec3(0, "position", vec3(x, y, z))
        setVec3(1, "position", vec3(y, x, z))
        setVec3(2, "position", vec3(z, x, y))
        setVec3(3, "position", vec3(z * 2, x, x))
    
        setFloat(0, "gain", g - 0.3)
        setFloat(1, "gain", g - 0.3)
        setFloat(2, "gain", g - 0.3)
        setFloat(3, "gain", 0.1)
        
        setVec3(4, "position", getVec3("param1"))
        setFloat(4, "gain", getFloat("param2") / 10.0)

