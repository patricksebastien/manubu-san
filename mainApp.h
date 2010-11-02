#ifndef MAINAPP_H
#define MAINAPP_H

//(*Headers(mainApp)
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
//*)

//SQLITE3
#include <wx/wxsqlite3.h>

//GANAWINDOW
#include "GanaFrame.h"
#include "helpFrame.h"
#include "NihongoFrame.h"

//MESSAGEBOX - DEBUG
#include <wx/msgdlg.h>

//TIP OF THE DAY
#include <wx/tipdlg.h>

//SPLASH
#include <wx/splash.h>

//for converting from xgana to romaji
//key = int
//value = string
WX_DECLARE_HASH_MAP(int, wxString, wxIntegerHash, wxIntegerEqual, GanaHash);
WX_DECLARE_STRING_HASH_MAP( wxString, CategoryHash );


class mainApp: public wxFrame
{
	public:

		mainApp(wxWindow* parent);
		virtual ~mainApp();

        //gana hash
        //for converting from xgana to romaji
		GanaHash gana;
		CategoryHash category;

        void Tip();

		//DATABASE (SQLITE3)
		wxSQLite3Database db;
        wxString requete;

        long int signed offset;
        long int signed max;
        int unsigned g_main_id;
        int unsigned state_category;
        wxString ConvertGana(wxString ganatoconvert);
        wxString ConvertCategory(wxString categorytoconvert);

		//(*Declarations(mainApp)
		wxToolBarToolBase* ToolBarItem4;
		wxToolBar* ToolBar1;
		wxButton* Button4;
		wxHtmlWindow* displayst;
		wxToolBarToolBase* ToolBarItem3;
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxCheckBox* cbrandom;
		wxToolBarToolBase* tbnihongo;
		wxChoice* cfrequency;
		wxCheckBox* showromaji;
		wxStaticText* StaticText3;
		wxButton* next;
		wxButton* Button2;
		wxCheckBox* tatoeba;
		wxButton* deletebutton;
		wxStaticText* nbviewst;
		wxButton* Button3;
		wxStaticText* stof;
		wxToolBarToolBase* ToolBarItem5;
		wxButton* previous;
		wxStaticText* stcurrent;
		wxStaticText* stmax;
		wxPanel* background;
		wxChoice* Choice1;
		wxToolBarToolBase* ToolBarItem2;
		//*)

	protected:

		//(*Identifiers(mainApp)
		//*)

	private:

		//(*Handlers(mainApp)
		void OndbconnectClick(wxCommandEvent& event);
		void OnPanel1Paint(wxPaintEvent& event);
		void OnQuitClick(wxCommandEvent& event);
		void OnPreviousClick(wxCommandEvent& event);
		void OnNextClick(wxCommandEvent& event);
		void OnDeleteClick(wxCommandEvent& event);
		void OnganashowClick(wxCommandEvent& event);
		void OnBasicClick(wxCommandEvent& event);
		void OnVerOftenClick(wxCommandEvent& event);
		void OnOftenClick(wxCommandEvent& event);
		void OnRarelyClick(wxCommandEvent& event);
		void OnChoice1Select(wxCommandEvent& event);
		void OnChoice1Select2(int thechoice);
		void OnNextClick2();
		void OncfrequencySelect(wxCommandEvent& event);
		void OnToolBarUpdateClicked(wxCommandEvent& event);
		void OnToolBarHelpClicked(wxCommandEvent& event);
		void OntbdonateClicked(wxCommandEvent& event);
		void OntbdonateClicked1(wxCommandEvent& event);
		void OntbnihongoClicked(wxCommandEvent& event);
		//*)
        void ConvertGanaInit();
        void ConvertCategoryInit();
		DECLARE_EVENT_TABLE()
};

#endif
