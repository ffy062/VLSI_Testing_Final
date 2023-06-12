# ndet = 1, no compression
./atpg -tdfatpg ../sample_circuits/c17.ckt > ../check_patterns/c17_noc.pat
./atpg -tdfatpg ../sample_circuits/c432.ckt > ../check_patterns/c432_noc.pat
./atpg -tdfatpg ../sample_circuits/c499.ckt > ../check_patterns/c499_noc.pat
./atpg -tdfatpg ../sample_circuits/c880.ckt > ../check_patterns/c880_noc.pat
./atpg -tdfatpg ../sample_circuits/c1355.ckt > ../check_patterns/c1355_noc.pat
./atpg -tdfatpg ../sample_circuits/c2670.ckt > ../check_patterns/c2670_noc.pat
./atpg -tdfatpg ../sample_circuits/c3540.ckt > ../check_patterns/c3540_noc.pat
./atpg -tdfatpg ../sample_circuits/c6288.ckt > ../check_patterns/c6288_noc.pat
./atpg -tdfatpg ../sample_circuits/c7552.ckt > ../check_patterns/c7552_noc.pat

# ndet = 8, no compression
./atpg -tdfatpg -ndet 8 ../sample_circuits/c17.ckt > ../check_patterns/c17_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c432.ckt > ../check_patterns/c432_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c499.ckt > ../check_patterns/c499_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c880.ckt > ../check_patterns/c880_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c1355.ckt > ../check_patterns/c1355_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c2670.ckt > ../check_patterns/c2670_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c3540.ckt > ../check_patterns/c3540_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c6288.ckt > ../check_patterns/c6288_8_noc.pat
./atpg -tdfatpg -ndet 8 ../sample_circuits/c7552.ckt > ../check_patterns/c7552_8_noc.pat