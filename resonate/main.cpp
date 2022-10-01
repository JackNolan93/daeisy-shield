#include "daisysp.h"
#include "daisy_seed.h"
#include "hw/daeisy_shield.h"
#include "engine.h"

using namespace daisy;
using namespace daisysp;

DaeisyShield shield;
daisyshield::resonate::Engine engine { shield };

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	engine.process (in[0], out[0], size);
}

int main(void)
{
	shield.Init ();
	shield.SetAudioBlockSize (24);
	engine.init ();

	shield.StartAdc ();
	shield.StartAudio(AudioCallback);

	while(1) {}
}
