/***************************************************************
 * Name:      manabuApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Patrick Sébastien Coulombe (patrick@11h11.com)
 * Created:   2010-08-06
 * Copyright: Patrick Sébastien Coulombe ()
 * License:
 **************************************************************/

#include "manabuApp.h"

//(*AppHeaders
#include "mainApp.h"
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
//*)

#include <wx/filesys.h>
#include <wx/fs_arc.h>

IMPLEMENT_APP(manabuApp);

bool manabuApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("mainApp.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("helpFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("GanaFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("NihongoFrame.xrc"));
    if ( wxsOK )
    {
    	mainApp* Frame = new mainApp(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
