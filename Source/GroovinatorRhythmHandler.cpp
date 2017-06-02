//
//  GroovinatorRhythmHandler.cpp
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

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

// Setters
void GroovinatorRhythmHandler::setOriginalNumSteps(int v)
{
    _originalNumSteps = v;
}
