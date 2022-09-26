#include "WtfoscWrapper.h"
#include "m_pd.h"
#include <math.h>

static t_class *wtfosc_tilde_class;

typedef struct _wtfosc_tilde {
    t_object x_obj;
    
    void * cpp_obj;

    t_float x_frequency;
    t_float x_window_width;

    t_inlet *x_in_window_width;
    t_outlet*x_out;
} t_wtfosc_tilde;

t_int * wtfosc_tilde_perform(t_int *w)
{
    t_wtfosc_tilde *x = (t_wtfosc_tilde *) (w[1]);
    t_sample    *freq =      (t_sample *) (w[2]);
    t_sample      *ww =      (t_sample *) (w[3]);
    t_sample     *out =      (t_sample *) (w[4]);
    int             n =             (int) (w[5]);

    for(int i = 0; i < n; i++)
    {
        WTFOscillatorSetFreq (x->cpp_obj, freq[i]);
        WTFOscillatorSetWindowW (x->cpp_obj, ww[i]);
        
        out[i] = WTFOscillatorProcess (x->cpp_obj);
    }

    return (w+6);
}

void wtfosc_tilde_dsp(t_wtfosc_tilde *x, t_signal **sp)
{
    dsp_add (wtfosc_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void wtfosc_tilde_free (t_wtfosc_tilde *x)
{
    inlet_free (x->x_in_window_width);
    outlet_free (x->x_out);
    
    WTFOscillatorRelease (x->cpp_obj);
}

void *wtfosc_tilde_new (t_floatarg f)
{
    t_wtfosc_tilde *x = (t_wtfosc_tilde *)pd_new(wtfosc_tilde_class);

    x->x_window_width = 0.0;
    x->x_frequency = f;

    x->x_in_window_width = inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->x_out = outlet_new (&x->x_obj, &s_signal);
    
    x->cpp_obj = WTFOscillatorCreate ();
    WTFOscillatorInit (x->cpp_obj, 44100.f);

    return (void *)x;
}

extern "C" void wtfosc_tilde_setup (void)
{
    wtfosc_tilde_class = class_new(gensym("wtfosc~"),
        (t_newmethod)wtfosc_tilde_new,
        (t_method)wtfosc_tilde_free,
        sizeof(t_wtfosc_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);
    
    CLASS_MAINSIGNALIN (wtfosc_tilde_class, t_wtfosc_tilde, x_frequency);

    class_addmethod (wtfosc_tilde_class,
      (t_method)wtfosc_tilde_dsp, gensym("dsp"), A_CANT, 0);
}


