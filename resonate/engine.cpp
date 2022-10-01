#include "engine.h"

namespace daisyshield::resonate
{

Engine::Engine (daisy::DaeisyShield & shield)
: _shield (shield)
{}

void Engine::init ()
{
     _resonator.Init (0.15, 0.5, _shield.seed.AudioSampleRate ());
     _resonator.SetStructure (0.7);
}

void Engine::process (const float * in, float * out, size_t size)
{
    updateParameters ();
    for (int i = 0; i < int (size); ++i)
        out [i] = _resonator.Process (in [i]);
}

void Engine::updateParameters ()
{
    _shield.ProcessAllControls();   

    _resonator.SetFreq (_shield.GetVOctValue ()); //110 - 880
    _resonator.SetStructure (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_1));
    _resonator.SetBrightness (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_2));
    _resonator.SetDamping (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_3));
}

}