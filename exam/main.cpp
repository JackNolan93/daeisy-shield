#include "daisysp.h"
#include "daisy_seed.h"
#include "hw/daeisy_shield.h"
#include "engine.h"

using namespace daisy;
using namespace daisysp;

DaeisyShield shield;
daisyshield::exam::Engine engine { shield };

AdEnv env;

bool ledState = false;

double Twelfth_root_2 = 1.0594630943592952645618252949463417007792043174941856285592084314;
// freq = freq * std::pow(Twelfth_root_2, shield.GetVOctValue () * 12.0);
bool led_state = true;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	engine.processDrums (in, out, size);
}

int main(void)
{
	shield.Init ();
	shield.SetAudioBlockSize (8);
	engine.init ();

	env.Init (shield.AudioSampleRate());
	env.SetTime (ADENV_SEG_ATTACK, 0.02);
	env.SetTime (ADENV_SEG_DECAY, 1.0);

	shield.StartAdc ();
	shield.StartAudio(AudioCallback);

	while(1) {}
}
