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

GroovinatorRhythmHandler::GroovinatorRhythmHandler() : _originalNumSteps(8), _originalRhythm(8, 0), _targetNumSteps(8), _targetRhythm(8, 0)
{
    _originalRhythm[0] = 1;
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps) : _originalNumSteps(_originalNumSteps), _originalRhythm(originalNumSteps, 0), _targetNumSteps(targetNumSteps), _targetRhythm(targetNumSteps, 0)
{
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, std::vector<int> originalRhythm, int targetNumSteps, std::vector<int> targetRhythm) : _originalNumSteps(originalNumSteps), _originalRhythm(originalRhythm), _targetNumSteps(targetNumSteps), _targetRhythm(targetRhythm)
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

std::vector<int> GroovinatorRhythmHandler::getOriginalRhythm()
{
    return _originalRhythm;
}

std::vector<int> GroovinatorRhythmHandler::getTargetRhythm()
{
    return _targetRhythm;
}

std::string GroovinatorRhythmHandler::getOriginalRhythmStr()
{
    return GroovinatorRhythmHandler::rhythmToString(_originalRhythm);
}

std::string GroovinatorRhythmHandler::getTargetRhythmStr()
{
    return GroovinatorRhythmHandler::rhythmToString(_targetRhythm);
}

// Setters
void GroovinatorRhythmHandler::setOriginalNumSteps(int v)
{
    _originalNumSteps = v;
    _originalRhythm.resize(v);
    
    // TODO: Generate Euclidean rhythm using old _originalRhythm's numPulses, and set _originalRhythm
    //_originalRhythm = GroovinatorRhythmHandler::generateEuclideanRhythm(3, v);
}
void GroovinatorRhythmHandler::setTargetNumSteps(int v)
{
    _targetNumSteps = v;
    _targetRhythm.resize(v);

    // TODO: Generate Euclidean rhythm using old _targetRhythm's numPulses, and set _targetRhythm
    //_targetRhythm = GroovinatorRhythmHandler::generateEuclideanRhythm(3, v);
}

void GroovinatorRhythmHandler::setOriginalRhythm(std::vector<int> r)
{
    _originalRhythm = r;
    _originalNumSteps = r.size();
}

void GroovinatorRhythmHandler::setTargetRhythm(std::vector<int> r)
{
    _targetRhythm = r;
    _targetNumSteps = r.size();
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

