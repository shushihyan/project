#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/ucontext.h>
using namespace std;

void signal_handler(int sig, siginfo_t *info, void *ucontext) {
    if (sig == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;

        struct passwd *pw = getpwuid(sender_uid);
        const char *user_name = pw ? pw->pw_name : "unknown";

        ucontext_t *ctx = (ucontext_t *)ucontext;

        cout << "Got SIGUSR1 from PID " << sender_pid
                  << ", UID " << sender_uid << " (" << user_name << ").\n";

#if defined(x86_64)
        cout << "Registers: RIP = " << std::hex << ctx->uc_mcontext.gregs[REG_RIP]
                  << ", RAX = " << ctx->uc_mcontext.gregs[REG_RAX]
                  << ", RBX = " << ctx->uc_mcontext.gregs[REG_RBX] << ".\n";
#elif defined(i386)
        cout << "Registers: EIP = " << std::hex << ctx->uc_mcontext.gregs[REG_EIP]
                  << ", EAX = " << ctx->uc_mcontext.gregs[REG_EAX]
                  << ", EBX = " << ctx->uc_mcontext.gregs[REG_EBX] << ".\n";
#else
        cout << "Register information is not available for this architecture.\n";
#endif
    }
}

int main() {
    cout << "Process running with PID: " << getpid() << "\n";

    struct sigaction sa = {};
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sa, nullptr) < 0) {
        perror("sigaction error");
        return 1;
    }

    while (true) {
        cout << "Waiting for signal...\n";
        sleep(10);
    }

    return 0;
}