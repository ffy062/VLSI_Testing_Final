mode="all" # mode = DTC, STC, all
circuit_numbers=("c17" "c432" "c499" "c880" "c1355" "c2670" "c3540" "c6288" "c7552")

for circuit_number in "${circuit_numbers[@]}"; do
  # Loop through N values from 1 to 8
  for n in {1..8}; do
    ./atpg -tdfatpg -ndet ${n} -compression ../sample_circuits/${circuit_number}.ckt > ../check_patterns/all/${circuit_number}_${n}_${mode}.pat
    ./atpg -tdfatpg -ndet ${n} ../sample_circuits/${circuit_number}.ckt > ../check_patterns/noc/${circuit_number}_${n}_noc.pat
  done
done
