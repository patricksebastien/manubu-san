#ifndef NIHONGOFRAME_H
#define NIHONGOFRAME_H

//(*Headers(NihongoFrame)
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
//*)

class NihongoFrame: public wxFrame
{
	public:

		NihongoFrame(wxWindow* parent);
		virtual ~NihongoFrame();

		//(*Declarations(NihongoFrame)
		wxHtmlWindow* HtmlWindow1;
		//*)

	protected:

		//(*Identifiers(NihongoFrame)
		//*)

	private:

		//(*Handlers(NihongoFrame)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
