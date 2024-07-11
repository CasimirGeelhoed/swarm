positions = {
vec3(-4,0,-4),
vec3(-4,0,-2),
vec3(-4,0,0),
vec3(-4,0,2),
vec3(-4,0,4),

vec3(-2,0,-4),
vec3(-2,0,-2),
vec3(-2,0,0),
vec3(-2,0,2),
vec3(-2,0,4),

vec3(0,0,-4),
vec3(0,0,-2),
vec3(0,0,0),
vec3(0,0,2),
vec3(0,0,4),

vec3(2,0,-4),
vec3(2,0,-2),
vec3(2,0,0),
vec3(2,0,2),
vec3(2,0,4),

vec3(4,0,-4),
vec3(4,0,-2),
vec3(4,0,0),
vec3(4,0,2),
vec3(4,0,4)
}


sourcesCount = #positions

passedTime = 0.0

waveDistance = 0


function length(v)
	return math.sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
	-- return math.sqrt(v.x * v.x + v.y * v.y + v.z + v.z)
end


function init()
	setCount(sourcesCount)

	addVec3Parameter("waveCenter", -10, 10, vec3(-4,0,-4))
	addFloatParameter("waveMax", 0, 50, 20)
	addFloatParameter("waveSpeed", -50, 50, 10)


	addVec3Field("displayPosition")
	addFloatField("displayScale")
	addFloatField("displayIntensity")

end



function update(t)

	waveDistance = waveDistance + t * getFloat("waveSpeed")
	if waveDistance > getFloat("waveMax") then
		waveDistance = 0
	end

	for i = 1, sourcesCount do

		intensity = math.max(0,(4 - math.abs(waveDistance - length(positions[i] + getVec3("waveCenter")))) / 5.0)


		setVec3(i, "displayPosition", positions[i] + vec3(0, intensity, 0))
		setFloat(i, "displayScale", math.max(0.1, intensity * 0.5))

		setFloat(i, "displayIntensity", intensity)

	end

end

