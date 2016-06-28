vsim work.iir_gen_test
delete wave *'
add wave -position insertpoint sim:/iir_gen_test/*
run -all
