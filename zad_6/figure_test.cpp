#include "Simple_window.h"
#include "Spinner.h"


int main() 
{ 
    Graph_lib::Window wnd(Point(100, 100), 600, 400,  
        "Radoslaw Dabkowski (325683)"); 
    Button btn(Point(wnd.x_max() - 80, wnd.y_max() - 20), 80,  
    20, "Close", cb_close); 
    wnd.attach(btn); 

    Spinner spin(Point(wnd.x_max() - 90, 10), 100); 
    spin.attachTo(wnd); 

    gui_main(); 
    return 0; 
} 
