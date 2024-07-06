



sourcesCount = 100

passedTime = 0.0

phases = {}


function init()
	setCount(sourcesCount)


	addVec3Field("displayPosition")
	addFloatField("displayScale")
	addFloatField("displayIntensity")

	addFloatField("distance")
	addFloatField("speed")
	addFloatField("distortion")

	addFloatField("position/x")
	addFloatField("position/y")
	addFloatField("position/z")


	addFloatParameter("speed", 0.01, 10, 1)
	addFloatParameter("scale", 0.01, 10, 1)
	addFloatParameter("vertical", 0.01, 10, 1)

	for i = 1, sourcesCount do
		phases[i] = 2 * math.pi * i / sourcesCount
	end

	return true
end



function update(t)
	-- time = time + (t / 100.0) * getFloat("speed")
	-- phase = t * getFloat("rotationSpeed")

	delta = getFloat("speed") * t

	for i = 1, sourcesCount do
		phases[i] = phases[i] + delta
		x = math.cos(phases[i]) * getFloat("scale")
		z = math.sin(phases[i]) * getFloat("scale")
		y = math.sin(i + phases[i]) * getFloat("vertical") * getFloat("scale")
		setFloat(i, "position/x", x)
		setFloat(i, "position/y", y)
		setFloat(i, "position/z", z)

		setVec3(i, "displayPosition", vec3(x,y,z))
		setFloat(i, "displayScale", 0.1)

	end

	return true
end


-- function init()
-- 	addVec3Parameter("test", 0, 100, vec3(1,2,3))
-- 	return true
-- end


-- timePassed = 0.0

-- function update(t)
-- 	timePassed = timePassed + t
-- 	log("hey! " .. timePassed)
-- 	test = getVec3("test");
-- 	return test.x
-- end