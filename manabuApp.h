/***************************************************************
 * Name:      manabuApp.h
 * Purpose:   Defines Application Class
 * Author:    Patrick Sébastien Coulombe (patrick@11h11.com)
 * Created:   2010-08-06
 * Copyright: Patrick Sébastien Coulombe ()
 * License:
 **************************************************************/

#ifndef MANABUAPP_H
#define MANABUAPP_H

#include <wx/app.h>
#include <wx/filefn.h>

class manabuApp : public wxApp
{
    public:

        virtual bool OnInit();
};

#endif // MANABUAPP_H
