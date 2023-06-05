#include "atpg.h"

void ATPG::tdfatpg() {
    string vec, vec1, vec2;
    int max_tried = max(1, (int) log2((double) detected_num));
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
    int fault_idx = 0;
    while (fault_under_test != nullptr) {
        // Perform PODEM multiple times for a fault, each time with different backtrace prob
        current_fault = fault_under_test;
        for(int i = 0; i < 4; ++i) {
            // clear the vector of unknown bit information
            cktu.clear();
            backtrace_prob = (float) i * 0.25;
            switch (podemtdf(fault_under_test, current_backtracks, backtrace_prob)) {
                case TRUE:
                    /* form a vector */
                    vec.clear();
                    // Pattern for v2
                    for (int i = 0; i < cktin.size(); ++i) {
                        vec.push_back(itoc(cktin[i]->value));
                    }
                    /* Add one bit for v1 */
                    vec1.clear();
                    vec2.clear();
                    vec1 = vec.substr(1, vec.size()-1) + '0' + vec[0];
                    vec2 = vec.substr(1, vec.size()-1) + '1' + vec[0];
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
                        unknown_ratio = (float) unknown_bit / vec1.size();
                        if(unknown_ratio < 0.625 || unknown_bit < 5) {
                            for(int j = 0; j < 2; ++j) {
                                for(int i = 0; i < cktu.size() + 1; ++i) {
                                    if(cktu[i]) {
                                        if(i == 0) {
                                            vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                                            vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                                        }
                                        else {
                                            vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                                            vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                                        }
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
                                        if((float) rand() / RAND_MAX > 0.5 * j) {
                                            if(i == 0) 
                                                vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                                            else
                                                vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                                        }
                                        else {
                                            if(i == 0)
                                                vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                                            else
                                                vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            for(int i = 0; i < cktu.size() + 1; ++i) {
                                if(cktu[i]) {
                                    if(i == 0) {
                                        vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                                        vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                                    }
                                    else {
                                        vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                                        vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                                    }
                                }
                            }
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
                            
                            for(int i = 0; i < cktu.size() + 1; ++i) {
                                if(cktu[i]) {
                                    if(rand() & 01) {
                                        if(i == 0) {
                                            vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                                            vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                                        }
                                        else {
                                            vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                                            vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                                        }
                                    }
                                }
                            }
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

                            for(int i = 0; i < cktu.size() + 1; ++i) {
                                if(cktu[i]) {
                                    if(i == 0) {
                                        vec1[cktu.size()] = '0';
                                        vec2[cktu.size()] = '0';
                                    }
                                    else {
                                        vec1[i-1] = '0';
                                        vec2[i-1] = '0';
                                    }
                                }    
                            }
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

                            for(int i = 0; i < cktu.size() + 1; ++i) {
                                if(cktu[i]) {
                                    if(i == 0) {
                                        vec1[cktu.size()] = '1';
                                        vec2[cktu.size()] = '1';
                                    }
                                    else {
                                        vec1[i-1] = '1';
                                        vec2[i-1] = '1';
                                    }
                                }    
                            }
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
        if(fault_under_test->detected_time >= detected_num) {
            fault_under_test->test_tried = true;
        }
        else {
            if(++fault_under_test->tried_time > max_tried) {
                fault_under_test->test_tried = true;
            }
        }
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
    //set_fault_detect_status();
    display_undetect();
}

int ATPG::tdfsim_v1v2(const string& vec1, const string& vec2, int& current_detect_num) {
    int vec1_det_num, vec2_det_num;
    // flag to identify whether the pattern hit the targeted fault
    bool vec1_hit = false, vec2_hit = false; 
    int vec1_dec = 0, vec2_dec = 0, vec_b = 0;
    // Reset fault be_detect flag
    for(auto f : flist_undetect) {
        f->be_det = 0;
    }

    current_detect_num = 0;
    tdfault_sim_a_vector(vec1,  current_detect_num, true, 1, vec1_hit);
    vec1_det_num = current_detect_num;
    reset_fault_detect_status();
    tdfault_sim_a_vector(vec2,  current_detect_num, true, 2, vec2_hit);
    vec2_det_num = current_detect_num;
    current_detect_num = 0;
    if(vec1_hit && !vec2_hit) {
        tdfault_fault_drop(1);
        current_detect_num += vec1_det_num;
        return 1;
    }
    else if(!vec1_hit && vec2_hit) {
        tdfault_fault_drop(2);
        current_detect_num += vec2_det_num;
        return 0;
    }
    else {
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
}

void ATPG::reset_fault_detect_status() {
    for(auto f : flist_undetect) {
        if(f->detect != REDUNDANT)
            f->detect = FALSE;
    }
}
void ATPG::set_fault_detect_status() {
    for(auto f : flist_undetect) {
        if(f->detect != REDUNDANT)
            f->detect = FALSE;
    }
}