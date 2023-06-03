#include "atpg.h"

void ATPG::tdfatpg() {
    string vec, vec1, vec2;
    float unknown_ratio, backtrace_prob;
    int vec1_det_num, vec2_det_num, unknown_bit, result;
    int current_detect_num = 0;
    int total_detect_num = 0;
    int total_no_of_backtracks = 0;  // accumulative number of backtracks
    int current_backtracks = 0;
    int no_of_aborted_faults = 0;
    int no_of_redundant_faults = 0;
    int no_of_calls = 0;

    srand(8);
    fptr fault_under_test = flist_undetect.front();
    while (fault_under_test != nullptr) {
        // Perform multiple PODEM for a fault, each time with different backtrace prob
        for(int i = 0; i < 4; ++i) {
            // clear the vector of unknown bit information
            cktu.clear();
            backtrace_prob = (float) i * 0.25;
            switch (podemtdf(fault_under_test, current_backtracks, backtrace_prob)) {
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
                    vec2 = '1' + vec.substr(0, vec.size());
                    /* run a fault simulation, drop ALL detected faults */
                    result = tdfsim_v1v2(vec1, vec2, current_detect_num);
                    // 1 means vec1, 0 means vec2, -1 means both detect num = 0 
                    if(result == 1) {
                        display_io_tdf(vec1);
                        in_vector_no++;
                        //total_detect_num +=vec1_det_num;
                    }
                    else if(result == 0) {
                        display_io_tdf(vec2);
                        in_vector_no++;
                        //total_detect_num +=vec2_det_num;
                    }
                    else {
                        // Try to flip every unsigned bit one by one
                        unknown_bit = 0;
                        unknown_ratio = 0.0;
                        for(int i = 0; i < cktu.size(); ++i) {
                            if(cktu[i]) {
                                unknown_bit++;
                            }
                        }
                        unknown_ratio = (float) unknown_bit / cktu.size();
                        if(unknown_ratio < 0.6 || unknown_bit < 5) {
                            for(int i = 0; i < cktu.size(); ++i) {
                                if(cktu[i]) {
                                    vec1[i+1] = (vec1[i+1] == '0')? '1' : '0';
                                    vec2[i+1] = (vec2[i+1] == '0')? '1' : '0';
                                    result = tdfsim_v1v2(vec1, vec2, current_detect_num);
                                    if(result == 1) {
                                        display_io_tdf(vec1);
                                        //total_detect_num +=vec1_det_num;
                                        in_vector_no++;
                                        break;
                                    }
                                    else if(result == 0) {
                                        display_io_tdf(vec2);
                                        //total_detect_num +=vec2_det_num;
                                        in_vector_no++;
                                        break;
                                    }
                                    vec1[i+1] = (vec1[i+1] == '0')? '1' : '0';
                                    vec2[i+1] = (vec2[i+1] == '0')? '1' : '0';
                                }
                            }
                        }
                    }
                    break;
                case FALSE:
                    fault_under_test->detect = REDUNDANT;
                    no_of_redundant_faults++;
                    break;
                case MAYBE:
                    no_of_aborted_faults++;
                    break;
            }
            // If we have found a solution
            if(current_detect_num)
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

int ATPG::tdfsim_v1v2(const string& vec1, const string& vec2, int& current_detect_num) {
    int vec1_det_num, vec2_det_num;
    // Reset fault be_detect flag
    for(auto f : flist_undetect) {
        f->be_det = 0;
    }

    tdfault_sim_a_vector(vec1,  current_detect_num, true, 1);
    vec1_det_num = current_detect_num;
    reset_fault_detect_status();
    tdfault_sim_a_vector(vec2,  current_detect_num, true, 2);
    vec2_det_num = current_detect_num;
    if(vec1_det_num > vec2_det_num) {
        tdfault_fault_drop(1);
        current_detect_num += vec1_det_num;
        //fprintf(stdout, "vector detects %d faults\n", vec1_det_num);
        return 1;
    }
    else {
        if(vec2_det_num == 0) {
            return -1;
        }
        else if(vec1_det_num == vec2_det_num) {
            int res = rand() & 01;
            if(res) {
                tdfault_fault_drop(1);
                current_detect_num += vec1_det_num;
                //fprintf(stdout, "vector detects %d faults\n", vec1_det_num);
            }
            else {
                tdfault_fault_drop(2);
                current_detect_num += vec2_det_num;
                //fprintf(stdout, "vector detects %d faults\n", vec2_det_num);
            } 
            return res;
        }
        else {
            tdfault_fault_drop(2);
            current_detect_num += vec2_det_num;
            //fprintf(stdout, "vector detects %d faults\n", vec2_det_num);
            return 0;
        }
    }
}

void ATPG::reset_fault_detect_status() {
    for(auto f : flist_undetect) {
        f->detect = FALSE;
    }
}