#include "WTFOscillator.h"

extern "C" void * WTFOscillator_create ()
{
   return new JackDsp::WTFOscillator;
}

extern "C" void WTFOscillator_release (void * object)
{
    delete static_cast<JackDsp::WTFOscillator *> (object);
}

extern "C" void WTFOscillator_init (void * object, float sampleRate)
{
    static_cast <JackDsp::WTFOscillator *> (object)->Init (sampleRate);
}

extern "C" float WTFOscillator_process (void * object)
{
    return static_cast <JackDsp::WTFOscillator *> (object)->Process();
}

extern "C" void WTFOscillator_setFreq (void * object, float frequency)
{
    static_cast <JackDsp::WTFOscillator *> (object)->SetFreq (frequency);
}

extern "C" void WTFOscillator_setWindowW (void * object, float ww)
{
    static_cast <JackDsp::WTFOscillator *> (object)->SetWindowW (ww);
}
