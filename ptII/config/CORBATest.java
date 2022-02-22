/* Simple Java Test Program for org.omg.CORBA.Object, which is not present in JDK 1.9.

Copyright (c) 2002-2018 The Regents of the University of California.
All rights reserved.
Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

PT_COPYRIGHT_VERSION_2
COPYRIGHTENDKEY

*/
import org.omg.CORBA.Object;


/**
 * Simple Java Test Program for org.omg.CORBA.Object, which is not
 * present in JDK 1.9.
 * Success is compiling this file.
 *
 *   @author Christopher Brooks.
 *   @version $Id$
 *   @since Ptolemy II 11.0
 *   @Pt.ProposedRating Green (cxh)
 *    @Pt.AcceptedRating Red
 */
public class CORBATest {
    public static void main(String[] args) {
        System.out.println(corbaObject);
    }
    public static org.omg.CORBA.Object corbaObject = null;
}
