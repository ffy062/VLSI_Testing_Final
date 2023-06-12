# DTC, STC, all
mode="all"

# ndet = 1, compression
./atpg -tdfatpg -compression ../sample_circuits/c17.ckt > ../check_patterns/c17_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c432.ckt > ../check_patterns/c432_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c499.ckt > ../check_patterns/c499_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c880.ckt > ../check_patterns/c880_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c1355.ckt > ../check_patterns/c1355_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c2670.ckt > ../check_patterns/c2670_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c3540.ckt > ../check_patterns/c3540_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c6288.ckt > ../check_patterns/c6288_${mode}.pat
./atpg -tdfatpg -compression ../sample_circuits/c7552.ckt > ../check_patterns/c7552_${mode}.pat

# ndet = 8, compression
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c17.ckt > ../check_patterns/c17_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c432.ckt > ../check_patterns/c432_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c499.ckt > ../check_patterns/c499_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c880.ckt > ../check_patterns/c880_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c1355.ckt > ../check_patterns/c1355_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c2670.ckt > ../check_patterns/c2670_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c3540.ckt > ../check_patterns/c3540_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c6288.ckt > ../check_patterns/c6288_8_${mode}.pat
./atpg -tdfatpg -ndet 8 -compression ../sample_circuits/c7552.ckt > ../check_patterns/c7552_8_${mode}.pat