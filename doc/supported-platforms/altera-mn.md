openPOWERLINK MN on Altera Nios II {#page_platform_altera-mn}
==================================

[TOC]

# Introduction {#sect_altera-mn_introduction}

The POWERLINK MN on Altera FPGA implementation utilizes IP-Cores available in
Altera Qsys and provided with the openPOWERLINK stack in the directory
`hardware/ipcore`.

The MN implementation uses the PCP (POWERLINK Communication Processor)
and the host processor to execute the openPOWERLINK kernel and user layer.
The soft-core processor Altera Nios II is instantiated as PCP and also as
host processor.

The Ethernet interface is built with the POWERLINK-optimized controller openMAC,
which enables e.g. low SoC jitters. Additionally, a hub IP-Core is provided
enabling daisy chained networks.

The two processors are connected by the Host Interface IP-Core,
which supports the following interface implementations:
- Altera Avalon interconnect
- External (de-)multiplexed address-/data-bus

# Contents {#sect_altera-mn_contents}

The following MN FPGA designs are available for the TERASIC DE2-115 INK board:
- PCP and host design within a single FPGA (driver and application): \n
    `hardware/boards/terasic-de2-115/mn-dual-hostif-gpio`
- PCP design with parallel host interface (driver instance): \n
    `hardware/boards/terasic-de2-115/mn-single-hostif-drv`
- Host processor design with parallel host interface (application): \n
    `hardware/boards/terasic-de2-115/mn-single-hostif-gpio`

_Note that hardware designs can be ported easily to other platforms by reusing
the Qsys subsystem instances in `hardware/ipcore/altera/qsys/mn_[pcp|host]`!_

The software projects for the PCP and the host processor are split into
`drivers` and `apps`:
- PCP driver daemon: \n
    `drivers/altera-nios2/drv_daemon/build`
- Host "demo_mn_embedded": \n
    `apps/demo_mn_embedded/build/altera-nios2`

# Requirements {#sect_altera-mn_requirements}

- Altera Quartus II v13.0 SP1 (Web Edition is also possible)
  - <https://www.altera.com/download/archives/arc-index.jsp>
  - <ftp://ftp.altera.com/outgoing/release/>
- Experience with this development environment is required.
- Development Board TERASIC_DE2-115 (INK Board)
- Optional: Second development Board TERASIC_DE2-115 (INK Board)
  if you want to run the separate FPGA demo.
- Optional: 40-line ribbon cable to connect two INK boards via JP5
  (length as short as possible).

# Hardware Setup {#sect_altera-mn_hardware}

- Setup for TERASIC_DE2-115 (INK)
 - Download the user guide for the board from the
 - [Terasic Website](http://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=139&No=502&PartNo=4)
 - Set the jumpers JP1, JP2, JP3, JP6 and JP7.
 - Set switch SW 19 to Run.

- Single FPGA demo
 - Connect one Ethernet port of the INK-board to the POWERLINK network.
 - Switch on the board with the Power button (SW 18).
- Two FPGA demo
 - Connect one Ethernet port of one INK-board to the POWERLINK network.
 - Make sure that JP6 on both INK boards are at same position (3.3V).
 - Connect the two INK boards with a 40-line ribbon cable through JP5.
 - Switch on the two boards with the Power button (SW 18).

# How to build the binaries {#sect_altera-mn_build}

The Altera Quartus II projects are located in
`hardware/boards/[EVALBOARD]/quartus.`\n
The subdirectories starting with `mn-` refer to designs for POWERLINK MN demos.

Steps 1-5 can be carried out by executing `$ make all` in a
"Nios II Command Shell".

1. Open the Quartus project file `*.qpf` with Altera Quartus II according to the
   demo you want to use.
2. Open Qsys via menu *Tools* -> *Qsys*.
3. Open the Qsys file (`*.qsys`) in the demo's directory, change to
   *Generation* page
   and press the button *Generate* to generate the Nios II system.
4. Close Qsys when the generation has finished (shown as information output).
5. Start the compilation in the Quartus II window via menu *Processing* ->
   *Start Compilation*.
6. Open "Nios II Command Shell"
7. Use the software design available in the openPOWERLINK subdirectory \n
   `apps/demo_mn_embedded/build/altera-nios2` and \n
   `drivers/altera-nios2/drv_daemon/build`. \n
8. Run the scripts *create-this-drv* or *create-this-app* respectively
   to create the Makefile for the driver daemon and the application demo. \n
   `$ ./create-this-app` or `$ ./create-this-drv` \n
   Note: The script uses the TERASIC DE2-115 dual MN demo by default
   (`hardware/boards/terasic-de2-115/mn-dual-hostif-gpio/board.settings`).
   If you want to change to another board, use the `--board` option: \n
   `$ ./create-this-app --board [PATH-TO-BOARD]` or
   `$ ./create-this-drv --board [PATH-TO-BOARD]` \n
9. Run make to build the ELF file after changing the sources.\n
   `$ make`
10. Rebuild the Makefile also if the Nios II Design was changed inside Qsys. \n
    `$ ./create-this-app --rebuild` or `$ ./create-this-drv --rebuild`
11. If you want to debug the demo, use the `--debug` option. \n
    `$ ./create-this-app --debug` or `$ ./create-this-drv --debug`
12. The script's help message can be printed with `--help`. \n
    `$ ./create-this-app --help` or `$ ./create-this-drv --help`

# How to run the demo {#sect_altera-mn_run}

1. Program SOF file with Quartus II Programmer into FPGA.
   It is located in the following subdirectory of openPOWERLINK main
   directory: \n
   `hardware/boards/terasic-de2-115/mn-dual-hostif-gpio/quartus/*.sof` \n
   or \n
   `hardware/boards/terasic-de2-115/mn-single-hostif-drv/quartus/*.sof` and \n
   `hardware/boards/terasic-de2-115/mn-single-hostif-gpio/quartus/*.sof` \n
   Note that the command `$ make download-bits` in the "Nios II Command Shell"
   downloads the SOF also!
2. Download the ELF file to the target:
   Always follow the sequence by downloading the PCP ELF file first,
   afterwards the host ELF. \n
   For the two FPGA demos enter in both directories in the "Nios II Command
   Shell" \n
   `$ make download-elf` \n
3. Enjoy the running POWERLINK network.

# How to import the project into Nios II Software Build Tools for Eclipse for debugging purposes {#sect_altera-mn_import}

Requirement: Steps in the previous section *How to build the binary* are completed.

1. Start the Nios II Software Build Tools for Eclipse
2. Select menu *File -> Import...*
3. Select the import source *Nios II Software Build Tools Project* ->
   *Import Nios II Software Build Tools Project*
4. Browse to `apps/demo_mn_embedded/build/altera-nios2` (via the button
   *Browse...*)
5. Set the project name to `demo_mn_embedded`.
6. Press the button *Finish*.
7. Repeat steps 2-6 with path `drivers/altera-nios2/drv_daemon/build` and project name
   `drv_daemon`.

# How to write the program to local flash {#sect_altera-mn_flash}

This feature is not supported by now.

# Troubleshooting {#sect_altera-mn_trouble}

1. It is advised to clean all generated files after switching from one demo to
   the other.
2. Always download the PCP ELF file before the host ELF file.
3. In case of separated FPGA demo, download the PCP's design bitstream first.
