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

GroovinatorRhythmHandler::GroovinatorRhythmHandler() : _originalRhythm(8, 0), _targetRhythm(8, 0)
{
    _originalRhythm[0] = 1;
    _targetRhythm[0] = 1;
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps) : _originalRhythm(originalNumSteps, 0), _targetRhythm(targetNumSteps, 0)
{
    _originalRhythm[0] = 1;
    _targetRhythm[0] = 1;
}

GroovinatorRhythmHandler::GroovinatorRhythmHandler(RhythmSequence originalRhythm, RhythmSequence targetRhythm) : _originalRhythm(originalRhythm), _targetRhythm(targetRhythm)
{
    
}

GroovinatorRhythmHandler::~GroovinatorRhythmHandler()
{
    
}

// Getters
int GroovinatorRhythmHandler::getOriginalNumSteps()
{
    return _originalRhythm.size();
}
int GroovinatorRhythmHandler::getTargetNumSteps()
{
    return _targetRhythm.size();
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
    _originalRhythm.resize(v);
    
    // TODO: Generate Euclidean rhythm using old _originalRhythm's numPulses, and set _originalRhythm
    //_originalRhythm = GroovinatorRhythmHandler::generateEuclideanRhythm(3, v);
}
void GroovinatorRhythmHandler::setTargetNumSteps(int v)
{
    _targetRhythm.resize(v);

    // TODO: Generate Euclidean rhythm using old _targetRhythm's numPulses, and set _targetRhythm
    //_targetRhythm = GroovinatorRhythmHandler::generateEuclideanRhythm(3, v);
}

void GroovinatorRhythmHandler::setOriginalRhythm(RhythmSequence r)
{
    _originalRhythm = r;
}

void GroovinatorRhythmHandler::setTargetRhythm(RhythmSequence r)
{
    _targetRhythm = r;
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

//std::vector<double> GroovinatorRhythmHandler::calculateStepStretchRatios()
//{
//    std::vector<double> ratios = GroovinatorRhythmHandler::calculateStepStretchRatios(_originalRhythm, _targetRhythm);
//    return ratios;
//}

//std::string GroovinatorRhythmHandler::calculateStepStretchRatiosStr()
//{
//    std::stringstream ss;
//    std::vector<double> ratios = calculateStepStretchRatios();
//    for (size_t i=0; i<ratios.size(); i++)
//    {
//        if (i != 0)
//            ss << ",";
//        ss << ratios[i];
//    }
//    return ss.str();
//}

// Fibonacci stretch methods (static)
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
    // Original and target pulse lengths
    std::vector<int> originalPulseLengths = GroovinatorRhythmHandler::calculatePulseLengths(originalRhythm);
    std::vector<int> targetPulseLengths = GroovinatorRhythmHandler::calculatePulseLengths(targetRhythm);
    
    // Pulse ratios
    std::vector<double> pulseRatios = GroovinatorRhythmHandler::calculatePulseRatios(originalRhythm, targetRhythm);
    if (pulseRatios.size() < originalPulseLengths.size()) // Add 0s to pulse ratios if there aren't enough
    {
        for (size_t i=0; i < originalPulseLengths.size() - pulseRatios.size(); i++)
        {
            pulseRatios.push_back(0.0);
        }
    }
    
    // Format pulse ratios so there's one for each step
    std::vector<double> pulseRatiosByStep;
    for (size_t i=0; i<originalPulseLengths.size(); i++)
    {
        int pulseLength = originalPulseLengths[i];
        for (size_t j=0; j<pulseLength; j++)
        {
            pulseRatiosByStep.push_back(pulseRatios[i]);
        }
    }
    
    // Calculate stretch ratios for each original step
    // Adapted from Euclidean stretch
    std::vector<double> stepStretchRatios;
    for (size_t i=0; i<std::min(originalPulseLengths.size(), targetPulseLengths.size()); i++)
    {
        // Pulse lengths
        int opl = originalPulseLengths[i];
        int tpl = targetPulseLengths[i];
        
        // Adjust target pulse length if it's too small
        while (opl > tpl)
            tpl *= 2;
        
        // Use steps as original pulse rhythm ("opr")
        RhythmSequence opr;
        for (size_t j=0; j<originalRhythm.size(); j++)
        {
            opr.push_back(1);
        }
        
        // Generate target pulse rhythm ("tpr") using Bjorklund's algorithm
        RhythmSequence tpr = GroovinatorRhythmHandler::generateEuclideanRhythm(opl, tpl);
        std::vector<int> tprPulseLengths = GroovinatorRhythmHandler::calculatePulseLengths(tpr);
        std::vector<double> tprPulseRatios = GroovinatorRhythmHandler::calculatePulseRatios(opr, tpr);
        
        // Scale the tpr pulse ratios by the corresponding ratio from pulseRatiosByStep
        for (size_t j=0; j<tprPulseRatios.size(); j++)
        {
            tprPulseRatios[j] *= pulseRatiosByStep[i];
        }
        
        // Append the ratios to step stretch ratios
        stepStretchRatios.insert(stepStretchRatios.end(), tprPulseRatios.begin(), tprPulseRatios.end());
    }
    
    // Multiply by stretch multiplier to make sure the length is same as original
    /*
    double stepStretchRatiosSum;
    for (size_t i=0; i<stepStretchRatios.size(); i++)
    {
        stepStretchRatiosSum += stepStretchRatios[i];
    }
    double stretchMultiplier = 1.0 / (stepStretchRatiosSum / (double) originalRhythm.size());
    for (size_t i=0; i<stepStretchRatios.size(); i++)
    {
        stepStretchRatios[i] *= stretchMultiplier;
    }
    */
    
    return stepStretchRatios;
}


std::string GroovinatorRhythmHandler::stepStretchRatiosToString(std::vector<double> ratios)
{
    std::stringstream ss;
    for (size_t i=0; i<ratios.size(); i++)
    {
        if (i != 0)
            ss << ",";
        ss << ratios[i];
    }
    return ss.str();
}

// Other static methods
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

