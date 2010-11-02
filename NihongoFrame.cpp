#include "NihongoFrame.h"

//(*InternalHeaders(NihongoFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(NihongoFrame)
//*)

BEGIN_EVENT_TABLE(NihongoFrame,wxFrame)
	//(*EventTable(NihongoFrame)
	//*)
END_EVENT_TABLE()

NihongoFrame::NihongoFrame(wxWindow* parent)
{
	//(*Initialize(NihongoFrame)
	wxXmlResource::Get()->LoadObject(this,parent,_T("NihongoFrame"),_T("wxFrame"));
	HtmlWindow1 = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	//*)
	HtmlWindow1->LoadPage(wxGetCwd() + "/nihongo.htm");
}

NihongoFrame::~NihongoFrame()
{
	//(*Destroy(NihongoFrame)
	//*)
}

