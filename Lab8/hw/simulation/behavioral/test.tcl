restart

reset
run 1000ns

put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/slv_reg0 10101010101010101010101010101010

put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/slv_reg1 00001111000011110000111100001111

put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/slv_reg2 00000000000000000000000000000100

run 1000ns

put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/mst_go 1
run 40ns
put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/mst_go 0
run 200ns

put /system/dma_controller_0/dma_controller_0/ipif_bus2ip_mst_cmplt 1
run 40ns
# Andrew just left this high the whole time
put /system/dma_controller_0/dma_controller_0/ipif_bus2ip_mst_cmplt 1 
run 300ns

put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/slv_reg2 00000000000000000000000000001100
put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/mst_go 1
run 40ns
put /system/dma_controller_0/dma_controller_0/USER_LOGIC_I/mst_go 0
run 200ns
