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



class LedControlButton : public Button {
    int signo;
public:
    enum button_sizes {
        button_h = 30,
        button_w = 100
    };
    LedControlButton(int x, int y, const char *lb, int sig)
        : Button(x, y, button_w, button_h, lb), signo(sig) {}
private:
    virtual void OnPress() {
        kill(getppid(), signo);
    }
};



class ButtonGreen : public LedControlButton {
public:
    ButtonGreen(int x, int y)
        : LedControlButton(x, y, "GREEN", SIGUSR1) {}
};

class ButtonRed : public LedControlButton {
public:
    ButtonRed(int x, int y)
        : LedControlButton(x, y, "RED", SIGUSR2) {}
};





class MainWindow : Fl_Window {
    Fl_Box *box[2];
    Button *button[2];
    enum buttons_box_sizes {
        bspace_h = LedControlButton::button_h*2,
        bspace_w = LedControlButton::button_w*3
    };
    enum label_box_sizes {
        lbox_h = bspace_h*3/2,
        lbox_w = bspace_w
    };
    enum window_sizes {
        win_h = lbox_h + bspace_h,
        win_w = lbox_w
    };
    enum { font_size = 18 };
public:
    MainWindow();
    void Show() { show(); }
};



MainWindow::MainWindow()
    : Fl_Window(win_w, win_h, "User interface")
{
    const char *message = "What the led do you\nwant to turn on?";
    box[0] = new Fl_Box(0, 0, lbox_w, lbox_h, message);
    box[0]->labelsize(font_size);

    int spacing = bspace_w/(3*3);
    int b1_x, b2_x, b_y;
    b1_x = spacing;
    b2_x = LedControlButton::button_w + spacing*2;
    b_y = bspace_h/4 + lbox_h;
    box[1] = new Fl_Box(0, lbox_h, bspace_w, bspace_h);
    button[0] = new ButtonGreen(b1_x, b_y);
    button[1] = new ButtonRed(b2_x, b_y);

    box[0]->box(FL_UP_BOX);
    box[1]->box(FL_UP_BOX);
    end();
}




int main()
{
    MainWindow *win = new MainWindow;
    win->Show();
    return Fl::run();
}
