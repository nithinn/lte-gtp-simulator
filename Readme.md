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
$ make
```


3. Simulator Options
-------------------------------------------------------------------------------
run the command 'gsim --help'


4. Running the Simulator
-------------------------------------------------------------------------------
gsim [options] scenarion_file.xml


5. Full Documentation
-------------------------------------------------------------------------------
Want to know more? Visit the [Wiki](https://github.com/nithinn/LTE-GTP-Simulator/wiki) Page.

===============================================================================
