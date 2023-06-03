#include "atpg.h"

void ATPG::tdfatpg() {
    string vec, vec1, vec2;
    int vec1_det_num, vec2_det_num, unknown_bit;
    int current_detect_num = 0;
    int total_detect_num = 0;
    int total_no_of_backtracks = 0;  // accumulative number of backtracks
    int current_backtracks = 0;
    int no_of_aborted_faults = 0;
    int no_of_redundant_faults = 0;
    int no_of_calls = 0;

    fptr fault_under_test = flist_undetect.front();
    while (fault_under_test != nullptr) {
        cktu.clear();
        switch (podemtdf(fault_under_test, current_backtracks)) {
            case TRUE:
                /* form a vector */
                vec.clear();
                // Pattern for v2
                for (int i = 1; i < cktin.size(); ++i) {
                    vec.push_back(itoc(cktin[i]->value));
                }
                vec.push_back(itoc(cktin[0]->value));
                /* Add one bit for v1 */
                vec1.clear();
                vec2.clear();
                vec1 = '0' + vec.substr(0, vec.size());
                vec2 = '1' + vec.substr(1, vec.size());
                /* run a fault simulation, drop ALL detected faults */
                tdfault_sim_a_vector(vec1, current_detect_num);
                vec1_det_num = current_detect_num;
                tdfault_sim_a_vector(vec2, current_detect_num);
                vec2_det_num = current_detect_num;
                if(vec1_det_num > vec2_det_num) {
                    display_io_tdf(vec1);
                    total_detect_num +=vec1_det_num;
                }
                else {
                    if(vec2_det_num == 0) {
                        for(int i = 0; i < cktu.size(); ++i) {
                            // If this bit is unknown
                            if(cktu[i]) {
                                vec1[i+1] = (vec1[i+1] == '0')? '1' : '0';
                                vec2[i+1] = (vec2[i+1] == '0')? '1' : '0';
                                tdfault_sim_a_vector(vec1, current_detect_num);
                                vec1_det_num = current_detect_num;
                                tdfault_sim_a_vector(vec2, current_detect_num);
                                vec2_det_num = current_detect_num;
                                if(vec1_det_num > 0) {
                                    display_io_tdf(vec1);
                                    total_detect_num +=vec1_det_num;
                                    break;
                                }
                                if(vec2_det_num > 0) {
                                    display_io_tdf(vec2);
                                    total_detect_num +=vec2_det_num;
                                    break;
                                }
                                vec1[i+1] = (vec1[i+1] == '0')? '1' : '0';
                                vec2[i+1] = (vec2[i+1] == '0')? '1' : '0';
                            }
                        }
                        break;
                    }
                    display_io_tdf(vec2);
                    total_detect_num += vec2_det_num;
                }
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