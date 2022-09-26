#include "WTFOscillator.h"

extern "C" void * WTFOscillatorCreate ()
{
   return new JackDsp::WTFOscillator;
}

extern "C" void WTFOscillatorRelease (void * object)
{
    delete static_cast<JackDsp::WTFOscillator *> (object);
}

extern "C" void WTFOscillatorInit (void * object, float sampleRate)
{
    static_cast <JackDsp::WTFOscillator *> (object)->init (sampleRate);
}

extern "C" float WTFOscillatorProcess (void * object)
{
    return static_cast <JackDsp::WTFOscillator *> (object)->Process();
}

extern "C" void WTFOscillatorSetFreq (void * object, float frequency)
{
    static_cast <JackDsp::WTFOscillator *> (object)->SetFreq (frequency);
}

extern "C" void WTFOscillatorSetWindowW (void * object, float ww)
{
    static_cast <JackDsp::WTFOscillator *> (object)->SetWindowW (ww);
}

extern "C" void WTFOscillatorSetWindowConfig (void * object, int config)
{
    auto wtfosc = static_cast <JackDsp::WTFOscillator *> (object);
    
    switch (config)
    {
        case 1:
            wtfosc->SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Single);
            break;
        case 2:
            wtfosc->SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Dual);
            break;
        default:
            break;
    }
}

