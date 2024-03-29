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

#include <csignal>
#include "EnergyInfo.h"
#include "StringUtils.h"

#include <cstdlib>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <fstream>


EnergyInfo::EnergyInfo(int pid, Config *config, std::string *output_folder, bool debug_mode) {
    this->Pid = pid;
    this->configuration = config;
    this->output_folder = output_folder;
    this->debug_mode = debug_mode;
}

void EnergyInfo::run_thread() {
    this->running_thread = std::thread(&EnergyInfo::run, this);
}

void EnergyInfo::run() {
    int EventSet = PAPI_NULL;
    int ret_val,cid,rapl_cid=-1,numcmp;
    int code;
    int r, i;
    int num_events=0;
    const PAPI_component_info_t *cmpinfo;
    char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
    char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
    std::vector<std::string> event_names_vector;
    int data_type[MAX_RAPL_EVENTS];
    long long *values;
    PAPI_event_info_t evinfo;

    long long start_time,after_time;
    double total_time = 0.0, local_time;

    std::cout << "Started Energy info. Looking for PID: " << this->Pid << std::endl;

    // No Process, we leave
    if(kill(this->Pid,0) != 0) {
        std::cout << "[" << this->class_name << "] No process for PID: " << this->Pid << ". Leaving" << std::endl;
        return;
    }

    if ((ret_val = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error initializing PAPI: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }
    numcmp = PAPI_num_components();

    for(cid=0; cid<numcmp; cid++) {

        if ( (cmpinfo = PAPI_get_component_info(cid)) == nullptr) {
            std::cerr << "[" << this->class_name << "][" << __func__ << "] PAPI_get_component_info failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (std::string(cmpinfo->name) == "rapl") {

            rapl_cid=cid;

            if (this->debug_mode) {
                std::cout << "[" << this->class_name << "] Found RAPL component at cid: " << rapl_cid << std::endl;
            }

            if (cmpinfo->disabled) {
                std::cout << "[" << this->class_name << "] RAPL component disabled: " << cmpinfo->disabled_reason << std::endl;
                return;
            }

            break;
        }
    }

    /* Component not found */
    if (cid==numcmp) {
        std::cout << "[" << this->class_name << "] RAPL component not found" << std::endl;
        return;
    }

    //eventSet creation
    if ((ret_val = PAPI_create_eventset(&EventSet)) != PAPI_OK){
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error creating event set: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Add all events */

    code = PAPI_NATIVE_MASK;

    r = PAPI_enum_cmp_event( &code, PAPI_ENUM_FIRST, rapl_cid );

    /*
     * AVAILABLE COUNTERS:
     * NAME: rapl:::THERMAL_SPEC_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::MINIMUM_POWER_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::MAXIMUM_POWER_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::MAXIMUM_TIME_WINDOW_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::PACKAGE_ENERGY_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::DRAM_ENERGY_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::PP0_ENERGY_CNT:PACKAGE0 UNITS:  DATATYPE: 1
     * NAME: rapl:::THERMAL_SPEC:PACKAGE0 UNITS: W DATATYPE: 2
     * NAME: rapl:::MINIMUM_POWER:PACKAGE0 UNITS: W DATATYPE: 2
     * NAME: rapl:::MAXIMUM_POWER:PACKAGE0 UNITS: W DATATYPE: 2
     * NAME: rapl:::MAXIMUM_TIME_WINDOW:PACKAGE0 UNITS: s DATATYPE: 2
     * NAME: rapl:::PACKAGE_ENERGY:PACKAGE0 UNITS: nJ DATATYPE: 1
     * NAME: rapl:::DRAM_ENERGY:PACKAGE0 UNITS: nJ DATATYPE: 1
     * NAME: rapl:::PP0_ENERGY:PACKAGE0 UNITS: nJ DATATYPE: 1
     */
    while ( r == PAPI_OK ) {

        ret_val = PAPI_event_code_to_name( code, event_names[num_events] );
        if ( ret_val != PAPI_OK ) {
            std::cerr << "[" << this->class_name << "][" << __func__ << "] Error translating code: " << PAPI_strerror(ret_val) << std::endl;
            exit(EXIT_FAILURE);
        }

        ret_val = PAPI_get_event_info(code,&evinfo);
        if (ret_val != PAPI_OK) {
            std::cerr << "[" << this->class_name << "][" << __func__ << "] Error getting event info: " << PAPI_strerror(ret_val) << std::endl;
            exit(EXIT_FAILURE);
        }

        strncpy(units[num_events],evinfo.units,sizeof(units[0]));

        // buffer must be null terminated to safely use strstr operation on it below
        units[num_events][sizeof(units[0])-1] = '\0';

        data_type[num_events] = evinfo.data_type;
        //std::cout << "NAME: " << event_names[num_events] << " UNITS: " <<  units[num_events] << " DATATYPE: " << data_type[num_events] << std::endl;

        ret_val = PAPI_add_event( EventSet, code );
        if (ret_val != PAPI_OK) {
            break; /* We've hit an event limit */
        }
        num_events++;

        r = PAPI_enum_cmp_event( &code, PAPI_ENUM_EVENTS, rapl_cid );
    }

    // Initialize results map
    for (i = 0; i< num_events; i++) {
        if (this->results.find(event_names[i]) == this->results.end()) {
            std::string key = event_names[i];
            std::string short_key = StringUtils::ReplaceAll(key, ":", "_");
            std::vector<EnergyMeasure> new_vector;
            event_names_vector.push_back(short_key);
            this->results.insert(std::make_pair<std::string, std::vector<EnergyMeasure>>(std::move(short_key), std::move(new_vector)));
        }
    }

    values= static_cast<long long int *>(calloc(num_events, sizeof(long long)));
    if (values==nullptr) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error getting memory for values: " << std::endl;
        exit(EXIT_FAILURE);
    }

    //Counter reset before begin
    if ((ret_val = PAPI_reset(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_reset: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    if ((ret_val = PAPI_start(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_start: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    while(kill(this->Pid,0) == 0){

        start_time=PAPI_get_real_nsec();
        sleep(this->configuration->MeasureInterval);

        //Read measures and store them
        if ((ret_val = PAPI_read(EventSet, values)) != PAPI_OK) {
            std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_read: " << PAPI_strerror(ret_val) << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!this->configuration->AccumulateEnergy) {
            if ((ret_val = PAPI_reset(EventSet)) != PAPI_OK) {
                std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_reset: " << PAPI_strerror(ret_val) << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        after_time=PAPI_get_real_nsec();
        local_time=((double)(after_time-start_time))/1.0e9;
        total_time = total_time + local_time;

        for (i = 0; i<num_events ; i++) {

            if (strstr(units[i],"nJ")) {

                auto measured_energy = (double)values[i]/1.0e9;	//Jules
                auto new_measure = EnergyMeasure {total_time, measured_energy };
                this->results.at(event_names_vector[i]).push_back(new_measure);

                if (this->debug_mode) {
                    std::cout << "[" << this->class_name << "] " << event_names_vector[i] << ": " << new_measure.time_seconds << ", " << new_measure.quantity << " Jules" << std::endl;
                }
            }

            /*if (strstr(event_names[i],"ENERGY_CNT")) {
                printf("%-40s%12lld\t%#08llx %s\n", event_names[i], values[i], values[i], units[i]);
            }*/
        }
        sleep(this->configuration->MeasureInterval);
    }

    //Finishing
    if ((ret_val = PAPI_stop(EventSet,values)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_stop: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Cleanup
    if ((ret_val = PAPI_cleanup_eventset(EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_cleanup_eventset: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((ret_val = PAPI_destroy_eventset(&EventSet)) != PAPI_OK) {
        std::cerr << "[" << this->class_name << "][" << __func__ << "] Error in PAPI_destroy_eventset: " << PAPI_strerror(ret_val) << std::endl;
        exit(EXIT_FAILURE);
    }

    this->write_results_to_file(event_names_vector);
}

void EnergyInfo::write_results_to_file(const std::vector<std::string>& event_names_vector) {

    if (this->debug_mode) {
        std::cout << "[" << EnergyInfo::class_name << "] Writing Energy Results at: " << *this->output_folder << std::endl;
    }

    for(const auto& event_name : event_names_vector) {
        std::string file_name = std::to_string(this->Pid) + "_" + event_name + ".csv";

        if (this->debug_mode) {
            std::cout << "[" << EnergyInfo::class_name << "] " << event_name << " file: " << *this->output_folder + "/" + file_name << std::endl;
        }

        // Write this counter measures
        std::ofstream event_file;
        event_file.open(*this->output_folder + "/" + file_name);

        for(const auto& current_event_measure : this->results.at(event_name)) {
            event_file << current_event_measure.time_seconds << ";" << current_event_measure.quantity << std::endl;
        }

        event_file.close();
    }
}
