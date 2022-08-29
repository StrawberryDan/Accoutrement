#include "wx/wx.h"
#include "wx/sizer.h"



class MainUI : public wxFrame
{
public:
    MainUI()
    : wxFrame(nullptr, wxID_ANY, "Accoutrement")
    {
        auto mainSizer = new wxBoxSizer(wxHORIZONTAL);


        auto songCategories = new wxBoxSizer(wxVERTICAL);
        songCategories->Add(new wxStaticText(this, wxID_ANY, "Song Categories"), wxSizerFlags(0).CenterHorizontal().Border());
        songCategories->Add(new wxListBox(this, wxID_ANY), wxSizerFlags(1).Expand().Border());
        auto songCategoriesButtons = new wxGridSizer(3, 3, {5, 5});
        songCategoriesButtons->Add(new wxButton(this, wxID_ANY, "Create"), wxSizerFlags(1).Expand());
        songCategoriesButtons->Add(new wxButton(this, wxID_ANY, "Delete"), wxSizerFlags(1).Expand());
        songCategoriesButtons->Add(new wxButton(this, wxID_ANY, "Rename"), wxSizerFlags(1).Expand());
        songCategories->Add(songCategoriesButtons, wxSizerFlags(0).Border().Expand());
        mainSizer->Add(songCategories, wxSizerFlags(1).Expand());


        auto songs = new wxBoxSizer(wxVERTICAL);
        songs->Add(new wxStaticText(this, wxID_ANY, "Songs"), wxSizerFlags(0).CenterHorizontal().Border());
        songs->Add(new wxListBox(this, wxID_ANY), wxSizerFlags(1).Expand().Border());
        auto songsButtons = new wxGridSizer(3, 3, {5, 5});
        songsButtons->Add(new wxButton(this, wxID_ANY, "Add"), wxSizerFlags(1).Expand());
        songsButtons->Add(new wxButton(this, wxID_ANY, "Remove"), wxSizerFlags(1).Expand());
        songsButtons->Add(new wxButton(this, wxID_ANY, "Rename"), wxSizerFlags(1).Expand());
        songs->Add(songsButtons, wxSizerFlags(0).Border().Expand());
        mainSizer->Add(songs, wxSizerFlags(1).Expand());


        auto playlist = new wxBoxSizer(wxVERTICAL);
        playlist->Add(new wxStaticText(this, wxID_ANY, "Playlist"), wxSizerFlags(0).CenterHorizontal().Border());
        playlist->Add(new wxListBox(this, wxID_ANY), wxSizerFlags(1).Expand().Border());
        auto playlistButtons = new wxGridSizer(3, 3, {5, 5});
        playlistButtons->Add(new wxButton(this, wxID_ANY, "Add"), wxSizerFlags(1).Expand());
        playlistButtons->Add(new wxButton(this, wxID_ANY, "Remove"), wxSizerFlags(1).Expand());
        playlistButtons->Add(new wxButton(this, wxID_ANY, "Rename"), wxSizerFlags(1).Expand());
        playlist->Add(playlistButtons, wxSizerFlags(0).Border().Expand());
        mainSizer->Add(playlist, wxSizerFlags(1).Expand());


        auto sfx = new wxBoxSizer(wxVERTICAL);
        sfx->Add(new wxStaticText(this, wxID_ANY, "SFX"), wxSizerFlags(0).CenterHorizontal().Border());
        sfx->Add(new wxListBox(this, wxID_ANY), wxSizerFlags(1).Expand().Border());
        auto sfxButtons = new wxGridSizer(3, 3, {5, 5});
        sfxButtons->Add(new wxButton(this, wxID_ANY, "Add"), wxSizerFlags(1).Expand());
        sfxButtons->Add(new wxButton(this, wxID_ANY, "Remove"), wxSizerFlags(1).Expand());
        sfxButtons->Add(new wxButton(this, wxID_ANY, "Rename"), wxSizerFlags(1).Expand());
        sfx->Add(sfxButtons, wxSizerFlags(0).Border().Expand());
        mainSizer->Add(sfx, wxSizerFlags(1).Expand());


        auto sfxGroups = new wxBoxSizer(wxVERTICAL);
        sfxGroups->Add(new wxStaticText(this, wxID_ANY, "SFX Groups"), wxSizerFlags(0).CenterHorizontal().Border());
        sfxGroups->Add(new wxListBox(this, wxID_ANY), wxSizerFlags(1).Expand().Border());
        auto sfxGroupsButtons = new wxGridSizer(3, 3, {5, 5});
        sfxGroupsButtons->Add(new wxButton(this, wxID_ANY, "Add"), wxSizerFlags(1).Expand());
        sfxGroupsButtons->Add(new wxButton(this, wxID_ANY, "Remove"), wxSizerFlags(1).Expand());
        sfxGroupsButtons->Add(new wxButton(this, wxID_ANY, "Rename"), wxSizerFlags(1).Expand());
        sfxGroups->Add(sfxGroupsButtons, wxSizerFlags(0).Border().Expand());
        mainSizer->Add(sfxGroups, wxSizerFlags(1).Expand());


        SetSizerAndFit(mainSizer);
    }

private:
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainUI, wxFrame)
wxEND_EVENT_TABLE()



class Accoutrement : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MainUI();
        frame->Show(true);
        return true;
    }
};



wxIMPLEMENT_APP(Accoutrement);