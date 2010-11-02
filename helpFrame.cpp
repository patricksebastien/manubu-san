#include "helpFrame.h"

//(*InternalHeaders(helpFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(helpFrame)
//*)

BEGIN_EVENT_TABLE(helpFrame,wxFrame)
	//(*EventTable(helpFrame)
	//*)
END_EVENT_TABLE()

helpFrame::helpFrame(wxWindow* parent)
{
	//(*Initialize(helpFrame)
	wxXmlResource::Get()->LoadObject(this,parent,_T("helpFrame"),_T("wxFrame"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	HtmlWindow1 = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	//*)
}

helpFrame::~helpFrame()
{
	//(*Destroy(helpFrame)
	//*)
}

