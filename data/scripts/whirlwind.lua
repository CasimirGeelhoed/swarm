
--[[

Whirlwind script with three oscillators per point.
By Casimir Geelhoed.

--]]

sourcesCount = 50

phasesA = {}
phasesB = {}
speedsB = {}
phasesC = {}
speedsC = {}


function init()
	setCount(sourcesCount)

	for i = 1, sourcesCount do
		phasesA[i] = 2 * math.pi * i / sourcesCount
		phasesB[i] = 2 * math.pi * i / sourcesCount
		phasesC[i] = 2 * math.pi * i / sourcesCount
		speedsB[i] = math.random()
		speedsC[i] = math.random()
	end

	addFloatParameter("speed", -10, 10, 5)
	addFloatParameter("scale", 0.01, 10, 2.5)
	addFloatParameter("horizontalModulation", 0.01, 10, 1)
	addFloatParameter("verticalModulation", 0.01, 10, 1)

	addVec3Field("displayPosition")
	addFloatField("displayScale")
	addFloatField("displayIntensity")

end


function update(dt)

	delta = getFloat("speed") * dt

	for i = 1, sourcesCount do

		phasesA[i] = phasesA[i] + delta
		x = math.cos(phasesA[i]) * getFloat("scale")
		z = math.sin(phasesA[i]) * getFloat("scale")

		phasesB[i] = phasesB[i] + delta * speedsB[i]
		x = x + math.cos(phasesB[i]) * getFloat("scale")  * getFloat("horizontalModulation")
		z = z + math.sin(phasesB[i]) * getFloat("scale")  * getFloat("horizontalModulation")

		phasesC[i] = phasesC[i] + delta * speedsC[i]
		y = math.sin(phasesC[i]) * getFloat("scale")  * getFloat("verticalModulation")

		setVec3(i, "displayPosition", vec3(x,y,z))
		setFloat(i, "displayScale", 0.1)
		setFloat(i, "displayIntensity", (i / sourcesCount * 2.0) - 1.0)

	end

end