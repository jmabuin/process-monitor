//
// Created by chema on 2/16/24.
//

#ifdef __cplusplus
extern "C"
{
#endif
#include "third-party/papi/papi.h"
#ifdef __cplusplus
}
#endif

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include "PapiInfo.h"
#include "PapiMapEvents.h"


PapiInfo::PapiInfo(int pid, Config *config, std::string *output_folder, bool debug_mode) {
    this->Pid = pid;
    this->configuration = config;
    this->output_folder = output_folder;
    this->debug_mode = debug_mode;
}

void PapiInfo::run_thread() {
    this->running_thread = std::thread(&PapiInfo::run, this);
}

void PapiInfo::run() {
    int EventSet = PAPI_NULL;
    int ret_val = 0;
    std::vector<int> events_vector;
    PAPI_option_t opt;

    std::cout << "[" << this->class_name << "] Started PAPI info. Looking for PID: " << this->Pid << std::endl;

    // No Process, we leave
    if(kill(this->Pid,0) != 0) {
        std::cout << "[" << this->class_name << "] No process for PID: " << this->Pid << ". Leaving" << std::endl;
        return;
    }

    for(const auto& event_str_item : this->configuration->PapiEvents) {
        int val = PapiMapEvents::PapiMapEvents.at(event_str_item);
        events_vector.push_back(val);

        std::string key;
        key.assign(event_str_item);

        std::vector<PapiMeasure> new_vector;

        this->results.insert(std::make_pair<std::string, std::vector<PapiMeasure>>(std::move(key), std::move(new_vector)));
    }

    int events[events_vector.size()];
    std::copy(events_vector.begin(), events_vector.end(), events);

    int num_events = (int)events_vector.size();
    long long int papi_counters[num_events];

    if ((ret_val = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error initializing PAPI: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Currently, from PAPI, only PAPI_GRN_SYS and PAPI_GRN_THR are supported.
    /*if ((ret_val = PAPI_set_cmp_granularity(PAPI_GRN_THR,0)) != PAPI_OK){
        fprintf(stderr,"[%s] Error assigning events granularity %s\n",__func__, PAPI_strerror(ret_val));
        exit(-1);
    }*/

    //eventSet creation
    if ((ret_val = PAPI_create_eventset(&EventSet)) != PAPI_OK){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error creating event set: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    //eventSet assign
    if ((ret_val = PAPI_assign_eventset_component(EventSet, 0)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_assign_eventset_component: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    memset( &opt, 0x0, sizeof ( PAPI_option_t ) );
    opt.inherit.inherit = PAPI_INHERIT_ALL;
    opt.inherit.eventset = EventSet;

    if ( ( ret_val = PAPI_set_opt( PAPI_INHERIT, &opt ) ) != PAPI_OK ) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_set_opt: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Add events to event set
    if ((ret_val=PAPI_add_events(EventSet, events, num_events)) != PAPI_OK){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error adding events: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Attach to PID
    if ((ret_val = PAPI_attach(EventSet, this->Pid)) != PAPI_OK){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_attach: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Reset counters on start
    if ((ret_val = PAPI_reset(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_reset: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start
    if ((ret_val = PAPI_start(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_start: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned int total_measures = 0;

    while(kill(this->Pid,0) == 0){
        if ((ret_val = PAPI_read(EventSet, papi_counters)) != PAPI_OK) {
            std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_read: " << PAPI_strerror(ret_val) << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!this->configuration->AccumulatePapi) {
            if ((ret_val = PAPI_reset(EventSet)) != PAPI_OK) {
                std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_reset: " << PAPI_strerror(ret_val) << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // Save results
        for (int i = 0; i<num_events ; i++) {
            auto new_measure = PapiMeasure {total_measures * this->configuration->MeasureInterval, papi_counters[i] };
            this->results.at(this->configuration->PapiEvents[i]).push_back(new_measure);
            if (this->debug_mode) {
                std::cout << "[" << this->class_name << "][" << __func__ << "] Adding to counter " << this->configuration->PapiEvents[i] << " Value " << new_measure.time_seconds << " s. => " << new_measure.quantity << std::endl;
            }
        }

        total_measures++;

        sleep(this->configuration->MeasureInterval);
    }

    //Finishing
    if ((ret_val = PAPI_stop(EventSet,papi_counters)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error stopping PAPI: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }
    //Detach from PID
    if ((ret_val = PAPI_detach(EventSet)) != PAPI_OK){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_detach: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((ret_val = PAPI_cleanup_eventset(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error cleaning EventSet: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((ret_val = PAPI_destroy_eventset(&EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error destroying EventSet: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    this->write_results_to_file();
}

void PapiInfo::write_results_to_file() {

    if (this->debug_mode) {
        std::cout << "[" << PapiInfo::class_name << "] Writing PAPI Results at: " << *this->output_folder << std::endl;
    }

    for(const auto& event_name : this->configuration->PapiEvents) {
        std::string file_name = std::to_string(this->Pid) + "_" +event_name + "_cpu.csv";

        if (this->debug_mode) {
            std::cout << "[" << PapiInfo::class_name << "] " << event_name << " file: " << *this->output_folder + "/" + file_name << std::endl;
        }

        // Write CPU measures
        std::ofstream event_file;
        event_file.open(*this->output_folder + "/" + file_name);

        for(const auto& current_event_measure : this->results.at(event_name)) {
            event_file << current_event_measure.time_seconds << ";" << current_event_measure.quantity << std::endl;
        }

        event_file.close();
    }
}

void PapiInfo::print_device_info() {

    const PAPI_hw_info_t *hwinfo = nullptr;
    int ret_val = 0;

    if ((ret_val = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT) {
        std::cerr << "[PapiInfo] [" << __func__ << "] Error initializing PAPI: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    if ((hwinfo = PAPI_get_hardware_info()) == nullptr) {
        std::cerr << "[PapiInfo] [" << __func__ << "] Error in PAPI_get_hardware_info." << std::endl;
        exit(EXIT_FAILURE);
    }

    printf("==========================DEVICE INFO==========================\n");
    printf("Vendor                                : %s (%u,0x%x)\n",
           hwinfo->vendor_string,
           hwinfo->vendor,
           hwinfo->vendor);
    printf("Name                                  : %s %s\n", hwinfo->model_string, hwinfo->vendor_string);
    printf("CPUID                                 : Family/Model/Stepping %u/%u/%u 0x%02x/0x%02x/0x%02x\n",
           hwinfo->cpuid_family, hwinfo->model, hwinfo->cpuid_stepping, hwinfo->cpuid_family, hwinfo->model,
           hwinfo->cpuid_stepping);
    printf("Sockets                               : %u\n", hwinfo->sockets);
    printf("Total threads                         : %u\n", hwinfo->totalcpus);
    printf("Cores per socket                      : %u\n", hwinfo->cores);
    printf("SMT threads per core                  : %u\n", hwinfo->threads);
    printf("===============================================================\n");
}
