#include "GanaFrame.h"

//(*InternalHeaders(GanaFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(GanaFrame)
//*)

BEGIN_EVENT_TABLE(GanaFrame,wxFrame)
	//(*EventTable(GanaFrame)
	//*)
END_EVENT_TABLE()

GanaFrame::GanaFrame(wxWindow* parent)
{
	//(*Initialize(GanaFrame)
	wxXmlResource::Get()->LoadObject(this,parent,_T("GanaFrame"),_T("wxFrame"));
	StaticBitmap1 = (wxStaticBitmap*)FindWindow(XRCID("ID_STATICBITMAP1"));
	//*)
}

GanaFrame::~GanaFrame()
{
	//(*Destroy(GanaFrame)
	//*)
}

