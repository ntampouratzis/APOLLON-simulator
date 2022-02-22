-- Author: David Come, updated Janette Cadoso - ISAE SUPAERO
-- janette.cardoso@isae-supaero.fr
-- Date: updated on May, 9,2016; July, 25, 2017

This folder contains two similar demos:
- 2Billes2Fed: two federates BillardBall1.xml and BillardBall2.xml, each one
registering/publishing a ball instance, and a third one, BillardTable.xml that
displays the two ball instances.

- 2Billes1Fed: one federate TwoBillardBalls.xml that registering/publishing
two ball instances, and a second one, BillardTable.xml that displays the two
ball instances.

Both demo 2Billes1Fed  and 2Billes2Fed  can interact with C++ federates.

Interaction with C++ federates:
-------------------------------
For having a correct interaction between C++ and Ptolemy billiard federates,
the patched version of billiard.cc must be used (/** pour recevoir la nouvelle
position de la bille en 2 RAV avec un seul attribut par RAV */).

There should be no memory effect with BilliarTable when running several
simulations in a row. But in case of doubt, I advice to close it and re-open.

1-  BilliardTable with N C++ federate (in BilliardTable, Make sure that you
disable the synchronization point in the HlaManager)
* kill rtig if need and launch it back.
* launch the C++ billard first
* Launch BilliardTable and SingleBillardBall (or TwoBilliardBalls) and the
  N-1 C++ billard
* Get back to the first C++ billard and press ENTER

or : run 2Billes1Fed.xml (or 2Billes2Fed.xml)  and follow the instructions

Launch order does not matter between Ptolemy federates when using Billard.cc
because the C++ federate is the register of the synchronization point.
