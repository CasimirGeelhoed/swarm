from swarm import *
from nap import vec3
import math
import random



# Calculates the distance between two vec3s.
def distance(pos1, pos2=vec3(0.,0.,0.)):
    return math.sqrt((pos1.x - pos2.x)**2 + (pos1.y - pos2.y)**2 + (pos1.z - pos2.z)**2)


def distance2D(pos1, pos2=vec3(0.,0.,0.)):
    return math.sqrt((pos1.x - pos2.x)**2 + (pos1.z - pos2.z)**2)


# Calculates (polar coordinates) angle between two vec3s. As a numeric value between -PI/2 and PI/2 radians.
def angle(pos1, pos2=vec3(0.,0., 0.)):
    if pos1.x-pos2.x == 0:
        return 0;
    else:
        return math.atan((pos1.z-pos2.z)/(pos1.x-pos2.x))



class Behaviour:
    def __init__(self, count):
        pass

    def getPositions(self, count, elapsedTime, deltaTime):
        raise NotImplementedError


# Copy from "Scatter, Swarm, Sublimate".
class SpinningBehaviour(Behaviour):

    def __init__(self, count):

        MAX_PARTICLE_COUNT = 512
        self.phases = []
        self.speedDeviations = []
        for i in range(MAX_PARTICLE_COUNT):
            self.phases.append(random.uniform(0., 1.)) # random value between 0 and 1
            self.speedDeviations.append(random.uniform(0.5, 2.0)) # random value between 0.5 and 2

        addFloatParameter("spinner/scale", 0., 50., 1.)
        addFloatParameter("spinner/speed", -50, 50., 1.)
        addFloatParameter("spinner/speedDeviation", 0., 1., 0.)
        addFloatParameter("spinner/secondarySpinX", 0., 10., 0.)
        addFloatParameter("spinner/secondarySpinZ", 0., 10., 0.)

        addFloatParameter("spinner/octave", 0., 1., 0.)


#again..

    # ___getPositions function___
    def getPositions(self, count, elapsedTime, deltaTime):
        positions = []


        # for i in range(count):
        #     positions.append(vec3(0.0, 1.0 + 2.0 * math.sin(0.1 * i + elapsedTime),0.0))

        iMax = min(count, len(self.phases))


        # TODO: this is inefficient.
        scale = getFloat("spinner/scale")
        secondarySpinX = getFloat("spinner/secondarySpinX")
        secondarySpinZ = getFloat("spinner/secondarySpinZ")
        speed = getFloat("spinner/speed")
        speedDeviation = getFloat("spinner/speedDeviation")

        for i in range(iMax):

            pos = vec3(scale * math.sin(self.phases[i] * 2. * math.pi), 0, scale * math.cos(self.phases[i] * 2. * math.pi))

            pos += vec3(secondarySpinX * math.sin(self.phases[iMax - i] * 2. * math.pi), 0, secondarySpinZ * math.cos(self.phases[iMax - i] * 2. * math.pi))


            # octave
            if (i / float(iMax)) < getFloat("spinner/octave"):
            	pos += vec3(0., 10., 0.)

            positions.append(pos);

            # increment phase
            finalSpeed = 0.25 * (speed * ((1. - speedDeviation) + speedDeviation * self.speedDeviations[i]))
            self.phases[i] += deltaTime * finalSpeed
            if self.phases[i] > 1.:
                self.phases[i] -= 1.
            if self.phases[i] < 0.:
                self.phases[i] += 1.

        return positions




class JumpingBehaviour(Behaviour):
    def __init__(self, count):
        self.time = 0.0
        addFloatParameter("jumpingSpeed", 0.1, 10.0, 0.5)

    def getPositions(self, count, elapsedTime, deltaTime):
        self.time += deltaTime * getFloat("jumpingSpeed")
        positions = []
        for i in range(count):
            positions.append(vec3(0.0, 1.0 + 2.0 * math.sin(0.1 * i + self.time),0.0))
        return positions


class LineBehaviour(Behaviour):
    def __init__(self, count):
        self.time = 0.0

    def getPositions(self, count, elapsedTime, deltaTime):
        positions = []
        for i in range(count):
            positions.append(vec3((i / float(count)) * 2.0 - 1.0, 0.0 ,0.0))
        return positions



class Control:

    def __init__(self, entity):

        initLogger(entity)

        # number of existing sources (part of which are inactive, based on the value of the sourcesCount parameter)
        self.maxSourcesCount = 16

        addFloatParameter("sourcesCount", 1, self.maxSourcesCount, self.maxSourcesCount)

        self.behaviours = []
        self.behaviours.append(SpinningBehaviour(self.maxSourcesCount))
        self.behaviours.append(JumpingBehaviour(self.maxSourcesCount))
        self.behaviours.append(LineBehaviour(self.maxSourcesCount))

        addFloatParameter("crossfade", 0, 1, 0)
        addFloatParameter("behaviourA", 0, 2, 0)
        addFloatParameter("behaviourB", 0, 2, 1)

        addVec3Field("position")
        addFloatField("displayIntensity")
        addFloatField("active")
        addFloatField("distance")
        addFloatField("horizontalDistance")
        addFloatField("speed")
        addFloatField("angle")
        
        init(entity, self.maxSourcesCount)

        # keep last positions for calculating speed
        self.lastPositions = []
        for i in range(self.maxSourcesCount):
            self.lastPositions.append(vec3(0.,0.,0.))

    
    def update(self, elapsedTime, deltaTime):
        activeCount = int(getFloat("sourcesCount"))

        xfade = getFloat("crossfade")

        positionsA = self.behaviours[int(getFloat("behaviourA"))].getPositions(int(getFloat("sourcesCount")), elapsedTime, deltaTime)
        positionsB = self.behaviours[int(getFloat("behaviourB"))].getPositions(int(getFloat("sourcesCount")), elapsedTime, deltaTime)

        for i in range(self.maxSourcesCount):
            active = activeCount > i
            if active:
                pos = (1.0 - xfade) * positionsA[i] + xfade * positionsB[i]
                setFloat(i, "displayIntensity", pos.y)
                setFloat(i, "active", 1.0)
                setVec3(i, "position", pos)
                setFloat(i, "distance", distance(pos))
                setFloat(i, "horizontalDistance", distance2D(pos))
                setFloat(i, "speed", distance(pos, self.lastPositions[i]) / deltaTime) #TODO: could be smoothed more!
                setFloat(i, "angle", angle(pos))
                self.lastPositions[i] = pos
            else:
                setFloat(i, "displayIntensity", 0.0)
                setFloat(i, "active", 0.0)
                setVec3(i, "position", vec3(0.0, 0.0, 0.0))
                setFloat(i, "distance", 0)
                setFloat(i, "horizontalDistance", 0)
                setFloat(i, "speed", 0)
                setFloat(i, "angle", 0)






