from nap import vec3


# TODO: change interface to: addVec3Field / addFloatField / addVec3Parameter / addFloatParameter / initAll
# That would be even more readable.

controller = None

def initController(entity):
    global controller
    controller = entity.findComponentByID("DataControlComponent")

def getVec3(name):
    return controller.getVec3(name)

def getFloat(name):
    return controller.getFloat(name)

def setVec3(id, name, val):
    controller.setVec3(id, name, val)

def setFloat(id, name, val):
    controller.setFloat(id, name, val)

def initParameters(vec3Parameters, floatParameters):
    controller.initParameters(vec3Parameters, floatParameters)

def initData(vec3Fields, floatFields, sourcesCount):
    controller.initData(vec3Fields, floatFields, sourcesCount)
