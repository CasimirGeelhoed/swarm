from nap import vec3

controller = None

logger = None

vec3Fields = []
floatFields = []
vec3Parameters = []
floatParameters = []

# Adds a vec3 data field. Call before calling init().
def addVec3Field(name):
    global vec3Fields
    vec3Fields.append(name)

# Adds a float data field. Call before calling init().
def addFloatField(name):
    global floatFields
    floatFields.append(name)
    
# Adds a vec3 parameter . Call before calling init().
def addVec3Parameter(name, min, max, value):
    global vec3Parameters
    vec3Parameters.append((name, min, max, value))

# Adds a float parameter. Call before calling init().
def addFloatParameter(name, min, max, value):
    global floatParameters
    floatParameters.append((name, min, max, value))

# Initialises the parameters and data fields.
def init(entity, sourcesCount):
    global controller
    controller = entity.findComponentByID("DataControlComponent")
    controller.initParameters(vec3Parameters, floatParameters)
    controller.initData(vec3Fields, floatFields, sourcesCount)
    
    # Clear lists so entries won't be duplicated after reloading of the script
    vec3Fields.clear()
    floatFields.clear()
    vec3Parameters.clear()
    floatParameters.clear()
    

# find the logging component
def initLogger(entity):
    global logger
    logger = entity.findComponentByID("PythonLoggingComponent")
    
# add a message to be displayed
def log(message):
    logger.addMessage(str(message))


# Returns a vec3 parameter value
def getVec3(name):
    return controller.getVec3(name)

# Returns a float parameter value.
def getFloat(name):
    return controller.getFloat(name)

# Sets the vec3 value for index 'id' of data field 'name'.
def setVec3(id, name, val):
    controller.setVec3(id, name, val)

# Sets the float value for index 'id' of data field 'name'.
def setFloat(id, name, val):
    controller.setFloat(id, name, val)
