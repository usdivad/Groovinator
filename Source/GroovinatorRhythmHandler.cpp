//
//  GroovinatorRhythmHandler.cpp
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#include <sstream>

#include "bjorklund.h"

#include "GroovinatorRhythmHandler.h"

GroovinatorRhythmHandler::GroovinatorRhythmHandler() : _originalNumSteps(8), _originalPulseIndices(), _targetNumSteps(8), _targetPulseIndices()
{
    
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps) : _originalNumSteps(_originalNumSteps), _originalPulseIndices(), _targetNumSteps(targetNumSteps), _targetPulseIndices()
{
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, std::vector<int> originalPulseIndices, int targetNumSteps, std::vector<int> targetPulseIndices) : _originalNumSteps(originalNumSteps), _originalPulseIndices(originalPulseIndices), _targetNumSteps(targetNumSteps), _targetPulseIndices(targetPulseIndices)
{
    
}

GroovinatorRhythmHandler::~GroovinatorRhythmHandler()
{
    
}

// Getters
int GroovinatorRhythmHandler::getOriginalNumSteps()
{
    return _originalNumSteps;
}
int GroovinatorRhythmHandler::getTargetNumSteps()
{
    return _targetNumSteps;
}

// Setters
void GroovinatorRhythmHandler::setOriginalNumSteps(int v)
{
    _originalNumSteps = v;
}
void GroovinatorRhythmHandler::setTargetNumSteps(int v)
{
    _targetNumSteps = v;
}

// Methods
std::vector<int> GroovinatorRhythmHandler::generateEuclideanRhythm(int numPulses, int numSteps)
{
    std::vector<int> rhythm;
    std::string rhythmStr = BjorklundsAlgorithm::bjorklund(numPulses, numSteps);
    for (size_t i=0; i<rhythmStr.size(); i++)
    {
        if (rhythmStr[i] == '0')
            rhythm.push_back(0);
        else
            rhythm.push_back(1);
    }
    return rhythm;
}

std::string GroovinatorRhythmHandler::rhythmToString(std::vector<int> rhythm)
{
    std::stringstream ss;
    for (size_t i=0; i<rhythm.size(); i++)
    {
        ss << rhythm[i];
    }
    return ss.str();
}

