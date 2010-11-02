#ifndef GANAFRAME_H
#define GANAFRAME_H

//(*Headers(GanaFrame)
#include <wx/statbmp.h>
#include <wx/frame.h>
//*)

class GanaFrame: public wxFrame
{
	public:

		GanaFrame(wxWindow* parent);
		virtual ~GanaFrame();

		//(*Declarations(GanaFrame)
		wxStaticBitmap* StaticBitmap1;
		//*)

	protected:

		//(*Identifiers(GanaFrame)
		//*)

	private:

		//(*Handlers(GanaFrame)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
