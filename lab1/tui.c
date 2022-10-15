#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <cdk/cdk.h>
#include <cdk/dialog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>





static volatile sig_atomic_t was_ctrl_c = 0;
static void sighdl(int signo)
{
    switch(signo) {
    case SIGQUIT:
        was_ctrl_c = 1;
        break;
    default:
        {}
    }
}


struct app_ctx {
    CDKSCREEN *screen;
    CDKDIALOG *dialog;
    CDK_PARAMS params;
};


#define ARRAY_SIZE(arr) ((sizeof(arr))/(sizeof(arr[0])))

static CDKDIALOG *create_dialog(CDKSCREEN *screen, CDK_PARAMS *params)
{
    CDKDIALOG *dialog;
    const char *message[] = {
        "",
        "<C>What led do you want to turn on?",
        ""
    };
    const char *buttons[] = { "GREEN", "RED" };

    dialog = newCDKDialog(
        screen,
        CDKparamValue(params, 'X', CENTER),
        CDKparamValue(params, 'Y', CENTER),
        (CDK_CSTRING2)message, ARRAY_SIZE(message),
        (CDK_CSTRING2)buttons, ARRAY_SIZE(buttons),
        A_REVERSE, TRUE,
        CDKparamValue(params, 'N', TRUE),
        CDKparamValue(params, 'S', FALSE)
    );
    if(dialog)
        setCDKDialogBackgroundColor(dialog, "</57>");
    return dialog;
}



static int app_start(int argc, char **argv, struct app_ctx *ctx)
{
    struct sigaction sa;

    CDKparseParams(argc, argv, &ctx->params, CDK_MIN_PARAMS);
    ctx->screen = initCDKScreen(NULL);
    initCDKColor();

    ctx->dialog = create_dialog(ctx->screen, &ctx->params);
    if(!ctx->dialog) {
        destroyCDKScreen(ctx->screen);
        endCDK();
        fprintf(stderr, "Can't draw the user interface, why?\n");
        return 0;
    }

    curs_set(0);

    sa.sa_handler = &sighdl;
    sa.sa_flags = 0;
    sigfillset(&sa.sa_mask);
    sigaction(SIGQUIT, &sa, NULL);

    return 1;
}

static void app_run(struct app_ctx *ctx)
{
    while(!was_ctrl_c) {
        int ch = activateCDKDialog(ctx->dialog, NULL);
        switch(ch) {
        case 0:
            kill(getppid(), SIGUSR1);
            break;
        case 1:
            kill(getppid(), SIGUSR2);
            break;
        default:
            {}
        }
    }
}


static void app_cleanup(struct app_ctx *ctx)
{
    destroyCDKDialog(ctx->dialog);
    destroyCDKScreen(ctx->screen);
    endCDK();
}


int main(int argc, char **argv)
{
    struct app_ctx ctx;
    int ok;

    ok = app_start(argc, argv, &ctx);
    if(!ok)
        return 1;
    app_run(&ctx);
    app_cleanup(&ctx);
    return 0;
}
