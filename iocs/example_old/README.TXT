RGA example - Pete Owens 24-6-2004
    Example to illustrate use of the rga application 

Hardware setup:
VME 64x Crate:
Slot 1: IOC - PPC 604
Slot 8: Hytec 8002 IP Carrier 8002 card -
    Slot B: Hytec 8515 Octal serial module -
        Port 7: /ty/71/6 - connected to MKS 937A controller
            Channel 1: TS-VA-RGA-01
            Channel 2: TS-VA-RGA-02

This directory contains following files:
    st.cmd             - VxWorks startup script to initialise an IOC
    simulation_st.cmd  - VxWorks startup script to initialise a simulation IOC
    rga.sub            - substitution files to instantiate the records
    simulation_rga.sub - substitution files to instantiate the simulated records
    rga-launch.edl     - EDM pannel to launch panels for the conected gauges 
    startgui           - script to start off the user interface






