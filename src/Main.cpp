#include "wx/wx.h"



class Accoutrement : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new wxFrame(nullptr, wxID_ANY, "Hello World");
        frame->Show(true);
        return true;
    }
};



wxIMPLEMENT_APP(Accoutrement);