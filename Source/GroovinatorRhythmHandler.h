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
    typedef std::vector<int> RhythmSequence;
    
    GroovinatorRhythmHandler();
    GroovinatorRhythmHandler(int originalNumSteps, int targetNumSteps);
    GroovinatorRhythmHandler(int originalNumSteps, RhythmSequence originalRhythm,
                             int targetNumSteps, RhythmSequence targetRhythm);
    ~GroovinatorRhythmHandler();
    
    // Getters
    int getOriginalNumSteps();
    int getTargetNumSteps();
    RhythmSequence getOriginalRhythm();
    RhythmSequence getTargetRhythm();
    std::string getOriginalRhythmStr();
    std::string getTargetRhythmStr();
    
    // Setters
    void setOriginalNumSteps(int v);
    void setTargetNumSteps(int v);
    void setOriginalRhythm(RhythmSequence r);
    void setTargetRhythm(RhythmSequence r);
    
    // Utility methods
    void toggleOriginalRhythmStepAt(int i);
    void toggleTargetRhythmStepAt(int i);
    
    // Static methods
    static RhythmSequence generateEuclideanRhythm(int numPulses, int numSteps);
    static std::string rhythmToString(RhythmSequence rhythm);

private:
    int _originalNumSteps;
    RhythmSequence _originalRhythm;
    int _targetNumSteps;
    RhythmSequence _targetRhythm;
};

#endif /* GroovinatorRhythmHandler_h */
