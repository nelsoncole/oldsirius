/*
 * Khole Operating System
 * ata/ahci.h
 * 
 * Copyright (C) 2017,2018 Nelson Cole <nelsoncole72@gmail.com>
 */
#ifndef __EHCI_H__
#define __EHCI_H__
#include <typedef.h>




// 0x8086 0x24DD (USB EHCI—D29:F7) ICH5 


typedef struct ehcipci
{
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t cmd;
    uint16_t sts;
    uint8_t  revision_id;
    uint8_t  progif;
    uint8_t  scc;
    uint8_t  bcc;
    uint8_t  primary_master_latency_timer; 
    uint32_t mem_base;
    uint16_t intline;
    uint16_t intpin;



}__attribute__((packed)) ehcipci_t;

typedef volatile struct EHCI_PORT
{
    uint32_t ccsts:1;       // Current Connect Status
    uint32_t cstsc:1;       // Connect Status Change
    uint32_t ped:1;         // Port Enabled/Disabled
    uint32_t pdc:1;         // Port Enable/Disable Change 
    uint32_t overa:1;       // Overcurrent Active
    uint32_t overc:1;       // Overcurrent Change 
    uint32_t fpresume:1;    // Force Port Resume
    uint32_t Suspend:1;     // Suspend 
    uint32_t preset:1;      // Port Reset
    uint32_t rsv0:1;
    uint32_t lsts:2;        // Line Status
    uint32_t pp:1;          // Port Power
    uint32_t po:1;          // Port Owner 
    uint32_t rsv1:2;
    uint32_t ptc:4;         // Port Test Control 
    uint32_t wce:1;         // Wake on Connect Enable (WKCNNT_E)
    uint32_t wde:1;         // Wake on Disconnect Enable (WKDSCNNT_E)
    uint32_t woe:1;         // Wake on Overcurrent Enable (WKOC_E)
    uint32_t rsv2:9;

}__attribute__((packed)) EHCI_PORT_T;
typedef volatile struct EHCI_MEM
{
    uint16_t caplength;     // Capabilities Registers Length
    uint16_t hcivrsion;     // Host Controller Interface Version Number
    uint32_t hcsparams;     // Host Controller Structural Parameters, value default(00103206h(D29:F7), 00102205(D26:F7))
    uint32_t hccparams;     // Host Controller Capability Parameters
    uint8_t  rsv0[0x20-0x0C];
    // +offset 20h
    uint32_t cmd;           // USB2.0_CMD USB 2.0 Command 
    uint32_t sts;           // USB2.0_STS USB 2.0 Status
    uint32_t intr;          // USB 2.0_INTR USB 2.0 Interrupt Enable 
    uint32_t frindex;       // USB 2.0 Frame Index
    uint32_t ctrldssegment; // Control Data Structure Segment 
    uint32_t perodiclistbase; // Period Frame List Base Address 
    uint32_t asynclistaddr; // Current Asynchronous List Address 
    uint8_t  rsv1[0x60-0x3C]; // Reserved
    uint32_t configflag;    // Configure Flag
    EHCI_PORT_T port[6];    // Port x Status and Control TODO Port6 74h–77h (D29 Only), Port7 78h–7Bh (D29 Only)


    // Debug Port Register Address Map
    uint8_t  rsv2[0xA0-0x7C];
    uint32_t ctrlsts;       // Control/Status 
    uint32_t pid;           // USB PIDs
    uint32_t datal;       // Data Buffer (Bytes 7:0)
    uint32_t datah;
    uint32_t config;        // config   
    uint8_t  rsv3[0x400-0xB4]; 
}__attribute__((packed)) EHCI_MEM_T;







#endif
