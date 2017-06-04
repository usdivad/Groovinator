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
    double getProportionOfRhythmElapsed();
    
    // Setters
    void setOriginalNumSteps(int v);
    void setTargetNumSteps(int v);
    void setOriginalRhythm(RhythmSequence r);
    void setTargetRhythm(RhythmSequence r);
    void setProportionOfRhythmElapsed(double v);
    
    // Utility methods
    void toggleOriginalRhythmStepAt(int i);
    void toggleTargetRhythmStepAt(int i);
    std::vector<double> calculateStepStretchRatios();
    std::string calculateStepStretchRatiosStr();
    
    // Fibonacci stretch methods (static)
    // Adapted from the Python implementations in fibonaccistretch.py
    static std::vector<int> calculatePulseLengths(RhythmSequence rhythm); // fibonaccistretch.calculate_pulse_lengths()
    static std::vector<double> calculatePulseRatios(RhythmSequence originalRhythm, RhythmSequence targetRhythm); // fibonaccistretch.calculate_pulse_ratios()
    static std::vector<double> calculateStepStretchRatios(RhythmSequence originalRhythm, RhythmSequence targetRhythm); // fibonaccistretch.calculate_step_stretch_ratios() -- Calculate stretch ratios for each original step, for use in real-time
    
    // Other static methods
    static RhythmSequence generateEuclideanRhythm(int numPulses, int numSteps);
    static std::string rhythmToString(RhythmSequence rhythm);
    static int proportionToStepIndex(double proportion, int numSteps);

private:
    int _originalNumSteps;
    RhythmSequence _originalRhythm;
    int _targetNumSteps;
    RhythmSequence _targetRhythm;
    double _proportionOfRhythmElapsed;
};

#endif /* GroovinatorRhythmHandler_h */
