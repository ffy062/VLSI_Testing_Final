log_noc="../check_fsim/noc.log"
log_DTC="../check_fsim/DTC.log"
log_STC="../check_fsim/STC.log"
log_all="../check_fsim/all.log"
log_8_noc="../check_fsim/noc_8.log"
log_8_DTC="../check_fsim/DTC_8.log"
log_8_STC="../check_fsim/STC_8.log"
log_8_all="../check_fsim/all_8.log"

rm ${log_noc}
rm ${log_DTC}
rm ${log_STC}
rm ${log_all}
rm ${log_8_noc}
rm ${log_8_DTC}
rm ${log_8_STC}
rm ${log_8_all}

make clean
make

circuit_numbers=("c17" "c432" "c499" "c880" "c1355" "c2670" "c3540" "c6288" "c7552")

# ndet = 1, noc
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 1... "
    ./atpg -ndet 1 -tdfsim ../check_patterns/${circuit_number}_noc.pat ../sample_circuits/${circuit_number}.ckt >> "${log_noc}" 2>&1
done

# ndet = 1, DTC 
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 1... "
    ./atpg -ndet 1 -tdfsim ../check_patterns/${circuit_number}_DTC.pat ../sample_circuits/${circuit_number}.ckt >> "${log_DTC}" 2>&1
done

# ndet = 1, STC 
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 1... "
    ./atpg -ndet 1 -tdfsim ../check_patterns/${circuit_number}_STC.pat ../sample_circuits/${circuit_number}.ckt >> "${log_STC}" 2>&1
done

# ndet = 1, all (DTC+STC) 
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 1... "
    ./atpg -ndet 1 -tdfsim ../check_patterns/${circuit_number}_all.pat ../sample_circuits/${circuit_number}.ckt >> "${log_all}" 2>&1
done

# ndet = 8, noc
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 8... "
    ./atpg -ndet 8 -tdfsim ../check_patterns/${circuit_number}_8_noc.pat ../sample_circuits/${circuit_number}.ckt >> "${log_8_noc}" 2>&1
done

# ndet = 8, DTC
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 8... "
    ./atpg -ndet 8 -tdfsim ../check_patterns/${circuit_number}_8_DTC.pat ../sample_circuits/${circuit_number}.ckt >> "${log_8_DTC}" 2>&1
done

# ndet = 8, STC
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 8... "
    ./atpg -ndet 8 -tdfsim ../check_patterns/${circuit_number}_8_STC.pat ../sample_circuits/${circuit_number}.ckt >> "${log_8_STC}" 2>&1
done

# ndet = 8, all
for circuit_number in "${circuit_numbers[@]}"; do
    echo "Testing with circuit number ${circuit_number} with ndet = 8... "
    ./atpg -ndet 8 -tdfsim ../check_patterns/${circuit_number}_8_all.pat ../sample_circuits/${circuit_number}.ckt >> "${log_8_all}" 2>&1
done