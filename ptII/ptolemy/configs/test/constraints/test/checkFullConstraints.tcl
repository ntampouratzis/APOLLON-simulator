# Tests for checking clone constraints configurations
#
# @Author: Steve Neuendorffer, Contributor: Christopher Hylands
#
# $Id$
#
# @Copyright (c) 2000-2019 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
# 						PT_COPYRIGHT_VERSION_2
# 						COPYRIGHTENDKEY
#######################################################################

# Ptolemy II test bed, see $PTII/doc/coding/testing.html for more information.

# Load up the test definitions.
if {[string compare test [info procs test]] == 1} then {
    source testDefs.tcl
} {}

#set VERBOSE 1

if {[string compare test [info procs parseConfiguration]] == 1} then {
    source ../../configurationTools.tcl
} {}

puts "FIXME FIXME FIXME FIXME FIXME FIXME" 
puts "ptolemy/configs/test/checkFullConstraints.tcl: FIXME: Invoking Configuration.check() "
puts "results in a memory leak that consumes"
puts "more than 4gig of memory and causes problems under Travis."
puts "so, we check the constraints only of the full configuration."
puts "FIXME FIXME FIXME FIXME FIXME FIXME" 

checkConstraints full/configuration.xml

puts "checkFullConstraints.tcl done!"
