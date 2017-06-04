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
    //_originalRhythm[0] = 1; // Testing
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps) : _originalNumSteps(_originalNumSteps), _originalRhythm(originalNumSteps, 0), _targetNumSteps(targetNumSteps), _targetRhythm(targetNumSteps, 0)
{
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, RhythmSequence originalRhythm, int targetNumSteps, RhythmSequence targetRhythm) : _originalNumSteps(originalNumSteps), _originalRhythm(originalRhythm), _targetNumSteps(targetNumSteps), _targetRhythm(targetRhythm)
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

GroovinatorRhythmHandler::RhythmSequence GroovinatorRhythmHandler::getOriginalRhythm()
{
    return _originalRhythm;
}

GroovinatorRhythmHandler::RhythmSequence GroovinatorRhythmHandler::getTargetRhythm()
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

double GroovinatorRhythmHandler::getProportionOfRhythmElapsed()
{
    return _proportionOfRhythmElapsed;
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

void GroovinatorRhythmHandler::setOriginalRhythm(RhythmSequence r)
{
    _originalRhythm = r;
    _originalNumSteps = r.size();
}

void GroovinatorRhythmHandler::setTargetRhythm(RhythmSequence r)
{
    _targetRhythm = r;
    _targetNumSteps = r.size();
}

void GroovinatorRhythmHandler::setProportionOfRhythmElapsed(double v)
{
    _proportionOfRhythmElapsed = v;
}

// Utility methods
void GroovinatorRhythmHandler::toggleOriginalRhythmStepAt(int i)
{
    if (i < 0 || i >= _originalRhythm.size())
        return;
    _originalRhythm[i] = _originalRhythm[i] == 1 ? 0 : 1;
}

void GroovinatorRhythmHandler::toggleTargetRhythmStepAt(int i)
{
    if (i < 0 || i >= _targetRhythm.size())
        return;
    _targetRhythm[i] = _targetRhythm[i] == 1 ? 0 : 1;
}

// Fibonacci stretch methods (static)
GroovinatorRhythmHandler::RhythmSequence GroovinatorRhythmHandler::generateEuclideanRhythm(int numPulses, int numSteps)
{
    RhythmSequence rhythm;
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

std::vector<int> GroovinatorRhythmHandler::calculatePulseLengths(GroovinatorRhythmHandler::RhythmSequence rhythm)
{
    std::vector<int> pulseIndices;
    for (size_t i=0; i<rhythm.size(); i++)
    {
        if (rhythm[i] > 0)
            pulseIndices.push_back((int) i);
    }
    pulseIndices.push_back(rhythm.size());
    
    std::vector<int> pulseLengths;
    for (size_t i=0; i<pulseIndices.size()-1; i++)
    {
        pulseLengths.push_back(pulseIndices[i+1] - pulseIndices[i]);
    }
    
    return pulseLengths;
}

std::vector<double> GroovinatorRhythmHandler::calculatePulseRatios(GroovinatorRhythmHandler::RhythmSequence originalRhythm, GroovinatorRhythmHandler::RhythmSequence targetRhythm)
{
    std::vector<double> pulseRatios;
    
    std::vector<int> originalPulseLengths = GroovinatorRhythmHandler::calculatePulseLengths(originalRhythm);
    std::vector<int> targetPulseLengths = GroovinatorRhythmHandler::calculatePulseLengths(targetRhythm);
    
    int numPulses = std::min(originalPulseLengths.size(), targetPulseLengths.size());
    
    for (size_t i=0; i<numPulses; i++)
    {
        pulseRatios.push_back(originalPulseLengths[i] / (double) targetPulseLengths[i]);
    }
    
    return pulseRatios;
}

std::vector<double> GroovinatorRhythmHandler::calculateStepStretchRatios(GroovinatorRhythmHandler::RhythmSequence originalRhythm, GroovinatorRhythmHandler::RhythmSequence targetRhythm)
{
    
}


// Other static methods
std::string GroovinatorRhythmHandler::rhythmToString(RhythmSequence rhythm)
{
    std::stringstream ss;
    for (size_t i=0; i<rhythm.size(); i++)
    {
        ss << rhythm[i];
    }
    return ss.str();
}

int GroovinatorRhythmHandler::proportionToStepIndex(double proportion, int numSteps)
{
    return (int) (proportion * numSteps);
}

