/*
 * Copyright (c) 2010 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Tampouratzis Nikolaos
 */


/** @file
 * Implementiation of Apollon Device
 */

#ifndef __DEV_APOLLON_DEVICE_HH__
#define __DEV_APOLLON_DEVICE_HH__


#include <string>

#include "HLA_Ptolemy.hh"

#include "dev/arm/amba_device.hh"
#include "mem/packet.hh"
#include "params/ApollonDevice.hh"

#include "dev/arm/ApollonStructs.h"

#include "sim/eventq.hh"

#define DEVICE_MEMORY_ENTRY_SIZE 65536
#define DELAY_PER_DMA_MEMORY_ENTRY 0


// Registers used by Device
#define DMA_TRANSFER_ACTUATOR_TO_DEVICE   0x00000
#define DMA_TRANSFER_SENSOR_TO_DEVICE     0x00008
#define DMA_TRANSFER_SENSOR_FROM_DEVICE   0x00010
#define GEM_CURRENT_TICK                  0x00018

class ApollonDevice : public AmbaDmaDevice
{
    
    HLA_Ptolemy * hla_ptolemy;
    
  protected:
    int nodeNumber;
    int TotalNodes;
    
    uint8_t * MemSimToDevActuator;
    uint8_t * MemSimToDevSensor;
    uint8_t * MemDevToSimSensor;
    
    PtolemyActuator_t *   PtolemyActuator;
    PtolemySensorAttr_t * PtolemySensorAttr;
    PtolemySensorData_t * PtolemySensorData;
    
    
    uint32_t bus_addr_sensor_to_device;
    uint32_t bus_addr_sensor_from_device;
    
    double PtolemySynchTimeTicks; 	//! PtolemySynchTimeTicks are the ticks in which the gem5 node will be synchronized with the Ptolemy counterpart node!//
    
      
  public:
      
    void Synch(); //! Global Synchronization Function !//
    typedef EventWrapper<ApollonDevice, &ApollonDevice::Synch> SynchEvent;
    friend void SynchEvent::process();
    SynchEvent synchEvent;
      
    void DMARcvPktActuatorComplete();
    EventWrapper<ApollonDevice, &ApollonDevice::DMARcvPktActuatorComplete> DMARcvPktActuatorEvent;
    
    void DMARcvPktSensorComplete();
    EventWrapper<ApollonDevice, &ApollonDevice::DMARcvPktSensorComplete> DMARcvPktSensorEvent;
    
    void DMASendPktSensorComplete();
    EventWrapper<ApollonDevice, &ApollonDevice::DMASendPktSensorComplete> DMASendPktSensorEvent;
    
    
    typedef ApollonDeviceParams Params;
    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }

    ApollonDevice(const Params *p);
    

    virtual Tick read(PacketPtr pkt);
    virtual Tick write(PacketPtr pkt);
    
    
    /**
     * Determine the address ranges that this device responds to.
     *
     * @return a list of non-overlapping address ranges
     */
    AddrRangeList getAddrRanges() const;
    
    void TimeConversion(const Params *p);
    
    void closeHLA();

};


#endif // __DEV_ARM_SENSOR_DEVICE_HH__
