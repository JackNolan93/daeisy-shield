#include <PhysicalModeling/resonator.h>

extern "C" void * ResonatorCreate ()
{
   return new daisysp::Resonator ();
}

extern "C" void ResonatorRelease (void * object)
{
    delete static_cast<daisysp::Resonator *> (object);
}

extern "C" void ResonatorInit (void * object, float structure, int resolution, float sampleRate)
{
    static_cast <daisysp::Resonator *> (object)->Init (structure, resolution, sampleRate);
}

extern "C" float ResonatorProcess (void * object, float inVal)
{
    return static_cast <daisysp::Resonator *> (object)->Process (inVal);
}

extern "C" void ResonatorSetFreq (void * object, float frequency)
{
    static_cast <daisysp::Resonator *> (object)->SetFreq (frequency);
}

 extern "C" void ResonatorDamping (void * object, float damping)
 {
     static_cast <daisysp::Resonator *> (object)->Resonator::SetDamping (damping);
 }

extern "C" void ResonatorStructure (void * object, float structure)
{
    static_cast <daisysp::Resonator *> (object)->SetStructure (structure);
}

extern "C" void ResonatorBrightness (void * object, float brightness)
{
    static_cast <daisysp::Resonator *> (object)->SetBrightness (brightness);
}

