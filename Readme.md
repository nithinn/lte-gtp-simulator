===============================================================================
1. LTE GTP Simulator
-------------------------------------------------------------------------------
LTE GTP Simulator is a 3gpp LTE GTPv2-C signaling simulator and gtp load generator. Any LTE GTP signaling scenario can easily be represented in XML and the simulator plays out the scenario. The simulator provides command line options to control the scenario execution. A scenario file, written in xml is a sequence of GTPC messages that is exchanged between between two LTE core network nodes. Each message, its informational elements etc are represented in XML. The scenario is then simulated by the GTP Simulator for a single UE or for multiple UEs in the form of load generator.


2. Build
-------------------------------------------------------------------------------
To build:
```
$ git clone https://github.com/nithinn/LTE-GTP-Simulator.git
$ cd LTE-GTP-Simulator
$ mkdir build
$ cd build
$ cmake ..
```


3. Simulator Options
-------------------------------------------------------------------------------
To list all command line options:
```
$ gsim --help
```
Or visit the [Wiki](https://github.com/nithinn/LTE-GTP-Simulator/wiki) Page.


4. Running the Simulator
-------------------------------------------------------------------------------
gsim --node=<node-type> --scenario=<scenario-file> [options...] 

The below example show S11 interface simulation between MME and SGW. The scenario simulates GTPv2-C signalling of a basic attach procedure.

**MME:**
```
./build/gsim --node=mme --scenario=scenario/mme_s11.xml
```


**SGW:**
```
./build/gsim --node=sgw --scenario=scenario/sgw_s11.xml
```

The below example show S5 interface simulation between SGW and PGW. The scenario simulates GTPv2-C signalling of a basic attach procedure.

**SGW:**
```
./build/gsim --node=sgw --scenario=scenario/sgw_s5.xml
```

**PGW:**
```
./build/gsim --node=pgw --scenario=scenario/pgw_s5.xml
```

5. Full Documentation
-------------------------------------------------------------------------------
Want to know more? Visit the [Wiki](https://github.com/nithinn/LTE-GTP-Simulator/wiki) Page.

===============================================================================
