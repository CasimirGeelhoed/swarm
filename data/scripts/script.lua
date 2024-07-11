
function init()

	setCount(4)

	addVec3Parameter("positionA", -20, 20, vec3(-1,0,-1))
	addVec3Parameter("positionB", -20, 20, vec3(-1,0,1))
	addVec3Parameter("positionC", -20, 20, vec3(1,0,-1))
	addVec3Parameter("positionD", -20, 20, vec3(1,0,1))

	addFloatParameter("intensityA", -1, 1, 0)
	addFloatParameter("intensityB", -1, 1, 0)
	addFloatParameter("intensityC", -1, 1, 0)
	addFloatParameter("intensityD", -1, 1, 0)

	addFloatField("displayIntensity")
	addVec3Field("displayPosition")
	addFloatField("displayScale")

end



function update(t)

	setVec3(1, "displayPosition", getVec3("positionA"))
	setVec3(2, "displayPosition", getVec3("positionB"))
	setVec3(3, "displayPosition", getVec3("positionC"))
	setVec3(4, "displayPosition", getVec3("positionD"))

	setFloat(1, "displayScale", 0.1)
	setFloat(2, "displayScale", 0.1)
	setFloat(3, "displayScale", 0.1)
	setFloat(4, "displayScale", 0.1)

	setFloat(1, "displayIntensity", getFloat("intensityA"))
	setFloat(2, "displayIntensity", getFloat("intensityB"))
	setFloat(3, "displayIntensity", getFloat("intensityC"))
	setFloat(4, "displayIntensity", getFloat("intensityD"))

end