#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

#include <unistd.h>
#include <signal.h>



class Button : Fl_Button {
public:
    Button(int x, int y, int w, int h, const char *lb)
        : Fl_Button(x, y, w, h, lb)
        { callback(ButtonCallback, 0); }
    virtual ~Button() {}
    virtual void OnPress() {}
private:
    static void ButtonCallback(Fl_Widget *w, void *user)
        { static_cast<Button*>(w)->OnPress(); }
};



class LedCtrlButton : public Button {
    int signo;
public:
    enum button_sizes {
        button_h = 30,
        button_w = 100
    };
    LedCtrlButton(int x, int y, const char *lb, int sig)
        : Button(x, y, button_w, button_h, lb), signo(sig) {}
private:
    virtual void OnPress() {
        kill(getppid(), signo);
    }
};



class ButtonGreen : public LedCtrlButton {
public:
    ButtonGreen(int x, int y) : LedCtrlButton(x, y, "GREEN", SIGUSR1) {}
};

class ButtonRed : public LedCtrlButton {
public:
    ButtonRed(int x, int y) : LedCtrlButton(x, y, "RED", SIGUSR2) {}
};





class MainWindow : Fl_Window {
    Fl_Box *box[2];
    Button *button[2];
    enum buttons_box_sizes {
        bspace_h = LedCtrlButton::button_h*2,
        bspace_w = LedCtrlButton::button_w*3
    };
    enum label_box_sizes {
        lbox_h = bspace_h*3/2,
        lbox_w = bspace_w
    };
    enum window_sizes {
        win_h = lbox_h + bspace_h,
        win_w = lbox_w
    };
public:
    MainWindow();
    void Show() { show(); }
};



MainWindow::MainWindow()
    : Fl_Window(win_w, win_h, "User interface")
{
    const char *message = "What the led do you want\nto turn on?";
    box[0] = new Fl_Box(0, 0, lbox_w, lbox_h, message);
    box[0]->labelsize(18);
    box[0]->box(FL_UP_BOX);

    int b_y = bspace_h/4 + lbox_h;
    int spacing = bspace_w/(3*3);
    box[1] = new Fl_Box(0, lbox_h, bspace_w, bspace_h);
    button[0] = new ButtonGreen(spacing, b_y);
    button[1] = new ButtonRed(LedCtrlButton::button_w + spacing*2, b_y);
    box[1]->box(FL_UP_BOX);

    end();
}




int main()
{
    MainWindow *w = new MainWindow;
    w->Show();
    return Fl::run();
}
