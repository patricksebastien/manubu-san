#ifndef HELPFRAME_H
#define HELPFRAME_H

//(*Headers(helpFrame)
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
//*)

class helpFrame: public wxFrame
{
	public:

		helpFrame(wxWindow* parent);
		virtual ~helpFrame();

		//(*Declarations(helpFrame)
		wxStaticText* StaticText1;
		wxHtmlWindow* HtmlWindow1;
		//*)

	protected:

		//(*Identifiers(helpFrame)
		//*)

	private:

		//(*Handlers(helpFrame)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
