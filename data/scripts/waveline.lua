
--[[

Waveline script consisting of a line of points on which 'waves' are triggered that propagate in one direction.
By Casimir Geelhoed.

--]]

sourcesCount = 50

basePositions = {}
passToRight = {}
passToLeft = {}

velocities = {}
offsets = {}

function init()
	setCount(sourcesCount)

	for i = 1, sourcesCount do
		basePositions[i] = vec3(i - sourcesCount / 2, 0, 0)
		-- offsets[i] = 0
		passToLeft[i] = 0
		passToRight[i] = 0
		velocities[i] = 0
		offsets[i] = 0
	end

	addFloatParameter("fade", 0, 1, 0.5)
	addFloatParameter("propagation", 0, 1, 0.95)
	addFloatParameter("speed", 0, 100, 10)
	addFloatParameter("height", 0, 10, 5)
	addFloatParameter("density", 0, 100, 5.0)


	addFloatField("value")


	addVec3Field("displayPosition")
	addFloatField("displayScale")
	addFloatField("displayIntensity")

	triggerWave(10)


end

-- triggers a wave from speaker 'index' by giving it a random offset
function triggerWave(index)
	if math.random() > 0.5 then
		passToLeft[index] = getFloat("height")
	else
		passToRight[index] = getFloat("height")
	end
end


function update(dt)

	-- skip when delta time is too high
	if dt > 0.3 then
		log("skipped cycle, t = " .. dt)
		return
	end

	-- retrieve parameter values
	propagation = getFloat("propagation")
	probability = getFloat("density")
	speed = getFloat("speed")
	fade = getFloat("fade")

	-- calculate the amount of energy that will be passed on to neighbours
	passAmount = math.min(1, speed * dt)

	-- update offsets
	for i = 1, sourcesCount do

		-- pass wave energy
		if i ~= 1 then
			passToLeft[i-1] = passToLeft[i-1] + passAmount * passToLeft[i] * propagation
			passToLeft[i] = (1.0 - passAmount) * passToLeft[i]
		end
		if i ~= sourcesCount then
			passToRight[i+1] = passToRight[i+1] + passAmount * passToRight[i] * propagation
			passToRight[i] = (1.0 - passAmount) * passToRight[i]
		end

		-- bounce wave energy
		if i == 1 then
			passToRight[i] = passToRight[i] + passAmount * passToLeft[i]
			passToLeft[i] = (1.0 - passAmount) * passToLeft[i]
		end
		if i == sourcesCount then
			passToLeft[i] = passToLeft[i] + passAmount * passToRight[i]
			passToRight[i] = (1.0 - passAmount) * passToRight[i]
		end

		-- fade wave energy
		passToLeft[i] = math.max(0, passToLeft[i] - fade * dt)
		passToRight[i] = math.max(0, passToRight[i] - fade * dt)


		-- set fields
		offset = passToLeft[i] + passToRight[i]
		setFloat(i, "displayScale", 0.1 + math.min(0.4, 0.25 * offset))
		setFloat(i, "displayIntensity", offset)
		setVec3(i, "displayPosition", basePositions[i] + vec3(0,offset,0))
		setFloat(i, "value", offset)
	end
 

	-- trigger waves 
	if math.random() < probability * dt then
		triggerWave(math.ceil(math.random() * sourcesCount))
	end

end

