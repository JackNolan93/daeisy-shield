#include "daeisy_shield.h"

using namespace daisy;

// Hardware Definitions

#define PIN_VOCT 15

#define PIN_CTRL_1 21
#define PIN_CTRL_2 24
#define PIN_CTRL_3 25
#define PIN_CTRL_4 28

#define PIN_CV_CTRL_1 17
#define PIN_CV_CTRL_2 18
#define PIN_CV_CTRL_3 19
#define PIN_CV_CTRL_4 20

#define PIN_BUTTON_1 29
#define PIN_BUTTON_2 30
#define PIN_BUTTON_3 5
#define PIN_BUTTON_4 6

#define PIN_GATE_OUT_1 2
#define PIN_GATE_OUT_2 1

#define PIN_GATE_IN_1 12
#define PIN_GATE_IN_2 13
#define PIN_GATE_IN_3 14

void DaeisyShield::Init(bool boost)
{
    seed.Configure();
    seed.Init(boost);
    InitCvOutputs();
    InitGates();
    InitControls();
    InitButtons();
}

void DaeisyShield::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void DaeisyShield::SetHidUpdateRates()
{
    for(size_t i = 0; i < CTRL_NBR; i++)
        controls[i].SetSampleRate(AudioCallbackRate());
}

void DaeisyShield::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void DaeisyShield::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void DaeisyShield::StopAudio()
{
    seed.StopAudio();
}

void DaeisyShield::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float DaeisyShield::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

void DaeisyShield::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t DaeisyShield::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

float DaeisyShield::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void DaeisyShield::StartAdc()
{
    seed.adc.Start();
}

/** Stops Transfering data from the ADC */
void DaeisyShield::StopAdc()
{
    seed.adc.Stop();
}

void DaeisyShield::ProcessAnalogControls()
{
    for(size_t i = 0; i < CTRL_NBR; i++)
        controls[i].Process();
}

float DaeisyShield::GetControlValue(AnalogCtrl control)
{
    return (controls[control].Value());
}

float DaeisyShield::GetVOctValue(AnalogCtrl control)
{
    return (controls [V_OCT_1].Value () * 3.3 * ((100 + 47) / 100));
}

void DaeisyShield::ProcessDigitalControls()
{
    for (auto & button : buttons)
        button.Debounce ();
}

void DaeisyShield::InitControls()
{
    AdcChannelConfig cfg[CTRL_NBR];

    cfg[CTRL_1].InitSingle (seed.GetPin (PIN_CTRL_1));
    cfg[CTRL_2].InitSingle (seed.GetPin (PIN_CTRL_2));
    cfg[CTRL_3].InitSingle (seed.GetPin (PIN_CTRL_3));
    cfg[CTRL_4].InitSingle (seed.GetPin (PIN_CTRL_4));

    cfg[CVCTRL_1].InitSingle (seed.GetPin (PIN_CV_CTRL_1));
    cfg[CVCTRL_2].InitSingle (seed.GetPin (PIN_CV_CTRL_2));
    cfg[CVCTRL_3].InitSingle (seed.GetPin (PIN_CV_CTRL_3));
    cfg[CVCTRL_4].InitSingle (seed.GetPin (PIN_CV_CTRL_4));

    cfg[V_OCT_1].InitSingle (seed.GetPin (PIN_VOCT));

    seed.adc.Init(cfg, CTRL_NBR);

    for(size_t i = 0; i < CTRL_NBR; i++)
        controls[i].Init(seed.adc.GetPtr (i), AudioCallbackRate ());
}

void DaeisyShield::InitCvOutputs()
{
    DacHandle::Config cfg;
    cfg.bitdepth   = DacHandle::BitDepth::BITS_12;
    cfg.buff_state = DacHandle::BufferState::ENABLED;
    cfg.mode       = DacHandle::Mode::POLLING;
    cfg.chn        = DacHandle::Channel::BOTH;
    seed.dac.Init(cfg);
    seed.dac.WriteValue(DacHandle::Channel::BOTH, 0);
}

void DaeisyShield::InitGates()
{
    // Gate Output
    gate_outputs[GATE_OUT_1].pin  = seed.GetPin(PIN_GATE_OUT_1);
    gate_outputs[GATE_OUT_1].mode = DSY_GPIO_MODE_OUTPUT_PP;
    gate_outputs[GATE_OUT_1].pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&gate_outputs[GATE_OUT_1]);
    gate_outputs[GATE_OUT_2].pin  = seed.GetPin(PIN_GATE_OUT_2);
    gate_outputs[GATE_OUT_2].mode = DSY_GPIO_MODE_OUTPUT_PP;
    gate_outputs[GATE_OUT_2].pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&gate_outputs[GATE_OUT_2]);

    // Gate Inputs
    dsy_gpio_pin pin;
    pin = seed.GetPin(PIN_GATE_IN_1);
    gate_inputs[GATE_IN_1].Init(&pin);
    pin = seed.GetPin(PIN_GATE_IN_2);
    gate_inputs[GATE_IN_2].Init(&pin);
    pin = seed.GetPin(PIN_GATE_IN_3);
    gate_inputs[GATE_IN_3].Init(&pin);
}

void DaeisyShield::InitButtons ()
{
    buttons[BUTTON_1].Init(seed.GetPin(PIN_BUTTON_1), seed.AudioCallbackRate ());
    buttons[BUTTON_2].Init(seed.GetPin(PIN_BUTTON_2), seed.AudioCallbackRate ());
    buttons[BUTTON_3].Init(seed.GetPin(PIN_BUTTON_3), seed.AudioCallbackRate ());
    buttons[BUTTON_4].Init(seed.GetPin(PIN_BUTTON_4), seed.AudioCallbackRate ());
} 
