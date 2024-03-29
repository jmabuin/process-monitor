#include <getopt.h>
#include <Args.h>
#include <Globals.h>
#include "Runner.h"
#include <cstdio>

static int usage() {
    fprintf(stderr, "\n");
    fprintf(stderr, "Program: ProcessMonitor (Monitor for a given process)\n");
    fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
    fprintf(stderr, "Contact: José M. Abuín <>\n\n");
    fprintf(stderr, "Usage:   ProcessMonitor [options]\n\n");
    fprintf(stderr, "Common options:\n");
    fprintf(stderr, "\n");

    fprintf(stderr, " -d \tDebug mode. Default: False.\n");
    fprintf(stderr, " -h \tPrint this help.\n");
    fprintf(stderr, " -p \tPID of process we are looking for.\n");
    fprintf(stderr, " -c \tJSON configuration file to be used.\n");
    fprintf(stderr, " -o \tOutput folder to store results.\n");

    fprintf(stderr, "\n");

    return 1;
}

int main(int argc, char *argv[]) {

    int option;
    Args input_args;

    while ((option = getopt(argc, argv, "hdc:p:o:")) >= 0) {
        switch (option) {
            case 'h':
                usage();
                return EXIT_SUCCESS;
            case 'c' :
                input_args.ConfigFile = optarg;
                break;
            case 'd':
                input_args.DebugMode = true;
                break;
            case 'p':
                input_args.Pid = std::stoi(optarg);
                break;
            case 'o':
                input_args.OutputFolder = optarg;
                break;
            default:
                break;
        }
    }

    auto runner = Runner(input_args);
    int result = runner.Run();

    return result;
}
