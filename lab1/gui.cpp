#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

#include <unistd.h>
#include <signal.h>



class Button : Fl_Button {
public:
    Button(int x, int y, int w, int h, const char *lb = 0)
        : Fl_Button(x, y, w, h, lb)
        { callback(CallbackFunction, 0); }
    virtual ~Button() {}
    virtual void OnPress() {}
private:
    static void CallbackFunction(Fl_Widget *w, void *user)
        { static_cast<Button*>(w)->OnPress(); }
};



class CommonButton : public Button {
public:
    enum button_sizes {
        button_h = 30,
        button_w = 100
    };
    CommonButton(int x, int y, const char *lb)
        : Button(x, y, button_w, button_h, lb) {}
};



class ButtonGreen : public CommonButton {
public:
    ButtonGreen(int x, int y) : CommonButton(x, y, "Green") {}
    virtual void OnPress() {
        kill(getppid(), SIGUSR1);
    }
};

class ButtonRed : public CommonButton {
public:
    ButtonRed(int x, int y) : CommonButton(x, y, "Red") {}
    virtual void OnPress() {
        kill(getppid(), SIGUSR2);
    }
};





class MainWindow : Fl_Window {
    Button *b[2];
    enum button_space_sizes {
        bspace_h = CommonButton::button_h*2,
        bspace_w = CommonButton::button_w*3
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
    MainWindow()
        : Fl_Window(win_w, win_h, "User interface")
    {
        Fl_Box *box = new Fl_Box(0, 0, lbox_w, lbox_h,
                            "What the led do you want\nto turn on?");
        box->labelsize(18);
        int b_y = bspace_h/4 + lbox_h;
        int spacing = bspace_w/(3*3);
        b[0] = new ButtonGreen(spacing, b_y);
        b[1] = new ButtonRed(CommonButton::button_w + spacing*2, b_y);
        end();
    }
    void Show() { show(); }
};





int main()
{
    MainWindow *w = new MainWindow;
    w->Show();
    return Fl::run();
}
