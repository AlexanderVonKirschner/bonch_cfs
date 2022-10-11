#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>





static volatile sig_atomic_t last_sig = 0;
static void sighdl(int signo)
{
    last_sig = signo;
}



enum led_id {
    li_green = 0,
    li_red   = 1
};

struct app_ctx {
    int ui_pid;
    int led_fd_array[2];
    enum led_id active_led;
    struct timespec next_switch_time;
    sigset_t curr_mask, orig_mask;
    int quit_flag;
    int ret_code;
};


static void led_on(int led_fd)
{
    write(led_fd, "1", 1);
}

static void led_off(int led_fd)
{
    write(led_fd, "0", 1);
}


static int start_ui()
{
    int pid;

    pid = fork();
    if(pid == -1) {
        perror("fork");
        return -1;
    } else if(pid == 0) {
        execlp("./ui", "./ui", NULL);
        perror("execlp");
        exit(1);
    } else {
        return pid;
    }
}


static int app_start(struct app_ctx *ctx)
{
    sigemptyset(&ctx->curr_mask);
    sigaddset(&ctx->curr_mask, SIGCHLD);
    sigaddset(&ctx->curr_mask, SIGUSR1);
    sigaddset(&ctx->curr_mask, SIGUSR2);
    sigaddset(&ctx->curr_mask, SIGINT);
    sigprocmask(SIG_BLOCK, &ctx->curr_mask, &ctx->orig_mask);

    ctx->ui_pid = start_ui();
    if(ctx->ui_pid == -1)
        return 0;

    led_fd_array[2];

    led_on(ctx->led_fd_array[li_green]);
    led_off(ctx->led_fd_array[li_red]);
    ctx->active_led = li_green;

    clock_gettime(CLOCK_REALTIME, &ctx->next_switch_time);
    ctx->next_switch_time.tv_nsec += 500*1000*1000;
    if(ctx->next_switch_time.tv_nsec >= 1000*1000*1000) {
        ctx->next_switch_time.tv_sec++;
        ctx->next_switch_time.tv_nsec -= 1000*1000*1000;
    }

    ctx->quit_flag = 0;
    ctx->ret_code = 0;
    return 1;
}


static void app_break(struct app_ctx *ctx, int ret)
{
    ctx->quit_flag = 1;
    ctx->ret_code = ret;
}


static void handle_signal(struct app_ctx *ctx)
{
    int status;

    switch(last_sig) {
    case SIGCHLD:
        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
            app_break(ctx, 0);
        else
            app_break(ctx, 1);
        break;
    case SIGUSR1:
        /* ... */
        break;
    case SIGUSR2:
        /* ... */
        break;
    default:
        {}
    }
    last_sig = 0;
}

static void handle_pselect_error(struct app_ctx *ctx)
{
    kill(ctx->ui_pid, SIGINT);
    perror("pselect");
    app_break(ctx, 1);
}

static void handle_timeout(struct app_ctx *ctx)
{
    ctx->next_switch_time.tv_nsec += 500*1000*1000;
    if(ctx->next_switch_time.tv_nsec >= 1000*1000*1000) {
        ctx->next_switch_time.tv_sec++;
        ctx->next_switch_time.tv_nsec -= 1000*1000*1000;
    }
    switch_leds(ctx);
}

static void app_run(struct app_ctx *ctx)
{
    do {
        int res = pselect(0, NULL, NULL, NULL,
                      &ctx->next_switch_time, &ctx->orig_mask);
        if(res == -1) {
            if(errno == EINTR)
                handle_signal(ctx);
            else
                handle_pselect_error(ctx);
        } else if(res == 0) {
            handle_timeout(ctx);
        }
    } while(!ctx->quit_flag);
}

static void app_cleanup(struct app_ctx *ctx)
{
    close(ctx->led_fd_array[0]);
    close(ctx->led_fd_array[1]);
    /* ... */
}


int main()
{
    struct app_ctx ctx;
    int ok;

    ok = app_start(&ctx);
    if(!ok)
        return 1;
    app_run(&ctx);
    app_cleanup(&ctx);
    return ctx.ret_code == 0 ? 0 : 2;
}
