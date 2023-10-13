# -*- coding: utf-8 -*-
setreg("RBASE", 0)
reset()
#setclk(2, "P1")
setclk(0, "P2")

pe_latency (5)
ms (0xfee00000, 0xfeefffff, 0, 0, 1, 1, 1)
ms (0xfec0000, 0xfecfffff,  0, 0, 2, 1, 1)

max(0x00FFFFFF)
load("./test.hex")
run()
q()
