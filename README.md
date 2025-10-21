<img width="1392" alt="Screenshot 2024-08-30 at 23 05 59" src="https://github.com/user-attachments/assets/1267b430-1863-404b-b394-805e0078db2e">

# Swarm

**Swarm** is a free lightweight programming environment for spatial sound and other purposes.

**Swarm** enables creative coders to intuitively develop new algorithms for live control of large numbers of data points.

**Swarm** is designed to be flexible and easy-to-use, using simple Lua scripts that can be edited in real-time. 

**Swarm** is meant to integrate with different creative software setups and workflows:
- Swarm can control any software that listens to OSC input (such as 4DSOUND, WFSCollider, SuperCollider synths, TouchDesigner patches, Max/MSP patches)
- Swarm itself can be controlled using any OSC controller (such as TouchOSC).

**Swarm** is extremely customisable. Using Lua scripts, users can define their own input parameters, output data fields and develop their own algorithms with real-time 3D visual feedback.

**Swarm** is developed by [Casimir Geelhoed](https://casimirgeelhoed.com/) on top of the [NAP Framework](https://github.com/napframework/nap). It it a standalone application that runs on MacOS.

# Key features

- Specifically designed for controlling large numbers of data elements (think: particle systems, flocking algorithms, etc)
- Real-time scripting in Lua
- Integrated 3D visualisation
- Complete customisation of OSC input & output addresses

# Scripting

## Location

Lua scripts are located in the ___/Resources/data/scripts___ folder inside the package contents of the app. New scripts can be added to this folder and edited while the app is running. Press "Refresh" to rescan the folder.

## init() & update(t)

A valid Lua script in Swarm needs to implement two functions: 
- ___init()___: initialisation function that is called once after the script is (re)loaded. Use this function to set the sources count, initialise input parameters & output fields.
- ___update(t)___: function that is called every frame. ___t___ indicates the time passed since the last call. Use this function to implement an algorithm that sets output field data based on input parameters and passed time.

## Available functions

### Input parameters
- ___addFloatParameter(name, min, max, default)___: Creates an input parameter of type float.
  - ___name___ is the name & OSC address of the input parameter.
  - ___min___ & ___max___ are the bounds of the value.
  - ___default___ is the default value.
- ___addVec3Parameter(name, min, max, default)___: Creates an input parameter of type vec3.
  - ___name___ is the name & OSC address of the inpu tparameter.
  - ___min___ & ___max___ are the bounds of the value (as floats).
  - ___default___ is the default value (as vec3).
- ___getFloat(name)___: Returns the value of a float input parameter with the given name.
- ___getVec3(name)___: Returns the value of a vec3 input parameter with the given name.

### Output fields
- ___setCount(count)___: Should be called in init(). Sets the number of sources / the number of output OSC address per output field.
- ___addFloatField(name)___: Creates an output field of type float.
  - ___name___ is the name of the output field.
- ___addVec3Field(name)___: Creates an output field of type vec3.
  - ___name___ is the name of the output field.
- ___setFloat(index, name, value)___: Sets the value for the given index of a float field.
  - ___index___ is the index of the source you are setting the data of (starting counting from 1). _
  - __name___ is the name of the field.
  - ___value___ is the value.
- ___setVec3(index, name, value)___: Sets the value for the given index of a vec3 field.
  - ___index___ is the index of the source you are setting the data of (starting counting from 1).
  - ___name___ is the name of the field.
  - ___value___ is the value.

### Mathematics
- ___normalize(vec)___: Normalizes a vec3.
- ___distance(vecA, vecB)___: Returns the distance between two vec3's.
- ___cross(vecA, vecB)___: Returns the cross product of two vec3's.
- ___dot(vecA, vecB)___: Returns the dot product of two vec3's.
- ___length(vec)___: Returns the length of a vec3.

### Logging
- ___log(message)___: Logs a message.

# Visualisation

The 3D visualisation is controlled through fields. Certain field names are mapped to certain aspects of the visualisation.

A vec3 field named ___displayPosition___ will control the position of sources in the 3D visualisation.

A float field named ___displayScale___ will control the scale of sources in the 3D visualisation.

A float field named ___displayIntensity___ will control the color of sources in the 3D visualisation (-1 is blue, 0 is grey, 1 is orange).

# OSC Input

All input parameters can be controlled over OSC, their name being the address. The OSC input port can be set under the 'Settings' dropdown menu.

# OSC Output

### OSC output settings

The OSC output address, port and rate can be set under the 'Settings' dropdown menu.

### Generation of OSC output addresses

A field contains a separate OSC output address per source. 

Normally, these output addresses are generated as follows:

_/sourceINDEX/FIELDNAME_

where INDEX is the index of the source and FIELDNAME the name of the field.

For example: if the count is set to 3, the field named 'position' will have the OSC output addresses: /source1/position, /source2/position & /source3/position.

### Custom OSC output addresses using the '$' token

To customise the generated OSC output addresses, you can give a field a name with the token '$' in it. For a field with a token '$' in its name, OSC addresses will be generated by replacing '$' by the index.

For example: if the count is set to 3, the field named 'position/$' will have the following OSC output addresses: /position/1, /position/2 and /position/3.

# Building from source (for contributors)

1. Clone [swarm-nap-0.7](https://github.com/CasimirGeelhoed/nap/tree/swarm-nap-0.7).
2. Clone [naplua](https://github.com/CasimirGeelhoed/naplua) in /modules.
3. Clone this repository in /apps.
4. Execute the 'generate_solution.sh' script to generate the XCode project.

For more information, see the readme and documentation of [NAP Framework](https://docs.nap-framework.tech/d1/d66/getting_started).
