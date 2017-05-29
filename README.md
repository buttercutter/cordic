# cordic

A pipelined cordic algorithm for computing approximate values of cos(angle) and sin(angle), tested using verilator against input angles ranging from -180 degree to +180 degree with output tolerance of 0.01

Input angles are fed into unit under test (uut) every clock cycle, thus after N (number of cordic iterations) pipeline stages, it will output results for cos(angle) and sin(angle) every clock cycle.

Note on usage:
1) cd bench/
2) make distclean && make
3) ./cordic_pipelined/Vcordic_pipelined
4) Then, verbose text output for approximated cordic results and exact results are shown

For more information on cordic algorithm itself, please read http://www.andraka.com/files/crdcsrvy.pdf
