--[[

Springs by Doukus (https://soundcloud.com/doukus).
Minor modifications by Casimir Geelhoed.

Each point is attached to all other points by an ideal linear spring. You can vary the spring length and spring constant, as well as the air friction.

--]]

sourcesCount = 24


positions = {}
velocities = {}

function init()

	setCount(sourcesCount)

	addFloatParameter("springLength", 0.01, 10, 5) -- rest length of the springs
	addFloatParameter("springConstant", 0, 5, 1) -- spring constant
	addFloatParameter("friction", 0, 1, 0.4) -- friction strength

	addVec3Field("displayPosition")
	addFloatField("displayIntensity")

	for i = 1, sourcesCount do
		positions[i] = vec3(math.cos(i * 2 * math.pi/sourcesCount), 0, math.sin(i * 2 * math.pi/sourcesCount))
		velocities[i] = vec3((math.random() - 0.5) * 0.1, (math.random() - 0.5) * 0.1, (math.random() * 0.5) * 0.1)
	end

end


function update(dt)
	
	L = getFloat("springLength")
	k = getFloat("springConstant")
	f = getFloat("friction")

	for i = 1, sourcesCount do
		positions[i] = positions[i] + velocities[i] * dt

		displayIntensity = 0

		for j = 1, sourcesCount do
			if j ~= i then
				-- calculate spring force
				velocities[i] = velocities[i] + normalize(positions[j] - positions[i]) * (distance(positions[i], positions[j]) - L) * k * dt

				-- calculate display intensity
				displayIntensity = displayIntensity + (distance(positions[i], positions[j]) - L) / L
			end
		end

		displayIntensity = displayIntensity / sourcesCount

		-- apply friction
		velocities[i] = velocities[i] * (1 - f * dt)

		setVec3(i, "displayPosition", positions[i])
		setFloat(i, "displayIntensity", displayIntensity)

	end

end
