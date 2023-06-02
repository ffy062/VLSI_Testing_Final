#include "atpg.h"

void ATPG::tdfatpg() {
    string vec;
    int current_detect_num = 0;
    int total_detect_num = 0;
    int total_no_of_backtracks = 0;  // accumulative number of backtracks
    int current_backtracks = 0;
    int no_of_aborted_faults = 0;
    int no_of_redundant_faults = 0;
    int no_of_calls = 0;

    fptr fault_under_test = flist_undetect.front();
    while (fault_under_test != nullptr) {
        switch (podemtdf(fault_under_test, current_backtracks)) {
            case TRUE:
                /* form a vector */
                vec.clear();
                for (wptr w: cktin) {
                    vec.push_back(itoc(w->value));
                }
                /* run a fault simulation, drop ALL detected faults */   
                vec.push_back('0');
                tdfault_sim_a_vector(vec, current_detect_num);
                if(current_detect_num == 0) {
                    vec.pop_back();
                    vec.push_back('1');
                    tdfault_sim_a_vector(vec, current_detect_num);
                    if(current_detect_num == 0)
                        break;
                }
                display_io_tdf(vec);
                total_detect_num += current_detect_num;
                in_vector_no++;
                break;
            case FALSE:fault_under_test->detect = REDUNDANT;
                no_of_redundant_faults++;
                break;
            case MAYBE:no_of_aborted_faults++;
                break;
        }
        fault_under_test->test_tried = true;
        fault_under_test = nullptr;
        for (fptr fptr_ele: flist_undetect) {
            if (!fptr_ele->test_tried) {
                fault_under_test = fptr_ele;
                break;
            }
        }
        total_no_of_backtracks += current_backtracks; // accumulate number of backtracks
        no_of_calls++;
    }
    display_undetect();
    // fprintf(stdout, "\n");
    // fprintf(stdout, "#number of aborted faults = %d\n", no_of_aborted_faults);
    // fprintf(stdout, "\n");
    // fprintf(stdout, "#number of redundant faults = %d\n", no_of_redundant_faults);
    // fprintf(stdout, "\n");
    // fprintf(stdout, "#number of calling podem1 = %d\n", no_of_calls);
    // fprintf(stdout, "\n");
    // fprintf(stdout, "#total number of backtracks = %d\n", total_no_of_backtracks);
}