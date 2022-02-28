# APOLLON
During the last decade, highly parallel and distributed Cyber Physical Systems (CPS) are growing in capability at an extraordinary rate, incorporating processing systems that vary from simple microcontrollers to high performance units and CPS physical aspects connected with each other through numerous networks. As a result, the complexity of those systems, their large scale and heterogeneity, emphasizes the urge to better modeling and simulation strategies of these systems at design-time. One of the main problems the designers of such systems face is the lack of simulation tools that can offer realistic insights beyond simple functional testing, such as the actual performance of the nodes, accurate overall system timing, power/energy estimations, and network deployment issues. On top of that, none of the existing solutions provide simulation of physical aspects of CPS so as to be able to simulate the whole CPS system in an integrated manner during design-time.

In this work, we present the APOLLON Simulation Framework which is an extension of an open-source framework (i.e. COSSIM [[1]](#1)) that aims to address all the aforementioned limitations. COSSIM efficiently integrates a series of sub-tools that model the computing devices of the processing nodes as well as the network(s) of the parallel systems. It provides cycle accurate results by simulating the actual application and system software executed on each node, together with the actual networks employed and it provides power/energy consumption estimation for both the processing elements and the network based on the actual dynamic usage scenarios. In this work, we extend COSSIM by introducing a novel flow that enables the designer to rapidly simulate the physical aspects of the CPSs in conjunction with the Processing & Network systems without worrying about communication and synchronisation issues. This allows CPS designers to simulate not only the software part of a CPS (on the processing units) but also its physical part within the same environment; this novel aspect significantly accelerates the final product delivery. Last but not least, APOLLON employs a standardized interconnection protocol between its sub-components (i.e. IEEE 1516.x  HLA (IEEE 1516.x: Standard for Modeling and Simulation High Level Architecture (HLA).), thus it can be seamlessly connected to additional similar tools.

Implementation details is presented in [[2]](#2) journal paper.

## Installation & Execution

In order to install all parts of APOLLON, you may read the file [Apollon Installation](apollon_install.sh) for aGEM5, aOMNET++ and HLA as well as the file [Ptloemy II Installation Notes](ptolemy-ii-version11-linux-installation-notes.txt) for Ptolemy II installation. We have tested an verified the process on Ubuntu 16.04 (similar commands can be used to install the tools in the newer versions of Ubuntu). 

In addition we have created and distributed a Virual Machine which we have install everything there. You may donwload it from [here] (http://kition.mhl.tuc.gr:8000/d/6331880f95/) (It is tested using VMWare tools). Finally here you can see videos with execution examples [Apollon gem5-Ptolemy](http://kition.mhl.tuc.gr:8000/f/6765b61cc3) , [APOLLON 2nodes](http://kition.mhl.tuc.gr:8000/f/fa7a952458).


## References
<a id="1">[1]</a> 
N. Tampouratzis, I. Papaefstathiou, A. Nikitakis, A. Brokalakis,
S. Andrianakis, A. Dollas, M. Marcon, and E. Plebani, “A novel,
highly integrated simulator for parallel and distributed systems,”
ACM Trans. Archit. Code Optim., vol. 17, no. 1, Mar. 2020.
Available: https://dl.acm.org/doi/10.1145/3378934

<a id="2">[2]</a> 
To be defined!!
