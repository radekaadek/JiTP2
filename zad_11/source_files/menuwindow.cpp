#include "menuwindow.h"

const std::vector<colorSpec> MenuWindow::fill_colors = {
    {"red", Graph_lib::Color::red},
    {"blue", Graph_lib::Color::blue},
    {"green", Graph_lib::Color::green},
    {"yellow", Graph_lib::Color::yellow},
    {"black", Graph_lib::Color::black},
    {"white", Graph_lib::Color::white},
    {"cyan", Graph_lib::Color::cyan},
    {"magenta", Graph_lib::Color::magenta},
};

MenuWindow::MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label) :
    Graph_lib::Window(loc, w, h, label),
    rect(Graph_lib::Point(100, 100), 300, 200),
    close_btn(Graph_lib::Point(x_max() - 70, 0), 70, 20, "Close", cb_close),
    menu_fill(Graph_lib::Point(0, 0), 70, 20, "Fill"),
    menu_line(Graph_lib::Point(x_max() - 130, 50), 80, 20, "Frame")
    {
        attach(close_btn);
        attach(rect);
        menu_fill.attach(this, fill_colors);
        menu_line.attach(this, fill_colors);
    }

MenuHeader::~MenuHeader() {
    if (pWnd) {
        pWnd->detach(*this);
    }
    for (auto btn : btns) {
        pWnd->detach(*btn);
        delete btn;
    }
}

void MenuItem::attach(MenuWindow* pWnd, MenuHeader *pMenu, Graph_lib::Color color, Graph_lib::Callback cb_setColor)
{
    buttonAction.pParent = pWnd;
    buttonAction.pMenu = pMenu;
    buttonAction.menu_action = actionDescriptor::Menu_select;
    buttonAction.selected_color = color;

    pWnd->attach(*this);
    pw->callback(reinterpret_cast<Fl_Callback*>(cb_setColor), &buttonAction);
}

void MenuHeader::cb_openClose(Graph_lib::Address, Graph_lib::Address pDsc) 
{ 
    actionDescriptor *pAD = reinterpret_cast<actionDescriptor*>(pDsc);
    pAD->pMenu->openClose();
    pAD->pParent->menuAction(pAD); 
} 

void MenuWindow::menuAction(actionDescriptor* action)
{
    switch (action->menu_action)
    {
    case actionDescriptor::Menu_toggle:
        action->pMenu->openClose();
        action->pParent->redraw();
        break;
    case actionDescriptor::Menu_select:
        setColor(action->selected_color);
        action->pParent->redraw();
        break;
    default:
        break;
    }
}

void MenuHeader::openClose()
{
    expanded ? hideMenu() : showMenu();
    expanded = !expanded;
}

void MenuHeader::hideMenu()
{
    for (auto btn : btns)
    {
        btn->hide();
    }
}

void MenuHeader::showMenu()
{
    for (auto btn : btns)
    {
        btn->show();
    }
}

void MenuHeader::cb_setColor (Graph_lib::Address, Graph_lib::Address pAD)
{
    actionDescriptor *pDsc = reinterpret_cast<actionDescriptor*>(pAD);
    pDsc->menu_action = actionDescriptor::Menu_select;
    pDsc->pParent->menuAction(pDsc);
};

void MenuHeader::attach(MenuWindow *pWnd, const std::vector<colorSpec> &colors)
{
    this->pWnd = pWnd;
    pWnd->attach(*this);
    Graph_lib::Point xy = this->loc;
    int h = this->height;
    for (auto col : colors)
    {
        xy.y += h;
        MenuItem *pItem = new MenuItem(xy, 80, 20, col.label);
        btns.push_back(pItem);
        pItem->attach(pWnd, this, col.color, cb_setColor);
    }

    hideMenu();

    mAction.pParent = pWnd;
    mAction.pMenu = this;
    pw->callback(reinterpret_cast<Fl_Callback*>(cb_openClose), &mAction);
}

void MenuWindow::setColor(Graph_lib::Color color)
{
    rect.set_color(color);
    redraw();
}

/*

14
 1/19 
Obsługa menu w 25 kroków 
Inspiracją do kolejnego zadania było rozwiązanie zadania, które 
dostałem w jednej z poprzednich edycji: 
 
Niewątpliwie wodotrysków dodanych do rysowania prostokątów jest tutaj 
sporo (dawały dodatkowe dwa punkty), ale zostały wykonane brutalnymi 
metodami. Można zrobić to zgrabniej, oszczędzając i miejsce w oknie, i 
ilość pisanego kodu. 
 11/19 
najpewniej nie wie, że powinno się odświeżyć. Dodaję w 
showMenu wywołanie pParent->redraw(), i: 
 18. Jest nieźle, ale attach (przypinanie widgetu do okna) jest dość 
kosztowną operacją. Podpięcie przycisków do okna przerzucę 
do menuHeader::attach, natomiast w showMenu będę 
wywoływać funkcję show (a w hideMenu za moment – hide). 
19. Wyszło, ale nie bardzo, bo po utworzeniu widzę menu już 
rozwinięte. Implementuję od razu menuHide, żeby wywołać w 
menuHeader::attach po utworzeniu i podpięciu przycisków do 
okna.  
 12/19 
Jako bonus przychodzi możliwość usunięcia z showMenu 
pParent->redraw() (w hideMenu nawet go nie 
umieszczałem). 
20. Ponieważ menu powinno „wiedzieć” czy jest rozwinięte, czy 
nie, dodaję składową expanded (typu bool) i ustawiam ją 
stosownie w showMenu i w hideMenu. 
21. Pora zająć się problemem rysowania menu pod prostokątem. 
To kolejna oznaka, że o rozwijaniu i zwijaniu menu powinno 
decydować okno. Trzeba wziąć pod uwagę, że okno będzie 
mieć co nieco do obsługi: 
a. Otwieranie menu 
b. Zamykanie menu 
c. Ustawianie koloru  
I to w dodatku docelowo z więcej niż jednego menu. 
 13/19 
Zacznę od przygotowania struktury, która będzie zbierać 
wszystkie informacje potrzebne dla okna (i menu): struct actionDescriptor 
{ 
    enum Action { NoAction, Menu_open, Menu_close, Menu_select }; 
    MenuWindow  *pParent = nullptr; 
    MenuHeader  *pMenu = nullptr; 
    Action      menu_action = NoAction; 
    Color       selected_color = Color::invisible; 
}; 
Uwaga: Żeby pomieścić się na stronie trochę uprościłem tę 
definicję – uzupełnienie brakujących elementów jest 
elementem zadania ☺. Zwróćcie uwagę, że wszystkie składowe struktury są inicjowane przy definicji – to 
oznacza, że domyślny konstruktor „zastanie” już te wartości ustawione. Czyli nie 
musimy go w ogóle definiować! 
22. Mając taką strukturę, będę mógł przekazać do okna (do nowej 
metody publicznej menuAction) komplet informacji. Definiuję tę 
metodę – na razie tylko z obsługą akcji Menu_open i 
Menu_close. 
 14/19 
23. Teraz porządki w funkcji cb_openClose i jej parametrach. Po 
pierwsze, chciałbym żeby jako drugi parametr do tej funkcji 
trafiło wskazanie na deskryptor akcji (a tam znajdę wskazanie 
na obiekt MenuHeader i na obiekt MenuWindow). 
Do MenuHeader dodaję składową mAction typu 
actionDescriptor. Wartości wskazań w tym obiekcie inicjuję 
w MenuHeader::attach i podaję wskazanie na tę składową do 
przekazania funkcji zwrotnej: pw->callback(reinterpret_cast<Fl_Callback*>(cb_openClose), &mAction); 
24. Teraz w cb_openClose wystarczy mi ustawić w deskryptorze 
akcję i wywołać menuAction z MenuWindow: void menuHeader::cb_openClose(Address, Address pDsc) 
{ 
    actionDescriptor *pAD = reinterpret_cast<actionDescriptor*>(pDsc); 
    if (pAD->pMenu->expanded) 
        pAD->menu_action = actionDescriptor::Menu_close; 
    else 
        pAD->menu_action = actionDescriptor::Menu_open; 
    pAD->pParent->menuAction(pAD); 
} 
 15/19 
25. Działanie nie zmieniło się, ale teraz mogę powalczyć z 
prostokątem. Jeśli mam zamiar wyświetlić menu, to trzeba 
„odczepić” kształt od okna – nie będzie wtedy odrysowywany 
po widgetach. Przy zamykaniu menu kształt trzeba „przyczepić” 
ponownie. 
26. Wreszcie można zająć się kolorami. Rozwiązanie z 
deskryptorem jest niezłe i chciałbym je zastosować również 
tutaj. Żeby nie mnożyć nad miarę deskryptorów użyję 
zdefiniowanego już deskryptora akcji. Jako że taki deskryptor 
ma mieć każdy przycisk, to potrzebuję nową klasę MenuItem, 
która dziedziczy publicznie po Graph_lib::Button.  
 16/19 
Jej definicję widzę tak: 
class MenuItem : public Graph_lib::Button 
{ 
public: 
    MenuItem(Graph_lib::Point loc, int w, int h, 
                 const std::string& label); 
    void attach(MenuWindow* pWnd, MenuHeader *pMenu, 
                Graph_lib::Color color, Graph_lib::Callback cb_setColor); 
private: 
    actionDescriptor buttonAction; 
}; 
27. Konstruktor jest oczywisty. W attach() należy: 
a. Ustawić sensownie wszystkie składowe buttonAction 
b. Przypiąć przycisk do okna 
c. Wskazać jako funkcję zwrotną cb_setColor, a jako jej 
argument przekazać wskazanie na buttonAction 
Kolejność punktów b i c jest bardzo istotna. 
28. Po usunięciu ewentualnych błędów kompilacji można ruszyć do 
zmian w MenuHeader. Najpierw trzeba zdefiniować statyczną 

*/