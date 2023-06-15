#include "atpg.h"

using namespace std;

bool ATPG::my_cmp(fptr& a, fptr& b) {
    return a->detected_time - b->detected_time;
}

void ATPG::tdfatpg() {
    string vec, vec1, vec2;
    string in_val = "01";
    bool chg_seed = false;
    float unknown_ratio, backtrace_prob;
    int vec1_det_num, vec2_det_num, unknown_bit, result, flist_size;
    int current_detect_num = 0;
    int total_detect_num = 0;
    int total_no_of_backtracks = 0;  // accumulative number of backtracks
    int current_backtracks = 0;
    int no_of_aborted_faults = 0;
    int no_of_redundant_faults = 0;
    int no_of_calls = 0;
    // Store the number of faults detected in each stage
    int first_try = 0;
    int random = 0;
    int all_zo = 0;
    int random_2 = 0;
    int flip = 0;
    // Minimum fault detect number of each round(cur_tried)
    int max_tried = max(1, (int) log2((double) detected_num)), cur_tried = 0;
    int min_det_num[5] = {1, 2, 4, 8};
    int cur_min_det = 0;


    srand(8);
    fptr fault_under_test = flist_undetect.front();
    fptr n_fault_under_test;
    int fault_idx = 0;
    int cnt = 0;
    bool v1_s, v2_s;
    vector<fptr> current_fault_detected;
    vector<wptr> n_cktin;
    vector<wptr> n_sort_wlist;
    tdf_vectors.clear();
    while (fault_under_test != nullptr) {
        // change seed
        if(++cnt % 777 == 0 || chg_seed) {
            chg_seed = false;
            srand(8 + cnt % 19);
        }
        // Perform PODEM multiple times for a fault, each time with different backtrace prob
        cur_min_det = min_det_num[max_tried - cur_tried];
        for(int i = 0; i < 4; ++i) {
            // clear the vector of unknown bit information
            cktu.clear();
            backtrace_prob = (float)  cur_tried * 0.075 + (float) i * (0.25);
            switch (podemtdf(fault_under_test, current_backtracks, backtrace_prob)) {
                case TRUE:
                    n_cktu.clear();
                    n_cktin.clear();
                    n_sort_wlist.clear();
                    for(auto u : cktu)
                        n_cktu.push_back(u);
                    for(auto in : cktin)
                        n_cktin.push_back(in);
                    for(auto w : sort_wlist)
                        n_sort_wlist.push_back(w);
                    /* form a vector */
                    vec.clear();
                    // Pattern for v2
                    for (int i = 0; i < cktin.size(); ++i) {
                        vec.push_back(itoc(cktin[i]->value));
                    }
                    unknown_bit = 0;
                    for(int i = 0; i < cktu.size(); ++i)
                        if(cktu[i]) unknown_bit++;
                    if(test_compression && max_tried - cur_tried < 1 && unknown_bit > cktin.size() / 16) {
                        current_detect_num = 0;
                        for (fptr fptr_ele: flist_undetect) {
                            n_fault_under_test = fptr_ele;
                            if(podemtdf2(n_fault_under_test, current_backtracks, 1.0 - backtrace_prob) == 1) {
                                vec.clear();
                                for (int i = 0; i < cktin.size(); ++i) {
                                    vec.push_back(itoc(cktin[i]->value));
                                }
                                break;
                            }
                            cktu.clear();
                            cktin.clear();
                            sort_wlist.clear();
                            for(auto u : n_cktu)
                                cktu.push_back(u);
                            for(auto in : n_cktin)
                                cktin.push_back(in);
                            for(auto w : n_sort_wlist)
                                sort_wlist.push_back(w);
                        }
                    }
                    /* Add one bit for v1 */
                    vec1.clear();
                    vec2.clear();
                    vec1 = vec.substr(1, vec.size()-1) + '0' + vec[0];
                    vec2 = vec.substr(1, vec.size()-1) + '1' + vec[0];
                    // if(max_tried - cur_tried < 1) {
                    //     v1_s = false;
                    //     v2_s = false;
                    //     for(int j = 0; j < detected_num * 2; ++j) {
                    //         if(!tdfault_sim_a_fault(vec1, fault_under_test)) {
                    //             for(int k = 0; k < cktu.size(); ++k) {
                    //                 if(cktu[i]) {
                    //                     if(rand() & 01) {
                    //                         if(i == 0) 
                    //                             vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                    //                         else 
                    //                             vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                    //                     }
                    //                 }
                    //             }
                    //         }
                    //         else {
                    //             v1_s = true;
                    //             break;
                    //         }
                    //     }
                    //     for(int j = 0; j < detected_num; ++j) {
                    //         if(!tdfault_sim_a_fault(vec2, fault_under_test)) {
                    //             for(int k = 0; k < cktu.size(); ++k) {
                    //                 if(cktu[i]) {
                    //                     if(rand() & 01) {
                    //                         if(i == 0) 
                    //                             vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                    //                         else 
                    //                             vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                    //                     }
                    //                 }
                    //             }
                    //         }
                    //         else {
                    //             v2_s = true;
                    //             break;
                    //         }
                    //     }
                    //     if(!v1_s && v2_s)
                    //         vec2 = vec1;
                    //     if(v1_s && !v2_s)
                    //         vec1 = vec2;
                    // }
                    /* run a fault simulation, drop ALL detected faults */
                    current_detect_num = cur_min_det;
                    checkRepeat(vec1, vec2, result, current_detect_num, current_fault_detected);
                    //result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
                    // result = 1 means vec1, 0 means vec2, -1 means both detect num = 0
                    if(result != -1) {
                        storePtn(vec1, vec2, result, current_fault_detected);
                        first_try++;
                        break;
                    }

                    // Generate random pattern on unknown bits
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
                    current_detect_num = cur_min_det;
                    checkRepeat(vec1, vec2, result, current_detect_num, current_fault_detected);
                    //result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
                    if(result != -1) {
                        storePtn(vec1, vec2, result, current_fault_detected);
                        random++;
                        break;
                    }
                    
                    // All nnknown bit flip to 0 / 1
                    for(int j = 0; j < 2; ++j) {
                        for(int i = 0; i < cktu.size(); ++i) {
                            if(cktu[i]) {
                                if(i == 0) {
                                    vec1[cktu.size()] = in_val[j];
                                    vec2[cktu.size()] = in_val[j];
                                }
                                else {
                                    vec1[i-1] = in_val[j];
                                    vec2[i-1] = in_val[j];
                                }
                            }    
                        }
                        current_detect_num = cur_min_det;
                        checkRepeat(vec1, vec2, result, current_detect_num, current_fault_detected);
                        //result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
                        if(result != -1) {
                            storePtn(vec1, vec2, result, current_fault_detected);
                            all_zo++;
                            break;
                        }
                        
                    }
                    if(result != -1)
                        break;
                    
                    // Generate random pattern on unknown bits
                    for(int i = 0; i < cktu.size() + 1; ++i) {
                        if(cktu[i]) {
                            if(rand() & 01) {
                                if(i == 0) {
                                    vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                                }
                                else {
                                    vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                                }
                            }
                            else {
                                if(i == 0)
                                    vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                                else 
                                    vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                            }
                        }
                    }
                    current_detect_num = cur_min_det;
                    checkRepeat(vec1, vec2, result, current_detect_num, current_fault_detected);
                    //result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
                    if(result != -1 ) {
                        storePtn(vec1, vec2, result, current_fault_detected);
                        random_2++;
                        break;
                    }
                    
                    // Flip every unsigned bit one by one if unknown bit size is small
                    unknown_bit = 0;
                    //unknown_ratio = 0.0;
                    for(int i = 0; i < cktu.size(); ++i) {
                        if(cktu[i]) unknown_bit++;
                    }
                    //unknown_ratio = (float) unknown_bit / vec1.size();
                    // if(unknown_bit > 0 && unknown_bit < 15) {
                    //     for(int i = 0; i < cktu.size() + 1; ++i) {
                    //         if(cktu[i]) {
                    //             if(i == 0) {
                    //                 vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                    //                 vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                    //             }
                    //             else {
                    //                 vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                    //                 vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                    //             }
                    //             current_detect_num = cur_min_det;
                    //             checkRepeat(vec1, vec2, result, current_detect_num, current_fault_detected);
                    //             //result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
                    //             if(result != -1) {
                    //                 storePtn(vec1, vec2, result, current_fault_detected);
                    //                 flip++;
                    //                 break;
                    //             }
                    //             if((float) rand() / RAND_MAX < 0.5) {
                    //                 if(i == 0) 
                    //                     vec1[cktu.size()] = (vec1[cktu.size()] == '0')? '1' : '0';
                    //                 else
                    //                     vec1[i-1] = (vec1[i-1] == '0')? '1' : '0';
                    //             }
                    //             else {
                    //                 if(i == 0) {
                    //                     vec2[cktu.size()] = (vec2[cktu.size()] == '0')? '1' : '0';
                    //                 }
                    //                 else {
                    //                     vec2[i-1] = (vec2[i-1] == '0')? '1' : '0';
                    //                 }
                    //             }
                    //         }
                    //     }
                    //     if(result != -1)
                    //         break;
                    // }
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
                //if(cur_tried % 2 == 0)
                    break;
            }
        }
        // total_no_of_backtracks += current_backtracks; // accumulate number of backtracks
        // no_of_calls++;
        // If there are still undetected faults and haven't reach max tried round
        if(fault_under_test == nullptr && cur_tried < max_tried) {
            cur_tried++;
            chg_seed = true;
            for(fptr fptr_ele : flist_undetect) {
                fault_under_test = fptr_ele;
                fptr_ele->test_tried = false;
            }
            flist_undetect.sort(my_cmp);
        }
    }
    if(detected_num > 2) {
        flist_size = distance(flist_undetect.begin(), flist_undetect.end()); 
        cnt = 0;
        int cur_vec_num = in_vector_no, min_det;
        for(int i = 0; i < detected_num - 1; ++i) {
            // Traverse every test pattern
            for(int j = cur_vec_num - 1; j >= 0; --j) {
                // Reset fault be_detect flag and fault count
                for(auto f : flist_undetect) {
                    f->be_det = 0;
                }
                cnt = 0;
                // Go through all the faults the pattern detected
                for(auto f : vectors_faults[j]) {
                    f->be_det = 1;
                    if(f->detected_time < detected_num && f->detected_time > 1 + i)
                        cnt++;
                }
                // Remove newly detected faults and store pattern, include DTC
                min_det = (test_compression)? 3 - i / 2 : 1 - i / 4;
                if(cnt > max(min_det, 0)) {
                    current_fault_detected.clear();
                    tdfault_fault_drop(1, current_fault_detected);
                    storePtn(tdf_vectors[j], tdf_vectors[j], 1, current_fault_detected);
                }
            }
        }
    }
    
    // cout << "# -------------------\n";
    // cout << "# First try detect: " << first_try << endl;
    // cout << "# Random    detect: " << random << endl;
    // cout << "# All 0 / 1 detect: " << all_zo << endl;
    // cout << "# Random 2  detect: " << random_2 << endl; 
    // cout << "# Bit flip  detect: " << flip << endl;
    display_undetect();
}

int ATPG::tdfsim_v1v2(const string& vec1, const string& vec2, int& current_detect_num, vector<fptr>& current_fault_detected) {
    int vec1_det_num = 0, vec2_det_num = 0; // fault detected by the pattern
    // Include DTC
    int min_det = (test_compression)?current_detect_num : 1;  // minimum detect number of this round

    current_fault_detected.clear();

    // Reset fault be_detect flag
    for(auto f : flist_undetect) {
        f->be_det = 0;
    }

    current_detect_num = 0;
    tdfault_sim_a_vector(vec1,  current_detect_num, true, 1);
    vec1_det_num = current_detect_num;
    reset_fault_detect_status();
    tdfault_sim_a_vector(vec2,  current_detect_num, true, 2);
    vec2_det_num = current_detect_num;
    current_detect_num = 0;
    if(vec1_det_num > vec2_det_num && vec1_det_num >= min_det) {
        tdfault_fault_drop(1, current_fault_detected);
        current_detect_num += vec1_det_num;
        //fprintf(stdout, "vector detects %d faults\n", vec1_det_num);
        return 1;
    }
    else if(vec1_det_num == vec2_det_num) {
        int res;
        if(vec2_det_num < min_det) {
            reset_fault_detect_status();
            return -1;
        }
        res = rand() & 01;
        if(res) {
            tdfault_fault_drop(1, current_fault_detected);
            current_detect_num += vec1_det_num;
            //fprintf(stdout, "vector detects %d faults\n", vec1_det_num);
        }
        else {
            tdfault_fault_drop(2, current_fault_detected);
            current_detect_num += vec2_det_num;
            //fprintf(stdout, "vector detects %d faults\n", vec2_det_num);
        }
        return res;
    }
    else {
        if(vec2_det_num >= min_det) {
            tdfault_fault_drop(2, current_fault_detected);
            current_detect_num += vec2_det_num;
            //fprintf(stdout, "vector detects %d faults\n", vec2_det_num);
            return 0;
        }
        reset_fault_detect_status();
        return -1;
    }
    reset_fault_detect_status();
    return -1;
}

int ATPG::tdfsim_v1v2(const string& vec1, int& current_detect_num, vector<fptr>& current_fault_detected) {
    int vec1_det_num = 0; // fault detected by the pattern
    // Include DTC
    int min_det = (test_compression)?current_detect_num : 1; // minimum detect number of this round

    // Reset vector of current detect fault and fault be_detect flag
    current_fault_detected.clear();
    for(auto f : flist_undetect) {
        f->be_det = 0;
    }
    current_detect_num = 0;

    tdfault_sim_a_vector(vec1, current_detect_num, true, 1);
    vec1_det_num = current_detect_num;
    reset_fault_detect_status();
    current_detect_num = 0;
    if(vec1_det_num >= min_det) {
        tdfault_fault_drop(1, current_fault_detected);
        current_detect_num += vec1_det_num;
        return 1;
    }
    reset_fault_detect_status();
    return -1;
}

void ATPG::reset_fault_detect_status() {
    for(auto f : flist_undetect) {
        if(f->detect != REDUNDANT)
            f->detect = FALSE;
    }
}

void ATPG::restoreFaultList() {
    for (auto f : flist_detect) {
        flist_undetect.push_front(f);
    }
    
    for (auto pos = flist_undetect.begin(); pos != flist_undetect.end(); ++pos) {
        fptr f = *pos;
        f->detect = FALSE;
        f->detected_time = 0;
    }
}

void ATPG::fSTC() {
    int current_detect_num = 0;
    int total_detect_num = 0;
    
    for (int i = tdf_vectors.size() - 1; i >= 0; i--) {
        bool sim_pat = false;

        for (auto f : vectors_faults[i]) { 
            if ((f->atpg_detected_time + f->detected_time) == detected_num) {
                sim_pat = true;
            }
            f->atpg_detected_time--;
        }
        if (!sim_pat) {
            tdf_vectors.erase(tdf_vectors.begin() + i);
            continue;
        }
        tdfault_sim_a_vector(tdf_vectors[i], current_detect_num, false, 0);
        total_detect_num += current_detect_num;
        display_io_tdf(tdf_vectors[i]);
    }

    in_vector_no = tdf_vectors.size();
}

void ATPG::checkRepeat(string vec1, string vec2, int& result, int& current_detect_num, vector<fptr>& current_fault_detected) {
    bool vec1_val = true;
    bool vec2_val = true;

    for (string v : tdf_vectors) { 
        if (v == vec1) {
            vec1_val = false;
        }
        if (v == vec2) {
            vec2_val = false;
        }
        if(!vec1_val && !vec2_val)
            break;       
    }

    if (vec1_val && vec2_val && vec1 != vec2) {
        result = tdfsim_v1v2(vec1, vec2, current_detect_num, current_fault_detected);
    } 
    else if (vec1_val) {
        result = tdfsim_v1v2(vec1, current_detect_num, current_fault_detected);
        if (result != -1) result = 1;
    }
    else if (vec2_val) {
        result = tdfsim_v1v2(vec2, current_detect_num, current_fault_detected);
        if (result != -1) result = 0;
    }
    else {
        result = -1;
    }
}

void ATPG::checkRepeat(string vec1, int& result, int& current_detect_num, vector<fptr>& current_fault_detected) {
    bool vec1_val = true;
    for (string v : tdf_vectors) { 
        if (v == vec1) {
            vec1_val = false;
            break;
        }    
    }
    if (vec1_val) {
        result = tdfsim_v1v2(vec1, current_detect_num, current_fault_detected);
        if (result != -1) result = 1;
    }
    else {
        result = -1;
    }
}

void ATPG::storePtn(string& vec1, string& vec2, int result, vector<fptr>& current_fault_detected) {
    if(result == 1) {
        vectors_faults.push_back(current_fault_detected);
        tdf_vectors.push_back(vec1);  //for STC
        if (!test_compression) display_io_tdf(vec1);
        //total_detect_num +=vec1_det_num;
        in_vector_no++;
    }
    if(result == 0) {
        vectors_faults.push_back(current_fault_detected);
        tdf_vectors.push_back(vec2);  //for STC
        if (!test_compression) display_io_tdf(vec2);
        //total_detect_num +=vec2_det_num;
        in_vector_no++;
    }
}