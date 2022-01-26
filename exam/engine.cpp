#include "engine.h"

namespace daisyshield::exam
{

Engine::Engine (daisy::DaeisyShield & shield)
: _shield (shield)
{}

void Engine::init ()
{

    float freq = 50;
    for (auto & osc : _oscillators)
    {
        osc.Init (_shield.AudioSampleRate ());
        osc.SetAmp (1.0f);
        osc.SetFreq (freq);
        osc.SetWaveform (daisysp::Oscillator::WAVE_POLYBLEP_SAW);

        freq *= 1.5f;
    }
}

void Engine::process (daisy::AudioHandle::InputBuffer & in,
                      daisy::AudioHandle::OutputBuffer & out, 
                      size_t size)
{
    _shield.ProcessAnalogControls ();
    updateParameters ();

    for (size_t spl = 0; spl < size; ++spl)
    {
        _output = 0.0;

        for (int i = 0; i < numberOfOscillators; ++i)
            _output += _oscillators [i].Process ();

        _output /= float (numberOfOscillators);

        out [0][spl] = _output;
        out [1][spl] = _output;
    }

    _shield.seed.SetLed (_shield.controls[3].Value () > 0.5);
};

void Engine::updateParameters ()
{
    _oscillators[0].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_1));
    _oscillators[1].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_2));
    _oscillators[2].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_3));
    _oscillators[3].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CTRL_4));
    _oscillators[4].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CVCTRL_1));
    _oscillators[5].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CVCTRL_2));
    _oscillators[6].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CVCTRL_3));
    _oscillators[7].SetAmp (_shield.GetControlValue (daisy::DaeisyShield::AnalogCtrl::CVCTRL_4));
}

}