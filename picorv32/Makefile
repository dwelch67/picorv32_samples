
obj_dir/Vptb : vmain.cpp ptb.v picorv32.v
	verilator --cc ptb.v picorv32.v --exe vmain.cpp --trace --trace-underscore 
	make -C obj_dir/ -f Vptb.mk

clean :
	rm -rf obj_dir
	rm -f dut.vcd


