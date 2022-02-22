/**
 *
 * Copyright (c) 2013-2017 The Regents of the University of California.
 * All rights reserved.
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the above
 * copyright notice and the following two paragraphs appear in all copies
 * of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 * THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 * CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * PT_COPYRIGHT_VERSION_2
 * COPYRIGHTENDKEY
 *
 */

package org.ptolemy.scala.actor

import language.dynamics
import scala.collection.mutable.HashMap

import org.ptolemy.scala.implicits._

/**
 * @author Moez Ben Hajhmida
 *
 */
abstract class TypedAtomicActor extends ptolemy.actor.TypedAtomicActor with ComponentEntity with Dynamic {

  /**
   * Creates a map between a name and a ptolemy.actor.TypedIOPort.
   * Used by selectDynamic(name: String) and updateDynamic(name:String)(value: ptolemy.actor.TypedIOPort).
   * Used to dynamically (during runtime) add and manage dynamic(new) fields to an object.
   */

  private val map = new HashMap[String, TypedIOPort]

  /**
   * Returns a reference to the wrapped actor.
   * This method returns the Ptolemy actor (java) wrapped  by the scala actor.
   * It's useful when the developer wants to access the java methods.
   *
   *  @return Reference to the wrapped actor.
   */
  def getActor(): ptolemy.actor.TypedAtomicActor

  /**
   * Allows to write field accessors.
   * Used to dynamically create accessors for a dynamic field of an object.
   * @param name The name of the field to be dynamically added to the object.
   */

  def selectDynamic(name: String): TypedIOPort = {
    if (!map.contains(name)){
      val port =  new TypedIOPort(this, name, true, false)
      port.isDynamicInstance = true
       map(name) = port
    }
    return map(name)
  }

  /**
   * Allows to write field updates.
   * Used to set a dynamic field of an object.
   * @param name The name of the dynamic field of the object.
   * @param value The Ptolemy port to be added top the object (Ptolemy actor)
   */
  def updateDynamic(name: String)(value: TypedIOPort) = {
    map(name) = value
  }

  /**
   * List all the output ports.
   *  @return A list of output TypedIOPort objects.
   */
  override def outputPortList(): java.util.List[ptolemy.actor.TypedIOPort] = {
    getActor().outputPortList()
  }

  /**
   * List all the input ports.
   *  @return A list of input TypedIOPort objects.
   */
  override def inputPortList(): java.util.List[ptolemy.actor.TypedIOPort] = {
    getActor().inputPortList()
  }
}