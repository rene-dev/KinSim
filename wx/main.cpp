#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "easygl.h"
#include "interpolator.h"
#include "gcode.h"

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

class glview: public wxGLCanvas {
        void Render();
public:
    glview(wxFrame* parent);
    easygl renderer;
    void Paintit(wxPaintEvent& event);
    void init();
protected:
    DECLARE_EVENT_TABLE()
};
 
BEGIN_EVENT_TABLE(glview, wxGLCanvas)
    EVT_PAINT(glview::Paintit)
END_EVENT_TABLE()
 
glview::glview(wxFrame *parent):wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxSize(100,100), 0, wxT("GLCanvas")){
    init();
    int argc = 1;
    char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
}
 
 
void glview::Paintit(wxPaintEvent& WXUNUSED(event)){
    Render();
}

void glview::init(){
    renderer.init();
    renderer.currentPath = gcode("/Users/rene/dev/kinsim/kinsim/kinsim/gcode.ngc");
    interpol(renderer.currentPath);
    renderer.robotState = &renderer.currentPath->pos;
}
 
void glview::Render()
{
    SetCurrent();
    wxPaintDC(this);
    renderer.viewportSize.x = (GLint)GetSize().x;
    renderer.viewportSize.y = (GLint)GetSize().y;
    renderer.draw(1);
    glFlush();
    SwapBuffers();
}
 
class Sim: public wxApp
{
    virtual bool OnInit();
    wxGLCanvas * glcanvas;
};

IMPLEMENT_APP(Sim)

bool Sim::OnInit()
{        
    wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
    wxFrame *frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Sim"), wxDefaultPosition, wxSize(800,600));
    glcanvas = new glview(frame);
    wxSizerFlags controlflags = wxSizerFlags(0).Border(wxALL, 3);
    box->Add(glcanvas,wxSizerFlags(1).Expand().Border(wxALL, 3));
    box->Add(new wxButton(frame, 0, wxT("Hello 1"),wxDefaultPosition, wxDefaultSize, 0),controlflags);
    box->Add(new wxButton(frame, 0, wxT("Hello 2"),wxDefaultPosition, wxDefaultSize, 0),controlflags);
    box->Add(new wxButton(frame, 0, wxT("Hello 3"),wxDefaultPosition, wxDefaultSize, 0),controlflags);
    box->Add(new wxSlider (frame, 0, 30, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, wxT("foo")),controlflags);
    box->Add(new wxSlider (frame, 0, 60, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, wxT("foo")),controlflags);
    box->Add(new wxSlider (frame, 0, 90, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, wxT("foo")),controlflags);
    frame->SetSizer(box);
    frame->Layout();

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuHelp, "&Help" );
    frame->SetMenuBar( menuBar );
    
    frame->CreateStatusBar();
    frame->SetStatusText("Statuskram und so");
 
    frame->Show(TRUE);
    return TRUE;
}
