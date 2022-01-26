#include "daisysp.h"
#include "daisy_seed.h"
#include "hw/daeisy_shield.h"
#include "engine.h"

using namespace daisy;
using namespace daisysp;

DaeisyShield shield;
daisyshield::exam::Engine engine { shield };

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	engine.process (in, out, size);

	// for (size_t i = 0; i < size; ++i)
	// 	out [0][i] = out [1][i] = osc.Process ();
}

int main(void)
{
	shield.Init ();
	engine.init ();

	shield.StartAdc ();
	shield.StartAudio(AudioCallback);

	while(1) {}
}
