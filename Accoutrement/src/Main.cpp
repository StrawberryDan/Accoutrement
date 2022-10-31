#include "wx/wx.h"
#include "wx/sizer.h"
#include "tls.h"
#include "Bot/Bot.hpp"



class Accoutrement : public wxApp
{
public:
    bool OnInit() override
    {
		return true;
    }
};



wxIMPLEMENT_APP(Accoutrement);