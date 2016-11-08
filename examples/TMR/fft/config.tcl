source ../../legup.tcl
set_project CycloneII DE2 Tiger_SDRAM

set_parameter TMR 1
set_parameter VOTER_MODE 0
# VOTER_MODE 0=output only,
#            1=all reigisters,
#            2=dest signal(fanin, phi),
#            3=source signal(fanout),
#            4=dest signal with reg(registered phi)
