//
//  GroovinatorRhythmHandler.h
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#ifndef GroovinatorRhythmHandler_h
#define GroovinatorRhythmHandler_h

#include <stdio.h>
#include <vector>

class GroovinatorRhythmHandler
{
public:
    GroovinatorRhythmHandler();
    GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps);
    GroovinatorRhythmHandler(int originalNumSteps, std::vector<int> originalPulseIndices,
                             int targetNumSteps, std::vector<int> targetPulseIndices);
    ~GroovinatorRhythmHandler();
    
    // Getters
    int getOriginalNumSteps();
    
    // Setters
    void setOriginalNumSteps(int v);
private:
    int _originalNumSteps;
    std::vector<int> _originalPulseIndices;
    int _targetNumSteps;
    std::vector<int> _targetPulseIndices;
};

#endif /* GroovinatorRhythmHandler_h */
