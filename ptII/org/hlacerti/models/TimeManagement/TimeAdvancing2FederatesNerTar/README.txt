$Id: README.txt 200 2018-03-29 14:42:35Z update 2019-10-10 cardoso@isae.fr $

This is a Federation with 2 Federates:

- 'TimeAdvancing2FederatesNerTarFed2TAR.xml': set parameter synchronizeToRealTime
   in DE director for forcing the time to advance in a federate

- 'TimeAdvancing2FederatesNerTarFed1NER.xml': register the synchronization point

No federate sends any UAV.

First of all set Ptolemy and CERTI environment variables as explained in the
manual in $PTII/org/hlacerti/manual-ptii-hla.pdf.

Open the file 'TimeAdvancing2FederatesTarNer.xml' with the links to above models
and some explanation.
