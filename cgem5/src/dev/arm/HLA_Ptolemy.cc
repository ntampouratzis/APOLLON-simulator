// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002-2018  ISAE-SUPAERO & ONERA
//
// This file is part of HLA_Ptolemy
//
// HLA_Ptolemy is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation ; either version 2 of the License, or
// (at your option) any later version.
//
// HLA_Ptolemy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// ----------------------------------------------------------------------------

#include <config.h>

#include "HLA_Ptolemy.hh"
#include "MessageBuffer.hh"
#include "PrettyDebug.hh"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>

using std::unique_ptr;
using std::string;
using std::endl;
using std::cout;
using std::vector;

static PrettyDebug D("HLA_Ptolemy", __FILE__);

// ----------------------------------------------------------------------------
/** Constructor
 */
HLA_Ptolemy::HLA_Ptolemy(int node, int _TotalNodes)
    : rtiamb()
    , Node(node)
    , TotalNodes(_TotalNodes)
    , handle(0)
    , creator(false)
    , nbTicks(0)
    , regulating(false)
    , constrained(false)
    , notimestamp(false)
    , // i.e. with time
    localTime(0.0)
    , TIME_STEP(1.0)
{
    
}

// ----------------------------------------------------------------------------
/** Destructor
 */
HLA_Ptolemy::~HLA_Ptolemy() throw(RTI::FederateInternalError)
{
}

// ----------------------------------------------------------------------------
/** Get the federate handle
 */
/*RTI::FederateHandle HLA_Ptolemy::getHandle() const
{
    return handle;
}*/



void 
HLA_Ptolemy::HLAInitialization(std::string federation, std::string fedfile, bool start_constrained, bool start_regulating){
    
        
        HLAInitializationRequest tmp;
        
        /* The gem5 is the only node, so, it creates the Initialization Server Signals            *
         * In the other cases (TotalNodes > 1), OMNET++ creates the Initialization Server Signals */
        if (TotalNodes==1){
            //! --- CERTI INITIALIZATION IP --- !//
            /****** Create Signal Files (HLA initialization) *****/
            tmp.type = CREATE;
            tmp.node = TotalNodes;
            
            strcpy(tmp.name, "PtolemyToGem5Signal");
            RequestFunction(tmp);
            
            strcpy(tmp.name, "Gem5ToPtolemySignal");
            RequestFunction(tmp);
            /****** END Create Signal Files (HLA initialization) *****/
            //! --- END CERTI INITIALIZATION IP --- !//
        }
    
        

        // Joins federation
        Debug(D, pdDebug) << "Create or join federation" << endl;
        this->join(federation, fedfile);

        // Continue initialisation...
        Debug(D, pdDebug) << "Synchronization" << endl;
        this->pause();
        Debug(D, pdDebug) << "Publish and subscribe" << endl;
        this->publish();
        this->subscribe();

        
        this->setTimeRegulation(start_constrained, start_regulating);
        this->tick();
        
        
        /* Write "1" in Node+1 line of Gem5ToPtolemySignal Array */
        tmp.type = WRITE;
        strcpy(tmp.name, "Gem5ToPtolemySignal");
        tmp.node = Node;
        RequestFunction(tmp);
        
        //! Wait until Ptolemy federation will be joined !//
        while(1){
            tmp.type = READ;
            strcpy(tmp.name, "PtolemyToGem5Signal");
            tmp.node = Node;
            bool ret = RequestFunction(tmp);
            if(ret){ break;}
        }
        
        this->synchronize();
        
        /* The gem5 is the only node, so, it removes the Initialization Server Signals            *
         * In the other cases (TotalNodes > 1), OMNET++ removes the Initialization Server Signals */
        if (TotalNodes==1){ 
            tmp.type = REMOVE;
        
            strcpy(tmp.name, "PtolemyToGem5Signal");
            RequestFunction(tmp);
            
            strcpy(tmp.name, "Gem5ToPtolemySignal");
            RequestFunction(tmp);
        }
    
}


// ----------------------------------------------------------------------------
/** Join the federation
    \param federation_name Federation name
    \param fdd_name Federation designator (.fed file)
 */
void HLA_Ptolemy::join(std::string federation_name, std::string fdd_name)
{
    federationName = federation_name;
    fed_file        = fdd_name;

    // create federation
    try {
        rtiamb.createFederationExecution(federation_name.c_str(), fdd_name.c_str());
        Debug(D, pdInit) << "Federation execution created." << std::endl;
        creator = true;
    }
    catch (RTI::FederationExecutionAlreadyExists& e) {
        printf("BILLARD Note : %s Reason is : %s. OK I can join it\n", e._name, e._reason);
        Debug(D, pdInit) << "Federation execution already created." << std::endl;
    }
    catch (RTI::CouldNotOpenFED& e) {
        printf("BILLARD ERROR : %s Reason is : %s\n", e._name, e._reason);
        Debug(D, pdExcept) << "BILLARD : Could not use FED file." << std::endl;
        delete &rtiamb; // Says RTIA to stop.
        exit(0);
    }

    // join federation
    bool joined = false;
    int nb = 5;

    while (!joined && nb > 0) {
        nb--;
        try {
            handle = rtiamb.joinFederationExecution("GEM5_CREATOR_FEDERATE", federation_name.c_str(), this);
            joined = true;
            break;
        }
        catch (RTI::FederateAlreadyExecutionMember& e) {
            Debug(D, pdExcept) << "Federate " << "GEM5_CREATOR_FEDERATE" << "already exists." << endl;

            throw;
        }
        catch (RTI::FederationExecutionDoesNotExist& e) {
            Debug(D, pdExcept) << "Federate " << "GEM5_CREATOR_FEDERATE" << ": FederationExecutionDoesNotExist." << std::endl;
            // sleep(1);
        }
        catch (RTI::Exception& e) {
            Debug(D, pdExcept) << "Federate " << "GEM5_CREATOR_FEDERATE" << " : Join Federation Execution failed : " << &e
                               << std::endl;
            throw;
        }
    }
}

// ----------------------------------------------------------------------------
/** Creator put federation in pause.
 */
void HLA_Ptolemy::pause()
{
    if (creator) {
        Debug(D, pdInit) << "Pause requested" << std::endl;
        try {
            rtiamb.registerFederationSynchronizationPoint("Init", "Waiting all players.");
        }
        catch (RTI::Exception& e) {
            Debug(D, pdExcept) << "Federate: Register Synchronization Point failed : " << endl;
        }
    }
}

// ----------------------------------------------------------------------------
/** Creator put federation in pause for synchronization with a friend
 */
//void HLA_Ptolemy::pause_friend()
//{
//    if (creator) {
//        Debug(D, pdInit) << "Pause requested for friend" << std::endl;
//        try {
//            // For testing purpose
//            RTI::FederateHandle numfed(0);
//            RTI::FederateHandleSet* federateSet = RTI::FederateHandleSetFactory::create(1);
//            cout << "Now we test Register Federation Synchronisation Point on some federates" << endl;
//            cout << "Please enter a federate handle (zero means none)" << endl;
//            cout << "This federate will be synchronized with the creator and not the others" << endl;
//            /* FIXME we should use C++ I/O and not mix C I/O with  C++ I/O  
//             while ( not (std::cin >> numfed) || numfed < 0) {
//	         std::cout << "That's not a number; ";
//		     std::cin.clear();
//		     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//		     }
//		     */
//            (void) scanf("%lu", &numfed);
//            if (numfed != 0) {
//                // We store numfed into the federate set
//                federateSet->add(numfed);
//                rtiamb.registerFederationSynchronizationPoint("Friend", "Synchro with a friend", *federateSet);
//            }
//        }
//        catch (RTI::Exception& e) {
//            Debug(D, pdExcept) << "Federate " << federateName << " : Register Synchronization Point failed" << endl;
//        }
//    }
//}
// ----------------------------------------------------------------------------
/** tick the RTI
 */
void HLA_Ptolemy::tick()
{
    usleep(0);
    rtiamb.tick();
    nbTicks++;
}
void HLA_Ptolemy::tick2()
{
    rtiamb.tick2();
    nbTicks++;
}

// ----------------------------------------------------------------------------
/** Set time regulation (time regulating and time constrained)
    @param start_constrained boolean, if true federate is constrained
    @param start_regulating boolean, if true federate is regulating
 */
void HLA_Ptolemy::setTimeRegulation(bool start_constrained, bool start_regulating)
{
    Debug(D, pdInit) << "Time Regulation setup" << std::endl;

    if (start_constrained) {
        if (!constrained) {
            // change from no constrained to constrained
            rtiamb.enableTimeConstrained();
            constrained = true;
            Debug(D, pdInit) << "Time Constrained enabled." << std::endl;
        }
        //rtiamb.modifyLookahead(TIME_STEP);
    }
    else {
        if (constrained) {
            // change from constrained to no constrained
            rtiamb.disableTimeConstrained();
            constrained = false;
            Debug(D, pdInit) << "Time Constrained disabled." << std::endl;
        }
    }

    if (start_regulating) {
        if (!regulating) {
            // change from no regulating to regulating
            for (;;) {
                rtiamb.queryFederateTime(localTime);

                try {
                    rtiamb.enableTimeRegulation(localTime, TIME_STEP);
                    regulating = true;
                    break;
                }
                catch (RTI::FederationTimeAlreadyPassed) {
                    // Si Je ne suis pas le premier, je vais les rattraper.
                    rtiamb.queryFederateTime(localTime);

                    RTIfedTime requestTime(((RTIfedTime&) localTime).getTime());
                    requestTime += TIME_STEP;

                    granted = false;
                    rtiamb.timeAdvanceRequest(requestTime);
                    while (!granted) {
                        try {
                            tick();
                        }
                        catch (RTI::RTIinternalError) {
                            printf("RTIinternalError Raised in tick.\n");
                            exit(-1);
                        }
                    }
                }
                catch (RTI::RTIinternalError) {
                    printf("RTIinternalError Raised in setTimeRegulating.\n");
                    exit(-1);
                }
            }
        }
    }
    else {
        if (regulating) {
            // change from regulating to no regulating
            rtiamb.disableTimeRegulation();
            regulating = false;
        }
    }
}

// ----------------------------------------------------------------------------
/** Synchronize with other federates
 */
void HLA_Ptolemy::synchronize()
{
    Debug(D, pdInit) << "Synchronize" << std::endl;

    if (creator) {
        Debug(D, pdInit) << "Creator can resume execution..." << std::endl;
        while (!paused)
            try {
                Debug(D, pdInit) << "not paused" << std::endl;
                tick();
            }
            catch (RTI::Exception& e) {
                Debug(D, pdExcept) << "******** Exception ticking the RTI : " << &e << std::endl;
                throw;
            }
        Debug(D, pdDebug) << "paused" << std::endl;

        try {
            rtiamb.synchronizationPointAchieved("Init");
        }
        catch (RTI::Exception& e) {
            Debug(D, pdExcept) << "**** Exception achieving a synchronization point by creator : " << &e << std::endl;
        }

        while (paused)
            try {
                tick();
            }
            catch (RTI::Exception& e) {
                Debug(D, pdExcept) << "**** Exception ticking the RTI : " << &e << std::endl;
                throw;
            }
    }
    else {
        printf("Synchronization...\n");

        if (!paused) {
            Debug(D, pdInit) << "Federate not paused: too early" << std::endl;
            while (!paused) {
                try {
                    tick();
                }
                catch (RTI::Exception& e) {
                    Debug(D, pdExcept) << "******** Exception ticking the RTI : " << &e << std::endl;
                    throw;
                }
            }
        }
        Debug(D, pdInit) << "Federate paused" << std::endl;

        try {
            // Federate ends its synchronization.
            rtiamb.synchronizationPointAchieved("Init");
            Debug(D, pdInit) << "Pause achieved." << std::endl;
        }
        catch (RTI::Exception& e) {
            Debug(D, pdExcept) << "**** Exception achieving a synchronization point : " << &e << std::endl;
        }

        Debug(D, pdInit) << "Federate waiting end of pause..." << std::endl;
        while (paused) {
            try {
                tick();
            }
            catch (RTI::Exception& e) {
                Debug(D, pdExcept) << "******** Exception ticking the RTI : " << &e << std::endl;
                throw;
            }
        }
        Debug(D, pdInit) << "End of pause" << std::endl;
    }

    Debug(D, pdInit) << "Federation is synchronized." << std::endl;
}

// ----------------------------------------------------------------------------
/** init ball with seed
 */
void HLA_Ptolemy::init(int seed)
{
    //local.init(seed);
}

// ----------------------------------------------------------------------------
/** init ball with values
    \param x Ball X value
    \param y Ball Y value
 */
void HLA_Ptolemy::init(int x, int y)
{
    //local.init(x, y);
}

// ----------------------------------------------------------------------------
/** create objects, regions, etc.
 */
void HLA_Ptolemy::declare(std::string federate_name)
{
    /* Check if the specific object is already registered 
       If not register it else return the function */
    for (auto it = objects_declared.cbegin(); it != objects_declared.cend(); it++){
        std::string obj = *it;
        if(federate_name == obj)
            return;
    }

    objects_declared.push_back(federate_name);
    
    unsigned long local_ID = rtiamb.registerObjectInstance(ActuatorClassID, federate_name.c_str());
    cout << "the class of the new created object is " << rtiamb.getObjectInstanceName(local_ID) << endl;
}

// ----------------------------------------------------------------------------
/** one simulation step advance)
 */
void HLA_Ptolemy::step()
{
    
    granted = false;

    try {
        rtiamb.queryFederateTime(localTime);
    }
    catch (RTI::Exception& e) {
        Debug(D, pdExcept) << "**** Exception asking for federate local time : " << &e << std::endl;
    }

    try {
        RTIfedTime time_aux(localTime.getTime() + TIME_STEP.getTime());

        Debug(D, pdDebug) << "time_aux : " << time_aux.getTime()
                          << " - localtime : " << ((RTIfedTime&) localTime).getTime()
                          << " - timestep : " << ((RTIfedTime&) TIME_STEP).getTime() << std::endl;
        granted = false;
        rtiamb.timeAdvanceRequest(time_aux);
    }
    catch (RTI::Exception& e) {
        Debug(D, pdExcept) << "******* Exception sur timeAdvanceRequest." << std::endl;
    }

    /*for (vector<Ball>::iterator it = remote.begin(); it != remote.end(); ++it) {
        it->active = false;
    }*/

    while (!granted) {
        try {
            tick2();
        }
        catch (RTI::Exception& e) {
            Debug(D, pdExcept) << "******** Exception ticking the RTI : " << &e << std::endl;
            throw;
        }
    }

    RTIfedTime next_step(localTime + TIME_STEP);

    /*local.erase();

    vector<Ball>::iterator it;

    for (it = remote.begin(); it != remote.end(); ++it) {
        if (it->ID != 0 && it->active && local.checkBallCollision(&(*it))) {
#ifdef X_DISPLAY_MISSING
            std::cout << "Collision between Ball <" << local.ID << "> and <" << it->ID << ">" << std::endl;
#endif
            sendInteraction(local.dx, local.dy, next_step, it->ID);
            // On prend la vitesse de l'autre sauf dans le cas ou
            // on avait deja la meme. Dans ce cas, on inverse la notre.
            if ((local.dx == it->dx) && (local.dy == it->dy)) {
                local.dx = -local.dx;
                local.dy = -local.dy;
            }
            else
                local.setDirection(it->dx, it->dy);
        }
        it->display();
        Debug(D, pdTrace) << "no collision." << std::endl;
    }

    // Teste la collision avec le bord
    Debug(D, pdTrace) << "Border collisions..." << std::endl;
    local.checkBorderCollision(XMAX, YMAX);*/

    


    Debug(D, pdTrace) << "End of step" << std::endl;
}

/*
 * This function resigns the object
 */
void HLA_Ptolemy::resign(std::string federate_name)
{
    unsigned long local_ID = rtiamb.getObjectInstanceHandle(federate_name.c_str());
    
    try {
        rtiamb.deleteObjectInstance(local_ID, localTime, "DO");
        Debug(D, pdRegister) << "Local object "<< federate_name <<" deleted from federation." << std::endl;
    }
    catch (RTI::Exception& e) {
        Debug(D, pdExcept) << "**** Exception delete object : " << &e << std::endl;
    }

    Debug(D, pdTerm) << "Local object "<< federate_name << " deleted." << std::endl;
}


// ----------------------------------------------------------------------------
/** resign the federation
 */
void HLA_Ptolemy::resign_federation()
{
    
    //Delete all registered objects */
    for (auto it = objects_declared.cbegin(); it != objects_declared.cend(); it++){
        std::string obj = *it;
        resign(obj);
    }
    

    setTimeRegulation(false, false);

    try {
        rtiamb.resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
        Debug(D, pdTerm) << "Just resigned from federation" << std::endl;
    }
    catch (RTI::Exception& e) {
        Debug(D, pdExcept) << "** Exception during resignFederationExecution by federate" << std::endl;
    }
    // Detruire la federation

    if (creator) {
        for (;;) {
            tick();
            try {
                Debug(D, pdTerm) << "Asking from federation destruction..." << std::endl;
                rtiamb.destroyFederationExecution(federationName.c_str());

                Debug(D, pdTerm) << "Federation destruction granted." << std::endl;
                break;
            }
            catch (RTI::FederatesCurrentlyJoined) {
                sleep(5);
            }
        }
    }
    Debug(D, pdTerm) << "Destroying RTIAmbassador and FedAmbassador." << std::endl;
    Debug(D, pdTerm) << "Federation terminated." << std::endl;
}


/* Returns a list of attributes for the specific ClassName */
std::list<std::string> HLA_Ptolemy::parser(std::string ClassName){
    
        char* pPath = getenv ("CERTI_FOM_PATH");
        if (pPath==NULL){
         //FIXME call gem5 panic   
        }
        
        std::string pPathString(pPath);
        std::string fullPathFedFile = pPathString + "/" + fed_file;        
    
        std::ifstream in(fullPathFedFile);
        
        std::string str;
        bool find_class = false;
        
        std::list<std::string> attributes;
        
        while (std::getline(in, str)) {
            
            /* Find the Class */
            if(!find_class){
                std::size_t found = str.find(ClassName);
                if (found!=std::string::npos){
                    find_class = true;     
                    continue;
                }
            }

            if(find_class){
                /* Find the bounds of class */
                std::size_t found1 = str.find("(Class ");
                std::size_t found2 = str.find("(Interactions");
                if ((found1!=std::string::npos) || (found2!=std::string::npos)){
                    find_class = false;
                    break;
                }
                
                /* Find the attributes */
                std::size_t found3 = str.find("(Attribute ");
                
                if (found3!=std::string::npos){
                    std::string delimiter = "(Attribute ";
                
                    size_t pos = 0;
                    std::string token;
                    while ((pos = str.find(delimiter)) != std::string::npos) {
                        token = str.substr(0, pos);
                        str.erase(0, pos + delimiter.length());
                    }
                    
                    std::string firstWord = str.substr(0, str.find(" "));
                    attributes.push_back(firstWord);
                }
            }
        }
        in.close();
        return attributes;
}


// ----------------------------------------------------------------------------
/** Carry out publications
 */
void HLA_Ptolemy::publish()
{
    // Get all class and attributes handles
    ActuatorClassID = rtiamb.getObjectClassHandle(CLA_ACTUATOR);
    std::list<std::string> attributes_name =  parser(CLA_ACTUATOR);
    
    int attr_number = attributes_name.size(); //number of attributes
    unique_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(attr_number));
    
    for (auto it = attributes_name.cbegin(); it != attributes_name.cend(); it++){
        std::string attr = *it;
        RTI::AttributeHandle AttrID = rtiamb.getAttributeHandle(attr.c_str(), ActuatorClassID);
        attributes->add(AttrID);
    }

    rtiamb.publishObjectClass(ActuatorClassID, *attributes);
}

// ----------------------------------------------------------------------------
/** Carry out subscriptions
 */
void HLA_Ptolemy::subscribe()
{
    // Get all class and attributes handles
    SensorClassID = rtiamb.getObjectClassHandle(CLA_SENSOR);
    std::list<std::string> attributes_name =  parser(CLA_SENSOR);
    
    int attr_number = attributes_name.size(); //number of attributes
    unique_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(attr_number));
    
    for (auto it = attributes_name.cbegin(); it != attributes_name.cend(); it++){
        std::string attr = *it;
        RTI::AttributeHandle AttrID = rtiamb.getAttributeHandle(attr.c_str(), SensorClassID);
        attributes->add(AttrID);
    }

    rtiamb.subscribeObjectClassAttributes(SensorClassID, *attributes, RTI::RTI_TRUE);
}



// ----------------------------------------------------------------------------
/*! Envoie une interaction, dont les parametres DX et DY ont pour valeur les
  dx et dy de la bille Local, et dont l'etiquette temporelle vaut
  InteractionTime.
*/
void HLA_Ptolemy::sendInteraction(double dx, double dy, const RTI::FedTime& InteractionTime, RTI::ObjectHandle id)
{
    /*libhla::MessageBuffer buffer;

    RTI::ParameterHandleValuePairSet* parameterSet = RTI::ParameterSetFactory::create(3);

    buffer.reset();
    buffer.write_int32(id);
    buffer.updateReservedBytes();
    parameterSet->add(ParamBoulID, static_cast<char*>(buffer(0)), buffer.size());

    Debug(D, pdDebug) << "SendInteraction" << std::endl;
    Debug(D, pdDebug) << "SendInteraction - ParamBoulID= " << ParamBoulID << std::endl;
    Debug(D, pdDebug) << "SendInteraction - x= " << id << std::endl;

    buffer.reset();
    buffer.write_double(dx);
    buffer.updateReservedBytes();
    parameterSet->add(ParamDXID, static_cast<char*>(buffer(0)), buffer.size());

    Debug(D, pdDebug) << "SendInteraction - ParamDXID= " << ParamDXID << ", x= " << dx << std::endl;

    buffer.reset();
    buffer.write_double(dy);
    buffer.updateReservedBytes();
    parameterSet->add(ParamDYID, static_cast<char*>(buffer(0)), buffer.size());

    Debug(D, pdDebug) << "SendInteraction - ParamDYID= " << ParamDYID << ", x= " << dy << std::endl;

    try {
        if (notimestamp) {
            Debug(D, pdRegister) << "Sending interaction without time (DX= " << dx << ", DY= " << dy << ")."
                                 << std::endl;
            rtiamb.sendInteraction(BingClassID, *parameterSet, "");
        }
        else {
            Debug(D, pdRegister) << "Sending interaction wit time (DX= " << dx << ", DY= " << dy << ")." << std::endl;
            rtiamb.sendInteraction(BingClassID, *parameterSet, InteractionTime, "");
        }
    }
    catch (RTI::Exception& e) {
        std::cout << "sendInteraction raise exception " << e._name << "(" << e._reason << ")" << std::endl;
        Debug(D, pdExcept) << "**** Exception sending interaction : " << &e << std::endl;
    }

    delete parameterSet;*/
}

// ----------------------------------------------------------------------------
/** Updates a ball by sending entity position and color.
    \param x X position
    \param y Y position
    \param color Color
    \param UpdateTime Event time
    \param id Object handle (ball)
 */
void HLA_Ptolemy::sendUpdate(std::string federate_name, std::list<ActuatorAttributes_t> attr_insert)// std::string attr1, double val1, std::string attr2, double val2)
{
    // APOLLON usage of hla bridge thus name=federation_name+federate_name (FIXED)
    // See https://savannah.nongnu.org/bugs/index.php?53641
    std::string ObjectInstanceName  = federationName + federate_name;
    
    // registers objects, regions, etc.
    declare(ObjectInstanceName);
    
    RTI::ObjectHandle local_ID = rtiamb.getObjectInstanceHandle(ObjectInstanceName.c_str());
    
    RTI::AttributeHandle attr;
    libhla::MessageBuffer buffer;
    RTI::AttributeHandleValuePairSet* attributeSet;
    
    int attr_number =  attr_insert.size();

    attributeSet = RTI::AttributeSetFactory::create(attr_number);

    for (auto it = attr_insert.cbegin(); it != attr_insert.cend(); it++){
        ActuatorAttributes_t obj = *it;
        buffer.reset();
        buffer.write_double(obj.value_double);
        buffer.updateReservedBytes();
        attr = rtiamb.getAttributeHandle(obj.attributeName.c_str(), ActuatorClassID);
        attributeSet->add(attr, static_cast<char*>(buffer(0)), buffer.size());
    }
    

    try {
            rtiamb.updateAttributeValues(local_ID, *attributeSet, localTime + TIME_STEP, "coucou");
    }
    catch (RTI::Exception& e) {
        std::cout << "Exception " << e._name << " (" << e._reason << ")" << std::endl;
        Debug(D, pdExcept) << "**** Exception updating attribute values: " << &e << std::endl;
    }

    delete attributeSet;
}



// ============================================================================
// FEDERATE AMBASSADOR CALLBACKS
// ============================================================================

// ----------------------------------------------------------------------------
/** Callback : discover object instance
 */
void HLA_Ptolemy::discoverObjectInstance(RTI::ObjectHandle theObject,
                                     RTI::ObjectClassHandle theObjectClass,
                                     const char* /*theObjectName*/) throw(RTI::CouldNotDiscover,
                                                                          RTI::ObjectClassNotKnown,
                                                                          RTI::FederateInternalError)
{
    if (theObjectClass != SensorClassID) {
        Debug(D, pdError) << "Object of Unknown Class discovered." << std::endl;
        std::string msg = stringize() << "Unknown objectClass < " << theObjectClass << ">";
        throw RTI::FederateInternalError(msg.c_str());
    }

    bool already = false;
    /*for (vector<Ball>::iterator i = remote.begin(); i != remote.end(); ++i) {
        if (i->ID == theObject) {
            cout << "Rediscovered object " << theObject << endl;
            already = true;
        }
    }*/
    if (!already) {
        cout << "Discovered object handle = " << theObject << ", name = " << rtiamb.getObjectInstanceName(theObject)
             << endl;
        //remote.push_back(Ball(theObject));
    }
}

// ----------------------------------------------------------------------------
/** Callback announce synchronization point
 */
void HLA_Ptolemy::announceSynchronizationPoint(const char* label, const char* /*tag*/) throw(RTI::FederateInternalError)
{
    if (strcmp(label, "Init") == 0) {
        paused = true;
        Debug(D, pdProtocol) << "announceSynchronizationPoint." << std::endl;
    }
#ifdef TEST_RFSP
    else if (strcmp(label, "Friend") == 0) {
        std::cout << "**** I am happy : I have a friend ****" << std::endl;
        paused = true;
        Debug(D, pdProtocol) << "announceSynchronizationPoint (friend)." << std::endl;
    }
#endif
    else {
        cout << "Unexpected synchronization label" << endl;
        exit(1);
    }
}

// ----------------------------------------------------------------------------
/** Callback : federation synchronized
 */
void HLA_Ptolemy::federationSynchronized(const char* label) throw(RTI::FederateInternalError)
{
    if (strcmp(label, "Init") == 0) {
        paused = false;
        Debug(D, pdProtocol) << "CALLBACK : federationSynchronized with label " << label << std::endl;
    }
}

// ----------------------------------------------------------------------------
/** Callback : receive interaction
 */
void HLA_Ptolemy::receiveInteraction(RTI::InteractionClassHandle theInteraction,
                                 const RTI::ParameterHandleValuePairSet& theParameters,
                                 const RTI::FedTime& /*theTime*/,
                                 const char* /*theTag*/,
                                 RTI::EventRetractionHandle /*theHandle*/) throw(RTI::InteractionClassNotKnown,
                                                                                 RTI::InteractionParameterNotKnown,
                                                                                 RTI::InvalidFederationTime,
                                                                                 RTI::FederateInternalError)
{
    /*libhla::MessageBuffer buffer;
    RTI::ULong valueLength;
    int dx1 = 0;
    int dy1 = 0;
    RTI::ObjectHandle h1 = 0;
    bool bille = false;

    Debug(D, pdTrace) << "Fed : receiveInteraction" << std::endl;
    if (theInteraction != BingClassID) {
        Debug(D, pdError) << "CALLBACK receiveInteraction : Unknown Interaction received" << std::endl;
        exit(-1);
    }

    Debug(D, pdDebug) << "receiveInteraction - nb attributs= " << theParameters.size() << std::endl;

    for (unsigned int j = 0; j < theParameters.size(); ++j) {
        RTI::ParameterHandle parmHandle = theParameters.getHandle(j);

        valueLength = theParameters.getValueLength(j);
        assert(valueLength > 0);
        buffer.resize(valueLength);
        buffer.reset();
        theParameters.getValue(j, static_cast<char*>(buffer(0)), valueLength);
        buffer.assumeSizeFromReservedBytes();

        if (parmHandle == ParamDXID) {
            dx1 = static_cast<int>(buffer.read_double());
        }
        else {
            if (parmHandle == ParamDYID) {
                dy1 = static_cast<int>(buffer.read_double());
            }

            else {
                if (parmHandle == ParamBoulID) {
                    h1 = buffer.read_int32();
                    ;
                    bille = true;
                }
                else {
                    Debug(D, pdError) << "Unrecognized parameter handle" << std::endl;
                }
            }
        }
    }
    if (bille) {
        if (h1 == local.ID) {
            local.dx = dx1;
            local.dy = dy1;
        }
    }*/
}

// ----------------------------------------------------------------------------
/** Callback : reflect attribute values with time
 */
void HLA_Ptolemy::reflectAttributeValues(RTI::ObjectHandle theObject,
                                     const RTI::AttributeHandleValuePairSet& theAttributes,
                                     const RTI::FedTime& /*theTime*/,
                                     const char* /*theTag*/,
                                     RTI::EventRetractionHandle /*theHandle*/) throw(RTI::ObjectNotKnown,
                                                                                     RTI::AttributeNotKnown,
                                                                                     RTI::InvalidFederationTime,
                                                                                     RTI::FederateInternalError)
{
    Debug(D, pdTrace) << "reflectAttributeValues with time" << std::endl;

    libhla::MessageBuffer buffer;

    RTI::ULong valueLength;
    
    /* Ptolemy sends one by one the attributes */
    if(theAttributes.size() == 1){
        RTI::AttributeHandle parmHandle = theAttributes.getHandle(0);
        valueLength = theAttributes.getValueLength(0);
        assert(valueLength > 0);
        buffer.resize(valueLength);
        buffer.reset();
        theAttributes.getValue(0, static_cast<char*>(buffer(0)), valueLength);
        buffer.assumeSizeFromReservedBytes();
        
        try {
            std::string ReceivedInstanceName(rtiamb.getObjectInstanceName(theObject));
            std::string ReceivedAttributeName(rtiamb.getAttributeName(parmHandle, SensorClassID));
            double Receivedval = buffer.read_double();
            
            std::list <SensorAttributes_t>::iterator iObject;
            
            for (iObject= sensor_attributes.begin(); iObject != sensor_attributes.end(); ++iObject){
                
                SensorAttributes_t attr   = *iObject;
                std::string instanceName  = attr.instanceName;
                std::string attributeName = attr.attributeName;
                
                //Attribute has already received in the past. So, update only the value!
                if((ReceivedInstanceName == instanceName) && (ReceivedAttributeName == attributeName)){ 
                    attr.value_double = Receivedval;
                    *iObject = attr;
                    return;
                }
            }
            
            //Attribute has not received in the past. So, insert a new list entry!
            SensorAttributes_t new_attr;
            new_attr.instanceName  = ReceivedInstanceName;
            new_attr.attributeName = ReceivedAttributeName;
            new_attr.value_double  = Receivedval;
            sensor_attributes.push_back(new_attr);
        }
        catch (RTI::ObjectNotKnown& e) {
            return;
        }
    }
}


std::list<SensorAttributes_t> HLA_Ptolemy::GetSensorAttributes(){
    return sensor_attributes;
}

// ----------------------------------------------------------------------------
/** Callback : reflect attribute values without time
 */
void HLA_Ptolemy::reflectAttributeValues(RTI::ObjectHandle theObject,
                                     const RTI::AttributeHandleValuePairSet& theAttributes,
                                     const char* /*theTag*/) throw(RTI::ObjectNotKnown,
                                                                   RTI::AttributeNotKnown,
                                                                   RTI::FederateInternalError)
{
    /*Debug(D, pdTrace) << "reflectAttributeValues without time" << std::endl;

    float x1 = 0;
    float y1 = 0;
    libhla::MessageBuffer buffer;

    RTI::ULong valueLength;

    Debug(D, pdDebug) << "reflectAttributeValues - nb attributs= " << theAttributes.size() << std::endl;

    for (unsigned int j = 0; j < theAttributes.size(); j++) {
        RTI::AttributeHandle parmHandle = theAttributes.getHandle(j);
        valueLength = theAttributes.getValueLength(j);
        assert(valueLength > 0);
        buffer.resize(valueLength);
        buffer.reset();
        theAttributes.getValue(j, static_cast<char*>(buffer(0)), valueLength);
        buffer.assumeSizeFromReservedBytes();

        if (parmHandle == AttrXID) {
            x1 = buffer.read_double();
        }
        else if (parmHandle == AttrYID) {
            y1 = buffer.read_double();
        }
        else {
            Debug(D, pdError) << "Fed: ERREUR: handle inconnu." << std::endl;
        }
    }

    vector<Ball>::iterator it = remote.begin();
    while (it != remote.end() && it->ID != theObject)
        ++it;

    if (it == remote.end()) {
        Debug(D, pdError) << "Fed: error, id not found: " << theObject << std::endl;
    }
    else {
        it->erase();

        float oldx = it->x;
        it->x = x1;
        it->dx = it->x - oldx;

        float oldy = it->y;
        it->y = y1;
        it->dy = it->y - oldy;

        it->active = true;
    }*/
}
// ----------------------------------------------------------------------------
/** Callback : remove object instance
 */
void HLA_Ptolemy::removeObjectInstance(RTI::ObjectHandle theObject,
                                   const RTI::FedTime&,
                                   const char*,
                                   RTI::EventRetractionHandle) throw(RTI::ObjectNotKnown,
                                                                     RTI::InvalidFederationTime,
                                                                     RTI::FederateInternalError)
{
    /*vector<Ball>::iterator it;

    for (it = remote.begin(); it != remote.end(); ++it) {
        if (it->ID == theObject) {
            it->erase();
            remote.erase(it);
            return;
        }
    }*/
}

// ----------------------------------------------------------------------------
/** Callback : time advance granted
 */
void HLA_Ptolemy::timeAdvanceGrant(const RTI::FedTime& theTime) throw(RTI::InvalidFederationTime,
                                                                  RTI::TimeAdvanceWasNotInProgress,
                                                                  RTI::FederateInternalError)
{
    granted = true;
    localTime = theTime;
    Debug(D, pdTrace) << "Time advanced, local time is now " << localTime.getTime() << std::endl;
}


//! --- CERTI INITIALIZATION IP --- !//
bool 
HLA_Ptolemy::RequestFunction(HLAInitializationRequest rqst){
  int sockfd = 0, n = 0;
  bool ret = false;
  
  struct sockaddr_in serv_addr; 

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Error : Could not create socket \n");
      exit(-1);
  } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(60403); 

  char* pPath = getenv ("CERTI_HOST");
  if (pPath==NULL)
    pPath = (char *) "127.0.0.1";
  
  if(inet_pton(AF_INET, pPath, &serv_addr.sin_addr)<=0)
  {
      printf("\n inet_pton error occured\n");
      exit(-1);
  }

  if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
     if(!FirstConnectionWithHLAInitialization){
       printf("\n Error : Connection with HLA Initialization Server (IP:%s) Failed \n",pPath);
       exit(-1);
     }
     else{
       printf("\n Warning : Connection with HLA Initialization Server (IP:%s) Failed.. Retry in 30 secs\n",pPath);
       sleep(30);
       return RequestFunction(rqst);
     }
  }
  else{
    FirstConnectionWithHLAInitialization = true;
  }
    
  std::string rqst_type;
  std::string rqst_name(rqst.name);
  std::string rqst_node = std::to_string(rqst.node);
  
  if(rqst.type == CREATE)
      rqst_type = "CREATE";	
  else if(rqst.type == REMOVE)
      rqst_type = "REMOVE";
  else if(rqst.type == WRITE)
      rqst_type = "WRITE";
  else if(rqst.type == READ)
      rqst_type = "READ";
  else if(rqst.type == CLOSE_SERVER)
      rqst_type = "CLOSE_SERVER";
  else{
      printf("\n Unknown Request Type \n");
      exit(-1);
  }
  
  std::string rqst_str = rqst_type + "|" + rqst_name + "|" + rqst_node;
  int lenght = rqst_str.length(); 
  char rqst_char_array[lenght + 1]; 
  strcpy(rqst_char_array, rqst_str.c_str()); 
  
  n = write(sockfd, rqst_char_array, sizeof(rqst_char_array)); 
  
  char resp_char[5] = {0};
  n = read(sockfd, (void *) &resp_char, sizeof(resp_char));
  if(n < 0)
    printf("\n Reply error \n");
  
  std::string resp_str(resp_char);
  if(resp_str == "true")
      ret = true;
  else
      ret = false;
  
  close(sockfd);
  //! Set the appropriate delay if the server is in localhost or not !//
  if((strcmp(pPath,(char *)"127.0.0.1")!=0)&&((rqst.type == READ)||(rqst.type == READ_GLOBAL))){
    usleep(200000);
  }
  else{
    usleep(10000);
  }
  return ret;
}
//! --- END CERTI INITIALIZATION IP --- !//
