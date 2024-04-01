# process-monitor

**process-monitor** is a tool developed to track performance of a given process PID. Available properties to measure right now are:

  * Memory and CPU %
  * IO read/write operations
  * PAPI counters
  * RAPL Energy counters

## Building
The tool is coded in C++17 and uses CMake to be built. Dependencies can be installed in Debian-like distributions with the provided script [InstallDependencies.sh](InstallDependencies.sh)

## How to run?
It is possible to run in two modes.

1. By providing a PID for the process to track and a configuration json file (See file [configs/example.json](configs/example.json))
2. By indicating a command to run in the configuration file. Measures will be taken then from this command.

From each measure, csv files will be created in the indicated output folder or, in the case this directory is not provided, in an output directory with the PID of the process being monitored.

## Third party libraries used
**process-monitor** relies on other libraries in order to work. These third party libraries are:

  * [https://github.com/nlohmann/json](https://github.com/nlohmann/json) to read JSON configuration file.
  * [PAPI](https://github.com/icl-utk-edu/papi/) to get info from software counters and RAPL.
  * [PFS](https://github.com/dtrugman/pfs) to get processes info from Linux /proc folder.