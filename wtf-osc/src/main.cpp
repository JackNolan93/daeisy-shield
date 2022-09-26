#include "daisy_seed.h"
#include "daisysp.h"

#include "hw/daeisy_shield.h"
#include "dsp/WTFOscillator.h"
#include "ctrl/ControlRamp.h"

using namespace daisy;
using namespace daisysp;

double Twelfth_root_2 = 1.0594630943592952645618252949463417007792043174941856285592084314;
// Hardware
DaeisyShield shield;

//Dsp
JackDsp::WTFOscillator _wtfOsc;

using Ramp = JackDsp::ControlRamp;

//control vlaues
float _freqNormVal = 0;
float _tuningFreq = 0;
float _cvFreq = 0;
bool _singleWindow = false;

Ramp _freqCV;
Ramp _windowWidthCV;
Ramp _windowWidth;

void processAnalogueControls ();
void processDigitalControls ();
void updateWaveFromIndicators ();

void readCVInputs ();
void updateRealtiimeCV ();

void AudioCallback (AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    shield.ProcessAllControls ();
    processAnalogueControls ();
    processDigitalControls ();
    readCVInputs ();

    for (size_t i = 0; i < size; i++)
    {
        if (_freqCV.isRamping () || 
            _windowWidthCV.isRamping () ||
            _windowWidth.isRamping ())
            updateRealtiimeCV ();

        out[0][i] = out[1][i] = _wtfOsc.Process ();
    }
}

void initRamps ();
void initOscillator ();
void initControls ();

int main(void)
{
    shield.Init ();
	shield.SetAudioBlockSize (48);
	
    initRamps ();
    initOscillator ();

    shield.StartAdc ();
	shield.StartAudio(AudioCallback);

    while(1) 
    {
    }
}

void readCVInputs ()
{
    _freqCV.setValue (_tuningFreq * std::pow (Twelfth_root_2, shield.GetVOctValue () * 12.0));

    if (abs (shield.GetControlValue (DaeisyShield::AnalogCtrl::CVCTRL_1)  - _windowWidthCV.getValue ()) > 0.0005)
    {
        _windowWidthCV.setValue (shield.GetControlValue (DaeisyShield::AnalogCtrl::CVCTRL_1));
    }

}

void updateRealtiimeCV ()
{
    _freqCV.tick ();
    _wtfOsc.SetFreq (_freqCV.getValue ());

    _windowWidthCV.tick ();
    _windowWidth.tick ();
    _wtfOsc.SetWindowW (_windowWidth.getValue () + _windowWidthCV.getValue ());
}

void processAnalogueControls ()
{
    if (abs (shield.GetControlValue (DaeisyShield::AnalogCtrl::CTRL_1) - _freqNormVal) > 0.0005)
    {
        _freqNormVal = shield.GetControlValue (DaeisyShield::AnalogCtrl::CTRL_1);
        _tuningFreq = (_freqNormVal * 300) + 40;
         _wtfOsc.SetFreq (_tuningFreq);
    }

    if (abs (shield.GetControlValue (DaeisyShield::AnalogCtrl::CTRL_2) - _windowWidth.getValue ()) > 0.0005)
    {
        _windowWidth.setValue (shield.GetControlValue (DaeisyShield::AnalogCtrl::CTRL_2));
        _wtfOsc.SetWindowW (_windowWidth.getValue ());
    }
}

void processDigitalControls ()
{
    if (shield.buttons[0].RisingEdge ())
        _wtfOsc.IncrementFrontWaveshape ();

    if (shield.buttons[1].RisingEdge ())
        _wtfOsc.IncrementBackWaveshape ();

    if (shield.buttons[3].RisingEdge ())
    {
        if (_singleWindow)
            _wtfOsc.SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Dual);
        else
            _wtfOsc.SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Single);

        _singleWindow = !_singleWindow;
    }
}

void initOscillator ()
{
    _wtfOsc.init (shield.AudioSampleRate ());
    _wtfOsc.SetFrontWaveshape (JackDsp::WTFOscillator::WaveShape::WS_SIN);
    _wtfOsc.SetBackWaveshape (JackDsp::WTFOscillator::WaveShape::WS_HARMONIC);

    _wtfOsc.SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Single);
}

void initButtons ();
void initKnobs ();

void initControls ()
{
    initButtons ();
    initKnobs ();
}

void initRamps ()
{
    _freqCV.setBlockSize (int (shield.AudioBlockSize ()));
    _freqCV.setValueImmediate (0);

    _windowWidthCV.setBlockSize (int (shield.AudioBlockSize ()));
    _windowWidthCV.setValueImmediate (0);

    _windowWidth.setBlockSize (int (shield.AudioBlockSize ()));
    _windowWidth.setValueImmediate (0);
}


