#include "Window.h"
#include "Spinner.h"
#include "GUI.h"


void Spinner::attach(Graph_lib::Window &wnd)
{ 
    Button::attach(wnd); // wywolanie attach z klasy bazowej 
    // przekazanie informacji do fltk o drugim parametrze callback 
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), pSpinner); 
}

