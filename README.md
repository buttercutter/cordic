# cordic
A pipelined cordic algoithm for computing cos(angle) and sin(angle)


I am still working on "pipelined testbench" (feed unit under test with a new sample every clock)

Just to share.
A pipelined cordic algoithm for computing cos(angle) and sin(angle) with "non-pipelined testbench" (feed unit under test with a new sample every N pipeline stages)
This had been tested using verilator against input angles ranging from -180 degree to +180 degree with output tolerance of 0.01
