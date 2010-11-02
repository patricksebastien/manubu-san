#include "mainApp.h"

//(*InternalHeaders(mainApp)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(mainApp)
//*)

BEGIN_EVENT_TABLE(mainApp,wxFrame)
	//(*EventTable(mainApp)
	//*)
END_EVENT_TABLE()

mainApp::mainApp(wxWindow* parent)
{
    //DATABASE CONNECTION
    wxSQLite3Database::InitializeSQLite();
    const wxString dbFile = wxGetCwd() + "/dict.sqlite";
    db.Open(dbFile);

    //SPLASH
    wxSQLite3ResultSet qsplash = db.ExecuteQuery("SELECT pref FROM pref WHERE id = 1");
    while (qsplash.NextRow()) {
        if(qsplash.GetInt(0) == 1) {
            wxBitmap bitmap;
            bitmap.LoadFile(wxGetCwd() + "/splash.png", wxBITMAP_TYPE_PNG);
            new wxSplashScreen(bitmap, wxSTAY_ON_TOP|wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 5000, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER);

            //NO MORE SPLASH (only once)
            db.ExecuteQuery("UPDATE pref SET pref = 0 WHERE id = 1");
            //TURN ON TIP
            db.ExecuteQuery("UPDATE pref SET pref = 1 WHERE id = 2");
        }
    }

    //TIP
    wxSQLite3ResultSet qtip = db.ExecuteQuery("SELECT pref FROM pref WHERE id = 2");
    while (qtip.NextRow()) {
        if(qtip.GetInt(0) == 1) {
            Tip();
        }
    }


	//(*Initialize(mainApp)
	wxXmlResource::Get()->LoadObject(this,parent,_T("mainApp"),_T("wxFrame"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	Choice1 = (wxChoice*)FindWindow(XRCID("ID_CHOICE1"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	cfrequency = (wxChoice*)FindWindow(XRCID("ID_CHOICE3"));
	cbrandom = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX1"));
	showromaji = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX3"));
	tatoeba = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX2"));
	displayst = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	stcurrent = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	stof = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT4"));
	stmax = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT5"));
	previous = (wxButton*)FindWindow(XRCID("ID_PREVIOUS"));
	next = (wxButton*)FindWindow(XRCID("ID_NEXT"));
	nbviewst = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT6"));
	Button1 = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Button2 = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));
	Button3 = (wxButton*)FindWindow(XRCID("ID_BUTTON3"));
	Button4 = (wxButton*)FindWindow(XRCID("ID_BUTTON4"));
	deletebutton = (wxButton*)FindWindow(XRCID("ID_BUTTON5"));
	background = (wxPanel*)FindWindow(XRCID("ID_PANEL1"));
	tbnihongo = (wxToolBarToolBase*)FindWindow(XRCID("id_nihongo"));
	ToolBarItem2 = (wxToolBarToolBase*)FindWindow(XRCID("tkanatable"));
	ToolBarItem3 = (wxToolBarToolBase*)FindWindow(XRCID("thelp"));
	ToolBarItem4 = (wxToolBarToolBase*)FindWindow(XRCID("tdonate"));
	ToolBarItem5 = (wxToolBarToolBase*)FindWindow(XRCID("tquit"));
	ToolBar1 = (wxToolBar*)FindWindow(XRCID("ID_TOOLBAR1"));

	Connect(XRCID("ID_CHOICE1"),wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&mainApp::OnChoice1Select);
	Connect(XRCID("ID_CHOICE3"),wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&mainApp::OncfrequencySelect);
	Connect(XRCID("ID_PREVIOUS"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnPreviousClick);
	Connect(XRCID("ID_NEXT"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnNextClick);
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnBasicClick);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnVerOftenClick);
	Connect(XRCID("ID_BUTTON3"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnOftenClick);
	Connect(XRCID("ID_BUTTON4"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnRarelyClick);
	Connect(XRCID("ID_BUTTON5"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainApp::OnDeleteClick);
	Connect(XRCID("id_nihongo"),wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&mainApp::OntbnihongoClicked);
	Connect(XRCID("tkanatable"),wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&mainApp::OnganashowClick);
	Connect(XRCID("thelp"),wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&mainApp::OnToolBarHelpClicked);
	Connect(XRCID("tdonate"),wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&mainApp::OntbdonateClicked1);
	Connect(XRCID("tquit"),wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&mainApp::OnQuitClick);
	//*)

    //OFFSET
    offset = 0;

    //Init HASH TABLE (romaji)
    ConvertGanaInit();

    //Init CategoryHASH TABLE
    ConvertCategoryInit();


    wxSQLite3ResultSet rslastcategory = db.ExecuteQuery("SELECT quitstate from quitstate where id = 1");
    while (rslastcategory.NextRow()) {
        Choice1->SetSelection(rslastcategory.GetInt(0));
        OnChoice1Select2(rslastcategory.GetInt(0));
    }


    //KEYBOARD SHORTCUT
    wxAcceleratorEntry entries[8];
    entries[0].Set(wxACCEL_NORMAL, WXK_LEFT, XRCID("ID_PREVIOUS"));
    entries[1].Set(wxACCEL_NORMAL, WXK_RIGHT, XRCID("ID_NEXT"));
    entries[2].Set(wxACCEL_NORMAL, WXK_DELETE, XRCID("ID_BUTTON5"));
    entries[3].Set(wxACCEL_CTRL, 113, XRCID("tquit"));
    entries[4].Set(wxACCEL_NORMAL, 49, XRCID("ID_BUTTON1"));
    entries[5].Set(wxACCEL_NORMAL, 50, XRCID("ID_BUTTON2"));
    entries[6].Set(wxACCEL_NORMAL, 51, XRCID("ID_BUTTON3"));
    entries[7].Set(wxACCEL_NORMAL, 52, XRCID("ID_BUTTON4"));

    wxAcceleratorTable accel(8, entries);
    this->SetAcceleratorTable(accel);
}

mainApp::~mainApp()
{
	//(*Destroy(mainApp)
	//*)
}

//should be after splash...
void mainApp::Tip() {
    wxTipProvider *tipProvider = wxCreateFileTipProvider(wxGetCwd() + "/tips.txt", 2);
    bool stnt;
    stnt = wxShowTip(this, tipProvider, true);
    if(!stnt) {
        db.ExecuteQuery("UPDATE pref SET pref = 0 WHERE id = 2");
    }
    delete tipProvider;

}

void mainApp::ConvertCategoryInit()
{
    //HIRAGANA HASH
    category["Noun or verb acting prenominally"] = "&adj-f;";
    category["Adjective (keiyoushi)"] = "&adj-i;";
    category["Adjectival nouns or quasi-adjectives (keiyodoshi)"] = "&adj-na;";
    category["Nouns which may take the genitive case particle 'no'"] = "&adj-no;";
    category["Pre-noun adjectival (rentaishi)"] = "&adj-pn;";
    category["'taru' adjective"] = "&adj-t;";
    category["Adverb taking the 'to' particle"] = "&adv-to;";
    category["Adverb (fukushi)"] = "&adv;";
    category["Auxiliary"] = "&aux;";
    category["Auxiliary verb"] = "&aux-v;";
    category["Auxiliary adjective"] = "&aux-adj;";
    category["Conjunction"] = "&conj;";
    category["Counter"] = "&ctr;";
    category["Expressions (phrases, clauses, etc.)"] = "&exp;";
    category["Interjection (kandoushi)"] = "&int;";
    category["Noun (common) (futsuumeishi)"] = "&n;";
    category["Adverbial noun (fukushitekimeishi)"] = "&n-adv;";
    category["Noun, used as a suffix"] = "&n-suf;";
    category["Noun, used as a prefix"] = "&n-pref;";
    category["Noun (temporal) (jisoumeishi)"] = "&n-t;";
    category["Numeric"] = "&num;";
    category["Pronoun"] = "&pn;";
    category["Prefix"] = "&pref;";
    category["Particle"] = "&prt;";
    category["Suffix"] = "&suf;";
    category["Ichidan verb"] = "&v1;";
    category["Nidan verb with 'u' ending (archaic)"] = "&v2a-s;";
    category["Yondan verb with 'hu/fu' ending (archaic)"] = "&v4h;";
    category["Yondan verb with 'ru' ending (archaic)"] = "&v4r;";
    category["Godan verb (not completely classified)"] = "&v5;";
    category["Godan verb - -aru special class"] = "&v5aru;";
    category["Godan verb with 'bu' ending"] = "&v5b;";
    category["Godan verb with 'gu' ending"] = "&v5g;";
    category["Godan verb with 'ku' ending"] = "&v5k;";
    category["Godan verb - Iku/Yuku special class"] = "&v5k-s;";
    category["Godan verb with 'mu' ending"] = "&v5m;";
    category["Godan verb with 'nu' ending"] = "&v5n;";
    category["Godan verb with 'ru' ending"] = "&v5r;";
    category["Godan verb with 'ru' ending (irregular verb)"] = "&v5r-i;";
    category["Godan verb with 'su' ending"] = "&v5s;";
    category["Godan verb with 'tsu' ending"] = "&v5t;";
    category["Godan verb with 'u' ending"] = "&v5u;";
    category["Godan verb with 'u' ending (special class)"] = "&v5u-s;";
    category["Godan verb - Uru old class verb (old form of Eru)"] = "&v5uru;";
    category["Godan verb with 'zu' ending"] = "&v5z;";
    category["Ichidan verb - zuru verb (alternative form of -jiru verbs)"] = "&vz;";
    category["Intransitive verb"] = "&vi;";
    category["Kuru verb - special class"] = "&vk;";
    category["Irregular nu verb"] = "&vn;";
    category["Irregular ru verb, plain form ends with -ri"] = "&vr;";
    category["Noun or participle which takes the aux. verb suru"] = "&vs;";
    category["Suru verb - special class"] = "&vs-s;";
    category["Suru verb - irregular"] = "&vs-i;";
    category["Transitive verb"] = "&vt;";
}

void mainApp::ConvertGanaInit()
{
    //HIRAGANA HASH
    gana[12354] = "A";

    gana[12356] = "I";

    gana[12358] = "U";

    gana[12360] = "E";

    gana[12362] = "O";

    gana[12363] = "KA";

    gana[12365] = "KI";

    gana[12367] = "KU";

    gana[12369] = "KE";

    gana[12371] = "KO";

    gana[12364] = "GA";

    gana[12366] = "GI";

    gana[12368] = "GU";

    gana[12370] = "GE";

    gana[12372] = "GO";

    gana[12373] = "SA";

    gana[12375] = "SHI";

    gana[12377] = "SU";

    gana[12379] = "SE";

    gana[12381] = "SO";

    gana[12374] = "ZA";

    gana[12383] = "TA";

    gana[12385] = "CHI";

    gana[12388] = "TSU";

    gana[12390] = "TE";

    gana[12392] = "TO";

    gana[12376] = "JI";

    gana[12378] = "ZU";

    gana[12380] = "ZE";

    gana[12382] = "ZO";

    gana[12384] = "DA";

    gana[12386] = "DI";

    gana[12389] = "DU";

    gana[12391] = "DE";

    gana[12393] = "DO";

    gana[12394] = "NA";

    gana[12395] = "NI";

    gana[12396] = "NU";

    gana[12397] = "NE";

    gana[12398] = "NO";

    gana[12399] = "HA";

    gana[12402] = "HI";

    gana[12405] = "HU";

    gana[12408] = "HE";

    gana[12411] = "HO";

    gana[12400] = "BA";

    gana[12403] = "BI";

    gana[12406] = "BU";

    gana[12409] = "BE";

    gana[12412] = "BO";

    gana[12401] = "PA";

    gana[12404] = "PI";

    gana[12407] = "PU";

    gana[12410] = "PE";

    gana[12413] = "PO";

    gana[12414] = "MA";

    gana[12415] = "MI";

    gana[12416] = "MU";

    gana[12417] = "ME";

    gana[12418] = "MO";

    gana[12420] = "YA";

    gana[12422] = "YU";

    gana[12424] = "YO";

    gana[12425] = "RA";

    gana[12426] = "RI";

    gana[12427] = "RU";

    gana[12428] = "RE";

    gana[12429] = "RO";

    gana[12431] = "WA";

    gana[12432] = "WI";

    gana[12433] = "WE";

    gana[12434] = "WO";

    gana[12436] = "VU";

    gana[12435] = "n";

    gana[12353] = "a";

    gana[12355] = "i";

    gana[12357] = "u";

    gana[12359] = "e";

    gana[12361] = "o";

    gana[12437] = "ka";

    gana[12438] = "ke";

    gana[12387] = "t";

    gana[12419] = "ya";

    gana[12421] = "yu";

    gana[12423] = "yo";

    gana[12430] = "wa";

    gana[12442] = ".";


    //KATAKANA HASH

    gana[12448] = "==";

    gana[12449] = "a";

    gana[12450] = "A";

    gana[12451] = "i";

    gana[12452] = "I";

    gana[12453] = "u";

    gana[12454] = "U";

    gana[12455] = "e";

    gana[12456] = "E";

    gana[12457] = "o";

    gana[12458] = "O";

    gana[12459] = "KA";

    gana[12460] = "GA";

    gana[12461] = "KI";

    gana[12462] = "GI";

    gana[12463] = "KU";

    gana[12464] = "GU";

    gana[12465] = "KE";

    gana[12466] = "GE";

    gana[12467] = "KO";

    gana[12468] = "GO";

    gana[12469] = "SA";

    gana[12470] = "ZA";

    gana[12471] = "SHI";

    gana[12472] = "JI";

    gana[12473] = "SU";

    gana[12474] = "ZU";

    gana[12475] = "SE";

    gana[12476] = "ZE";

    gana[12477] = "SO";

    gana[12478] = "ZO";

    gana[12479] = "TA";

    gana[12480] = "DA";

    gana[12481] = "CHI";

    gana[12482] = "JI";

    gana[12483] = "t";

    gana[12484] = "TSU";

    gana[12485] = "ZU";

    gana[12486] = "TE";

    gana[12487] = "DE";

    gana[12488] = "TO";

    gana[12489] = "DO";

    gana[12490] = "NA";

    gana[12491] = "NI";

    gana[12492] = "NU";

    gana[12493] = "NE";

    gana[12494] = "NO";

    gana[12495] = "HA";

    gana[12496] = "BA";

    gana[12497] = "PA";

    gana[12498] = "HI";

    gana[12499] = "BI";

    gana[12500] = "PI";

    gana[12501] = "FU";

    gana[12502] = "BU";

    gana[12503] = "PU";

    gana[12504] = "HE";

    gana[12505] = "BE";

    gana[12506] = "PE";

    gana[12507] = "HO";

    gana[12508] = "BO";

    gana[12509] = "PO";

    gana[12510] = "MA";

    gana[12511] = "MI";

    gana[12512] = "MU";

    gana[12513] = "ME";

    gana[12514] = "MO";

    gana[12515] = "ya";

    gana[12516] = "YA";

    gana[12517] = "yu";

    gana[12518] = "YU";

    gana[12519] = "yo";

    gana[12520] = "YO";

    gana[12521] = "RA";

    gana[12522] = "RI";

    gana[12523] = "RU";

    gana[12524] = "RE";

    gana[12525] = "RO";

    gana[12526] = "wa";

    gana[12527] = "WA";

    gana[12528] = "WI";

    gana[12529] = "WE";

    gana[12530] = "WO";

    gana[12531] = "n";

    gana[12532] = "VU";

    gana[12533] = "ka";

    gana[12534] = "ke";

    gana[12535] = "VA";

    gana[12536] = "VI";

    gana[12537] = "VE";

    gana[12538] = "VO";

    gana[12539] = "・";

    gana[12540] = "-";

    gana[12541] = "ヽ";

    gana[12542] = "ヾ";

    gana[12543] = "KOTO";

}

wxString mainApp::ConvertGana(wxString ganatoconvert)
{
    wxString hirakata = ganatoconvert;
    size_t len = hirakata.length();
    wxString romaji;
    for(size_t i = 0; i < len; i++) {
        romaji.Append(gana[hirakata.GetChar(i).GetValue()]);
    }

    //exceptions...
    romaji.Replace("KIya", "KYA");
    romaji.Replace("KIyu", "KYU");
    romaji.Replace("KIyo", "KYO");

    romaji.Replace("SHIya", "SHA");
    romaji.Replace("SHIyu", "SHU");
    romaji.Replace("SHIyo", "SHO");

    romaji.Replace("CHIya", "CHA");
    romaji.Replace("CHIyu", "CHU");
    romaji.Replace("CHIyo", "CHO");

    romaji.Replace("NIya", "NYA");
    romaji.Replace("NIyu", "NYU");
    romaji.Replace("NIyo", "NYO");

    romaji.Replace("HIya", "HYA");
    romaji.Replace("HIyu", "HYU");
    romaji.Replace("HIyo", "HYO");

    romaji.Replace("MIya", "MYA");
    romaji.Replace("MIyu", "MYU");
    romaji.Replace("MIyo", "MYO");

    romaji.Replace("RIya", "RYA");
    romaji.Replace("RIyu", "RYU");
    romaji.Replace("RIyo", "RYO");

    romaji.Replace("GIya", "GYA");
    romaji.Replace("GIyu", "GYU");
    romaji.Replace("GIyo", "GYO");

    romaji.Replace("JIya", "JA");
    romaji.Replace("JIyu", "JU");
    romaji.Replace("JIyo", "JO");

    romaji.Replace("BIya", "BYA");
    romaji.Replace("BIyu", "BYU");
    romaji.Replace("BIyo", "BYO");

    romaji.Replace("PIya", "PYA");
    romaji.Replace("PIyu", "PYU");
    romaji.Replace("PIyo", "PYO");

    romaji.Replace("tD", "DD");
    romaji.Replace("tH", "HH");
    romaji.Replace("tT", "TT");
    romaji.Replace("tK", "KK");
    romaji.Replace("tP", "PP");
    romaji.Replace("tB", "BB");
    romaji.Replace("tS", "SS");
    romaji.Replace("tC", "CC");
    romaji.Replace("CCH", "TCH");

    romaji.Replace("n", "N'");

    //katakana exception
    romaji.Replace("A-", "AA");
    romaji.Replace("I-", "II");
    romaji.Replace("U-", "UU");
    romaji.Replace("E-", "EE");
    romaji.Replace("O-", "OO");

    romaji.Replace("AA", "&#257;");
    romaji.Replace("EE", "&#275;");
    romaji.Replace("OO", "&#333;");
    romaji.Replace("OU", "&#333;");
    romaji.Replace("UU", "&#363;");

    //TODO
    //KliKli
    //IF LAST LETTER == ' REMOVE

    return romaji.MakeLower();
}

wxString mainApp::ConvertCategory(wxString categorytoconvert)
{
    wxString dbcat;
    dbcat = category[categorytoconvert];
    return dbcat;
}

void mainApp::OnQuitClick(wxCommandEvent& event)
{
    wxString categorychoice;
    categorychoice << Choice1->GetCurrentSelection();

    //Category
    db.ExecuteQuery("UPDATE quitstate SET quitstate = "+categorychoice+" where id = 1");

    //Frequency

    wxString offsetqs;
    offsetqs << offset;

    //Cursor
    db.ExecuteQuery("UPDATE quitstate SET quitstate = "+offsetqs+" where id = 2");

    //MAYBE RANDOM, ROMAJI, TATEOBA
    Close();
}

void mainApp::OnPreviousClick(wxCommandEvent& event)
{
    offset--;
    if(offset == -1) {
            offset = max - 1;
    }
    wxString offsettxt = wxString() << offset;

    wxSQLite3ResultSet rsnext = db.ExecuteQuery(requete+" LIMIT 1 OFFSET "+offsettxt);

    while (rsnext.NextRow()) {

        wxString main_id;
        main_id << rsnext.GetInt(0);
        g_main_id = rsnext.GetInt(0);

        //update nbview
        db.ExecuteQuery("UPDATE main SET nbview = nbview + 1 where id = "+main_id);

        wxSQLite3ResultSet rsnbview = db.ExecuteQuery("SELECT nbview from main WHERE id = "+main_id);
        while (rsnbview.NextRow()) {
            wxString nbviewaff;
            nbviewaff << rsnbview.GetInt(0);
            nbviewst->SetLabelText("Viewed: "+nbviewaff);
        }

        //frequency button highlight
        wxSQLite3ResultSet rsmain = db.ExecuteQuery("SELECT frequency_id from main WHERE id = "+main_id);
        while (rsmain.NextRow()) {
            switch(rsmain.GetInt(0)) {
                case 1:
                    Button1->SetBackgroundColour(*wxBLUE);
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 2:
                    Button2->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 3:
                    Button3->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 4:
                    Button4->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
                default:
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
            }
        }

        wxSQLite3ResultSet rskanji = db.ExecuteQuery("SELECT kanji from kanji WHERE main_id = "+main_id);

        displayst->SetPage("<html><body BGCOLOR=\"#f3f3f3\"><p align=\"center\"><font size=\"8\">");
        while (rskanji.NextRow()) {
            displayst->AppendToPage(rskanji.GetAsString(0));
            displayst->AppendToPage("<br />");
        }

        wxSQLite3ResultSet rskana = db.ExecuteQuery("SELECT hirakata from hirakata WHERE main_id = "+main_id);
        displayst->AppendToPage("<br />");
        while (rskana.NextRow()) {
            displayst->AppendToPage("<font size=\"8\" color=\"#000000\">");
            displayst->AppendToPage(rskana.GetAsString(0));
            displayst->AppendToPage("</font><br />");

             if(showromaji->GetValue()) {
                displayst->AppendToPage("<font size=\"5\" color=\"#6D7B8D\">");
                displayst->AppendToPage(ConvertGana(rskana.GetAsString(0)));
                displayst->AppendToPage("</font><br />");
             }
        }

        wxSQLite3ResultSet rsenglish = db.ExecuteQuery("SELECT english from english WHERE main_id = "+main_id);
        displayst->AppendToPage("</font><font size=\"5\" color=\"#000000\"><hr style=\"width: 350px\"><center>");
        while (rsenglish.NextRow()) {
            displayst->AppendToPage(rsenglish.GetAsString(0));
            displayst->AppendToPage("<hr style=\"width: 350px\"><center><br />");
        }

    }
    stcurrent->SetLabelText(wxString() << offset + 1);
}

void mainApp::OnNextClick(wxCommandEvent& event)
{
    offset++;
    if(offset == max) {
            offset = 0;
    }
    wxString offsettxt = wxString() << offset;

    wxSQLite3ResultSet rsnext = db.ExecuteQuery(requete+" LIMIT 1 OFFSET "+offsettxt);

    while (rsnext.NextRow()) {

        wxString main_id;
        main_id << rsnext.GetInt(0);
        g_main_id = rsnext.GetInt(0);

        //update nbview
        db.ExecuteQuery("UPDATE main SET nbview = nbview + 1 where id = "+main_id);


        wxSQLite3ResultSet rsnbview = db.ExecuteQuery("SELECT nbview from main WHERE id = "+main_id);
        while (rsnbview.NextRow()) {
            wxString nbviewaff;
            nbviewaff << rsnbview.GetInt(0);
            nbviewst->SetLabelText("Viewed: "+nbviewaff);
        }


        //frequency button highlight
        wxSQLite3ResultSet rsmain = db.ExecuteQuery("SELECT frequency_id from main WHERE id = "+main_id);
        while (rsmain.NextRow()) {
            switch(rsmain.GetInt(0)) {
                case 1:
                    Button1->SetBackgroundColour(*wxBLUE);
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 2:
                    Button2->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 3:
                    Button3->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 4:
                    Button4->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
                default:
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
            }
        }


        wxSQLite3ResultSet rskanji = db.ExecuteQuery("SELECT kanji from kanji WHERE main_id = "+main_id);

        displayst->SetPage("<html><body BGCOLOR=\"#f3f3f3\"><p align=\"center\"><font size=\"8\">");
        while (rskanji.NextRow()) {
            displayst->AppendToPage(rskanji.GetAsString(0));
            displayst->AppendToPage("<br />");
        }

        wxSQLite3ResultSet rskana = db.ExecuteQuery("SELECT hirakata from hirakata WHERE main_id = "+main_id);
        displayst->AppendToPage("<br />");
        while (rskana.NextRow()) {
            if(tatoeba->GetValue()) {
                wxLaunchDefaultBrowser("http://tatoeba.org/eng/sentences/search?from=jpn&to=eng&query="+rskana.GetAsString(0));
            }
            //create temp file with the hiragana
            wxTextFile tmp;
            tmp.Create("ja.txt");
            tmp.AddLine(rskana.GetAsString(0));
            tmp.Write();
            tmp.Close();
            wxExecute("open_jtalk -td /usr/lib/hts-voice/nitech-jp-atr503-m001/tree-dur.inf -tf /usr/lib/hts-voice/nitech-jp-atr503-m001/tree-lf0.inf -tm /usr/lib/hts-voice/nitech-jp-atr503-m001/tree-mgc.inf -md /usr/lib/hts-voice/nitech-jp-atr503-m001/dur.pdf -mf /usr/lib/hts-voice/nitech-jp-atr503-m001/lf0.pdf -mm /usr/lib/hts-voice/nitech-jp-atr503-m001/mgc.pdf -df /usr/lib/hts-voice/nitech-jp-atr503-m001/lf0.win1 -df /usr/lib/hts-voice/nitech-jp-atr503-m001/lf0.win2 -df /usr/lib/hts-voice/nitech-jp-atr503-m001/lf0.win3 -dm /usr/lib/hts-voice/nitech-jp-atr503-m001/mgc.win1 -dm /usr/lib/hts-voice/nitech-jp-atr503-m001/mgc.win2 -dm /usr/lib/hts-voice/nitech-jp-atr503-m001/mgc.win3 -ef /usr/lib/hts-voice/nitech-jp-atr503-m001/tree-gv-lf0.inf -em /usr/lib/hts-voice/nitech-jp-atr503-m001/tree-gv-mgc.inf -cf /usr/lib/hts-voice/nitech-jp-atr503-m001/gv-lf0.pdf -cm /usr/lib/hts-voice/nitech-jp-atr503-m001/gv-mgc.pdf -k /usr/lib/hts-voice/nitech-jp-atr503-m001/gv-switch.inf -x /usr/lib/open_jtalk/dic/utf-8 -ow japanesevoice.wav ja.txt", wxEXEC_SYNC);
            wxExecute("sndfile-play japanesevoice.wav");

            displayst->AppendToPage("<font size=\"8\" color=\"#000000\">");
            displayst->AppendToPage(rskana.GetAsString(0));
            displayst->AppendToPage("</font><br />");

            if(showromaji->GetValue()) {
                displayst->AppendToPage("<font size=\"5\" color=\"#6D7B8D\">");
                displayst->AppendToPage(ConvertGana(rskana.GetAsString(0)));
                displayst->AppendToPage("</font><br />");
            }
        }

        wxSQLite3ResultSet rsenglish = db.ExecuteQuery("SELECT english from english WHERE main_id = "+main_id);
        displayst->AppendToPage("</font><font size=\"5\" color=\"#000000\"><hr style=\"width: 350px\"><center>");
        while (rsenglish.NextRow()) {
            displayst->AppendToPage(rsenglish.GetAsString(0));
            displayst->AppendToPage("<hr style=\"width: 350px\"><center><br />");
        }

    }
    stcurrent->SetLabelText(wxString() << offset + 1);

}

void mainApp::OnNextClick2()
{
    //fetch last cursor
    wxSQLite3ResultSet rslastcursor = db.ExecuteQuery("SELECT quitstate from quitstate where id = 2");
    while (rslastcursor.NextRow()) {
        offset = rslastcursor.GetInt(0);
        offset--;
    }

    offset++;
    if(offset == max) {
            offset = 0;
    }
    wxString offsettxt = wxString() << offset;
    wxSQLite3ResultSet rsnext = db.ExecuteQuery(requete+" LIMIT 1 OFFSET "+offsettxt);

    while (rsnext.NextRow()) {

        wxString main_id;
        main_id << rsnext.GetInt(0);
        g_main_id = rsnext.GetInt(0);

        //frequency button highlight
        wxSQLite3ResultSet rsmain = db.ExecuteQuery("SELECT frequency_id from main WHERE id = "+main_id);
        while (rsmain.NextRow()) {
            switch(rsmain.GetInt(0)) {
                case 1:
                    Button1->SetBackgroundColour(*wxBLUE);
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 2:
                    Button2->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 3:
                    Button3->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                break;
                case 4:
                    Button4->SetBackgroundColour(*wxBLUE);
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
                default:
                    Button4->SetBackgroundColour(wxColour(212,208,199));
                    Button1->SetBackgroundColour(wxColour(212,208,199));
                    Button2->SetBackgroundColour(wxColour(212,208,199));
                    Button3->SetBackgroundColour(wxColour(212,208,199));
                break;
            }
        }


        wxSQLite3ResultSet rskanji = db.ExecuteQuery("SELECT kanji from kanji WHERE main_id = "+main_id);

        displayst->SetPage("<html><body BGCOLOR=\"#f3f3f3\"><p align=\"center\"><font size=\"8\">");
        while (rskanji.NextRow()) {
            displayst->AppendToPage(rskanji.GetAsString(0));
            displayst->AppendToPage("<br />");
        }

        wxSQLite3ResultSet rskana = db.ExecuteQuery("SELECT hirakata from hirakata WHERE main_id = "+main_id);
        displayst->AppendToPage("<br />");
        while (rskana.NextRow()) {
            if(tatoeba->GetValue()) {
                wxLaunchDefaultBrowser("http://tatoeba.org/eng/sentences/search?from=jpn&to=eng&query="+rskana.GetAsString(0));
            }
            displayst->AppendToPage("<font size=\"8\" color=\"#000000\">");
            displayst->AppendToPage(rskana.GetAsString(0));
            displayst->AppendToPage("</font><br />");

            if(showromaji->GetValue()) {
                displayst->AppendToPage("<font size=\"5\" color=\"#6D7B8D\">");
                displayst->AppendToPage(ConvertGana(rskana.GetAsString(0)));
                displayst->AppendToPage("</font><br />");
            }
        }

        wxSQLite3ResultSet rsenglish = db.ExecuteQuery("SELECT english from english WHERE main_id = "+main_id);
        displayst->AppendToPage("</font><font size=\"5\" color=\"#000000\"><hr style=\"width: 350px\"><center>");
        while (rsenglish.NextRow()) {
            displayst->AppendToPage(rsenglish.GetAsString(0));
            displayst->AppendToPage("<hr style=\"width: 350px\"><center><br />");
        }

    }
    stcurrent->SetLabelText(wxString() << offset + 1);

}

void mainApp::OnDeleteClick(wxCommandEvent& event)
{
        wxString main_id;
        main_id << g_main_id;
        db.ExecuteQuery("DELETE from main where id = "+main_id);
        db.ExecuteQuery("DELETE from kanji where main_id = "+main_id);
        db.ExecuteQuery("DELETE from hirakata where main_id = "+main_id);
        db.ExecuteQuery("DELETE from english where main_id = "+main_id);
        db.ExecuteQuery("DELETE from category where main_id = "+main_id);
        offset--;
        OnNextClick(event);

}

void mainApp::OnganashowClick(wxCommandEvent& event)
{
    GanaFrame* frm = new GanaFrame(this);
    frm->Show();
}

void mainApp::OnBasicClick(wxCommandEvent& event)
{
    wxString main_id;
    main_id << g_main_id;
    db.ExecuteQuery("UPDATE main SET frequency_id = 1 WHERE id = "+main_id);
    Button1->SetBackgroundColour(*wxBLUE);
    Button2->SetBackgroundColour(wxColour(212,208,199));
    Button3->SetBackgroundColour(wxColour(212,208,199));
    Button4->SetBackgroundColour(wxColour(212,208,199));
}

void mainApp::OnVerOftenClick(wxCommandEvent& event)
{
    wxString main_id;
    main_id << g_main_id;
    db.ExecuteQuery("UPDATE main SET frequency_id = 2 WHERE id = "+main_id);
    Button2->SetBackgroundColour(*wxBLUE);
    Button1->SetBackgroundColour(wxColour(212,208,199));
    Button3->SetBackgroundColour(wxColour(212,208,199));
    Button4->SetBackgroundColour(wxColour(212,208,199));
}

void mainApp::OnOftenClick(wxCommandEvent& event)
{
    wxString main_id;
    main_id << g_main_id;
    db.ExecuteQuery("UPDATE main SET frequency_id = 3 WHERE id = "+main_id);
    Button3->SetBackgroundColour(*wxBLUE);
    Button1->SetBackgroundColour(wxColour(212,208,199));
    Button2->SetBackgroundColour(wxColour(212,208,199));
    Button4->SetBackgroundColour(wxColour(212,208,199));
}

void mainApp::OnRarelyClick(wxCommandEvent& event)
{
    wxString main_id;
    main_id << g_main_id;
    db.ExecuteQuery("UPDATE main SET frequency_id = 4 WHERE id = "+main_id);
    Button4->SetBackgroundColour(*wxBLUE);
    Button1->SetBackgroundColour(wxColour(212,208,199));
    Button2->SetBackgroundColour(wxColour(212,208,199));
    Button3->SetBackgroundColour(wxColour(212,208,199));
}

void mainApp::OnChoice1Select(wxCommandEvent& event)
{

    //FREQUENCY = ALL BY DEFAULT
    cfrequency->SetSelection(0);

    //ORDER BY
    wxString order;
    if(cbrandom->GetValue()) {
        order = "RANDOM()";
    } else {
        order = "main_id";
    }

    //SELECT CATEGORY FROM DB
    if(ConvertCategory(event.GetString()) == "&n;") {
        requete = "SELECT main_id FROM category WHERE category = '&n;' AND main_id NOT IN (SELECT main_id FROM category WHERE category != '&n;') ORDER BY "+order;
    } else {
        requete = "SELECT main_id FROM category WHERE category = '"+ConvertCategory(event.GetString())+"' ORDER BY "+order;
    }


    //COUNT NB OF RECORD
    wxSQLite3ResultSet nbofrecord = db.ExecuteQuery("SELECT count(main_id) FROM ("+requete+");");

    //SET STATIC TEXT
    while (nbofrecord.NextRow()) {
        max = nbofrecord.GetInt(0);

        wxString nbrs = wxString() << nbofrecord.GetInt(0);
        stmax->SetLabelText(nbrs);
        stof->SetLabelText("of");
        stcurrent->SetLabelText("1");
        offset = -1;
        OnNextClick(event);
    }
    next->Enable();
    previous->Enable();
    Button1->Enable();
    Button2->Enable();
    Button3->Enable();
    Button4->Enable();
    deletebutton->Enable();
    cfrequency->Enable();
}

void mainApp::OnChoice1Select2(int thechoice)
{
    //FREQUENCY = ALL BY DEFAULT
    cfrequency->SetSelection(0);

    //ORDER BY
    wxString order;
    if(cbrandom->GetValue()) {
        order = "RANDOM()";
    } else {
        order = "main_id";
    }

    //SELECT CATEGORY FROM DB
    if(ConvertCategory(Choice1->GetString(thechoice)) == "&n;") {
        requete = "SELECT main_id FROM category WHERE category = '&n;' AND main_id NOT IN (SELECT main_id FROM category WHERE category != '&n;') ORDER BY "+order;
    } else {
        requete = "SELECT main_id FROM category WHERE category = '"+ConvertCategory(Choice1->GetString(thechoice))+"' ORDER BY "+order;
    }


    //COUNT NB OF RECORD
    wxSQLite3ResultSet nbofrecord = db.ExecuteQuery("SELECT count(main_id) FROM ("+requete+");");

    //SET STATIC TEXT
    while (nbofrecord.NextRow()) {
        max = nbofrecord.GetInt(0);

        wxString nbrs = wxString() << nbofrecord.GetInt(0);
        stmax->SetLabelText(nbrs);
        stof->SetLabelText("of");
        stcurrent->SetLabelText("1");
        OnNextClick2();
    }
    next->Enable();
    previous->Enable();
    Button1->Enable();
    Button2->Enable();
    Button3->Enable();
    Button4->Enable();
    deletebutton->Enable();
    cfrequency->Enable();

}

void mainApp::OncfrequencySelect(wxCommandEvent& event)
{
    //ORDER BY
    wxString order;
    if(cbrandom->GetValue()) {
        order = "RANDOM()";
    } else {
        order = "main_id";
    }
    //SELECT CATEGORY FROM DB
    wxString cs;
    cs << cfrequency->GetCurrentSelection();
    if(cfrequency->GetCurrentSelection() == 0) {
        requete = "SELECT category.main_id FROM category WHERE category.category = '"+ConvertCategory(Choice1->GetString(Choice1->GetCurrentSelection()))+"' ORDER BY "+order;
    } else {
        requete = "SELECT category.main_id FROM category LEFT JOIN main ON main.id = category.main_id WHERE main.frequency_id = "+cs+" AND category.category = '"+ConvertCategory(Choice1->GetString(Choice1->GetCurrentSelection()))+"' ORDER BY "+order;
    }
    //COUNT NB OF RECORD
    wxSQLite3ResultSet nbofrecord = db.ExecuteQuery("SELECT count(main_id) FROM ("+requete+");");

    //SET STATIC TEXT
    while (nbofrecord.NextRow()) {
        max = nbofrecord.GetInt(0);

        //bad hack
        if(max == 0) {
            wxMessageBox("There's no word in that frequency category, switching back to ALL");
            cfrequency->SetSelection(0);
            requete = "SELECT category.main_id FROM category WHERE category.category = '"+ConvertCategory(Choice1->GetString(Choice1->GetCurrentSelection()))+"' ORDER BY "+order;
            wxSQLite3ResultSet nbofrecord2 = db.ExecuteQuery("SELECT count(main_id) FROM ("+requete+");");
            while (nbofrecord2.NextRow()) {
               max = nbofrecord2.GetInt(0);
            }
        }
        wxString nbrs = wxString() << max;
        stmax->SetLabelText(nbrs);
        stof->SetLabelText("of");
        stcurrent->SetLabelText("1");
        offset = -1;
        OnNextClick(event);
    }
}

void mainApp::OnToolBarHelpClicked(wxCommandEvent& event)
{
    helpFrame* frmh = new helpFrame(this);
    frmh->Show();
}

void mainApp::OntbdonateClicked1(wxCommandEvent& event)
{
    wxMessageBox("Really? Thank you so much!!!!!!");
    wxLaunchDefaultBrowser("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8MHRXDZ3C2HW");
}

void mainApp::OntbnihongoClicked(wxCommandEvent& event)
{
    NihongoFrame* frmh = new NihongoFrame(this);
    frmh->Show();
}
