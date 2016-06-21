vsim work.fir_gen_test
delete wave *'
add wave -position insertpoint sim:/fir_gen_test/*
run -all
