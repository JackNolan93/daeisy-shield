#include "daisy_seed.h"
#include "daisysp.h"
#include <bitset>

#include "dsp/WTFOscillator.h"
#include "ctrl/ControlRamp.h"

using namespace daisy;
using namespace daisysp;

float TWELTH_ROOT_2 = 1.059463094359;
// Hardware
DaisySeed hw;

//Dsp
JackDsp::WTFOscillator _wtfOsc;

//controls
Switch WaveShapeButton1, WaveShapeButton2;
Switch WindowConfigSwitch;

// digi outs
dsy_gpio W1_bit0, W1_bit1, W1_bit2;
dsy_gpio W2_bit0, W2_bit1, W2_bit2;

using Ramp = JackDsp::ControlRamp;

//control vlaues
float _windowWidth = 0;
float _freqNormVal = 0;
float _tuningFreq = 0;

Ramp _freqCV;
Ramp _windowWidthCV;

void processAnalogueControls ();
void processDigitalControls ();
void updateWaveFromIndicators ();

void readCVInputs ();
void updateRealtiimeCV ();

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    processAnalogueControls ();
    processDigitalControls ();
    readCVInputs ();

    for (size_t i = 0; i < size; i++)
    {
        if (_freqCV.isRamping () || 
            _windowWidthCV.isRamping ())
            updateRealtiimeCV ();

        out[i] = out[i + 1] = _wtfOsc.Process ();
    }
}

void initRamps ();
void initOscillator ();
void initControls ();
void initWaveformIndicator ();

int main(void)
{
    hw.Configure();
    hw.Init();

    initRamps ();
    initControls ();
    initWaveformIndicator ();
    initOscillator ();
    
    hw.StartAudio (AudioCallback);
    hw.adc.Start ();

    while(1) 
    {
    }
}

void readCVInputs ()
{
    if (abs (hw.adc.GetFloat(3) - _freqCV.getValue ()) > 0.0005)
        _freqCV.setValue (hw.adc.GetFloat(3));

    if (abs (hw.adc.GetFloat(2) - _windowWidthCV.getValue ()) > 0.0005)
        _windowWidthCV.setValue (hw.adc.GetFloat(2));

}

void updateRealtiimeCV ()
{
    _freqCV.tick (); ;
    _wtfOsc.SetFreq (_tuningFreq * std::pow (TWELTH_ROOT_2, (_freqCV.getValue () * 3.3  * 12.0)));

    _windowWidthCV.tick (); ;
    _wtfOsc.SetWindowW (_windowWidth + _windowWidthCV.getValue ());
}


void processAnalogueControls ()
{
    if (abs (hw.adc.GetFloat(1) - _freqNormVal) > 0.0005)
    {
        _freqNormVal = hw.adc.GetFloat(1);
        _tuningFreq = (_freqNormVal * 300) + 40;

        _wtfOsc.SetFreq (_tuningFreq);
    }

    if (abs (hw.adc.GetFloat(0) - _windowWidth) > 0.0005)
    {
        _windowWidth = hw.adc.GetFloat(0);
        _wtfOsc.SetWindowW (_windowWidth);
    }
}

void processDigitalControls ()
{
    bool updateOutput = false;

    WaveShapeButton1.Debounce ();
    if (WaveShapeButton1.RisingEdge ())
    {
        _wtfOsc.IncrementFrontWaveshape ();
        updateOutput = true;
    }

    WaveShapeButton2.Debounce ();
    if (WaveShapeButton2.RisingEdge ())
    {
        _wtfOsc.IncrementBackWaveshape ();
        updateOutput = true;
    }

    if (updateOutput) 
        updateWaveFromIndicators ();

    WindowConfigSwitch.Debounce ();
    if (WindowConfigSwitch.RisingEdge ())
    {
        _wtfOsc.SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Dual);
    }
    if (WindowConfigSwitch.FallingEdge ())
    {
        _wtfOsc.SetWindowConfig (JackDsp::WTFOscillator::WindowConfig::Single);
    }
}

void updateWaveFromIndicators ()
{
    auto indexes = _wtfOsc.getWaveshapeIndexes ();

    std::bitset <3> waveformIndicator1Out (indexes.first);

    dsy_gpio_write (&W1_bit0, waveformIndicator1Out [0]);
    dsy_gpio_write (&W1_bit1, waveformIndicator1Out [1]);
    dsy_gpio_write (&W1_bit2, waveformIndicator1Out [2]);

    std::bitset <3> waveformIndicator2Out (indexes.second);

    dsy_gpio_write (&W2_bit0, waveformIndicator2Out [0]);
    dsy_gpio_write (&W2_bit1, waveformIndicator2Out [1]);
    dsy_gpio_write (&W2_bit2, waveformIndicator2Out [2]);
}

void initOscillator ()
{
    _wtfOsc.Init (hw.AudioSampleRate ());
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
 
void initKnobs ()
{
    AdcChannelConfig adcConfig [4];
    adcConfig[0].InitSingle (hw.GetPin (15));
    adcConfig[1].InitSingle (hw.GetPin (16));
    adcConfig[2].InitSingle (hw.GetPin (24));
    adcConfig[3].InitSingle (hw.GetPin (25));

    hw.adc.Init (adcConfig, 4);
}

void initButtons ()
{
    WaveShapeButton1.Init(hw.GetPin(29), hw.AudioCallbackRate ());
    WaveShapeButton2.Init(hw.GetPin(30), hw.AudioCallbackRate ());

    WindowConfigSwitch.Init(hw.GetPin(28), 
                            hw.AudioCallbackRate (),
                            Switch::Type::TYPE_TOGGLE, 
                            Switch::Polarity::POLARITY_INVERTED, 
                            Switch::Pull::PULL_UP);
}

void initRamps ()
{
    _freqCV.setBlockSize (int (hw.AudioBlockSize ()));
    _freqCV.setValueImmediate (0);

    _windowWidthCV.setBlockSize (int (hw.AudioBlockSize ()));
    _windowWidthCV.setValueImmediate (0);
}

void initWaveformIndicator ()
{
    W1_bit0.pin  = hw.GetPin (1);
    W1_bit0.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W1_bit0.pull = DSY_GPIO_PULLDOWN;
    W1_bit1.pin  = hw.GetPin (2);
    W1_bit1.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W1_bit1.pull = DSY_GPIO_PULLDOWN;
    W1_bit2.pin  = hw.GetPin (3);
    W1_bit2.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W1_bit2.pull = DSY_GPIO_PULLDOWN;

    W2_bit0.pin  = hw.GetPin (7);
    W2_bit0.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W2_bit0.pull = DSY_GPIO_PULLDOWN;
    W2_bit1.pin  = hw.GetPin (6);
    W2_bit1.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W2_bit1.pull = DSY_GPIO_PULLDOWN;
    W2_bit2.pin  = hw.GetPin (5);
    W2_bit2.mode = DSY_GPIO_MODE_OUTPUT_PP;
    W2_bit2.pull = DSY_GPIO_PULLDOWN;

    dsy_gpio_init(&W1_bit0);
    dsy_gpio_init(&W1_bit1);
    dsy_gpio_init(&W1_bit2);

    dsy_gpio_init(&W2_bit0);
    dsy_gpio_init(&W2_bit1);
    dsy_gpio_init(&W2_bit2);

    updateWaveFromIndicators ();
}


