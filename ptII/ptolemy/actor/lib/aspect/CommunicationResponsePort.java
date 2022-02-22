/* This actor implements an output port in a composite communication aspect.

@Copyright (c) 2011-2018 The Regents of the University of California.
All rights reserved.

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the
above copyright notice and the following two paragraphs appear in all
copies of this software.

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
package ptolemy.actor.lib.aspect;

import java.util.LinkedList;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.data.Token;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Workspace;

/** This actor implements an output port in a composite communication aspect
 *  (@link CompositeCommunicationAspect).
 *
 *  @author Patricia Derler
 *  @version $Id$
 *  @since Ptolemy II 10.0
 *  @Pt.ProposedRating Yellow (derler)
 *  @Pt.AcceptedRating Red (derler)
 */
public class CommunicationResponsePort extends TypedAtomicActor {

    /** Construct a ResourceMappingOutputPort. The contained entities (SetVariable,
     *  Parameter and input port) are created from the XML description
     *  in the library.
     *
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the container is incompatible
     *   with this actor.
     *  @exception NameDuplicationException If the name coincides with
     *   an actor already in the container.
     */
    public CommunicationResponsePort(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        input = new TypedIOPort(this, "input", true, false);
        _tokens = new LinkedList<Token>();
    }

    /** Clone the actor into the specified workspace.
     *  @param workspace The workspace for the new object.
     *  @return A new actor.
     *  @exception CloneNotSupportedException If a derived class contains
     *   an attribute that cannot be cloned.
     */
    @Override
    public Object clone(Workspace workspace) throws CloneNotSupportedException {
        CommunicationResponsePort newObject = (CommunicationResponsePort) super.clone(
                workspace);
        newObject._tokens = new LinkedList<Token>();
        return newObject;
    }

    /** The input port. */
    public TypedIOPort input;

    /** Initialize actor and clear Parameter value in case it was set
     *  in a previous execution.
     */
    @Override
    public void initialize() throws IllegalActionException {
        super.initialize();
        _tokens.clear();
    }

    /** Check whether the contained parameter contains a token.
     *  @return True if the contained parameter contains a token.
     */
    public boolean hasToken() {
        return (!_tokens.isEmpty());
    }

    /** Get token from parameter and remove it from the parameter.
     *  @return The token, or null if there are no tokens.
     */
    public Token takeToken() {
        return _tokens.poll();
    }

    /** Store all available input tokens.
     *  @exception IllegalActionException Not thrown here.
     */
    @Override
    public void fire() throws IllegalActionException {
        super.fire();
        while (input.hasToken(0)) {
            _tokens.add(input.get(0));
        }
    }

    /** Get the token from parameter but do not remove it.
     *  @return The token, or null if there are no tokens.
     *  @exception IllegalActionException Not thrown.
     */
    public Token getToken() throws IllegalActionException {
        return _tokens.peek();
    }

    private LinkedList<Token> _tokens;

}
