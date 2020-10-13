/*
 * Copyright (c) 2014-2016 IBM Corporation.
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "lmic.h"
#include "debug.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <stdint.h>

//////////////////////////////////////////////////
// CONFIGURATION (FOR APPLICATION CALLBACKS BELOW)
//////////////////////////////////////////////////

// application router ID (LSBF)
static const u1_t APPEUI[8]  = { 0xDE, 0x46, 0x03, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// unique device ID (LSBF)
static const u1_t DEVEUI[8]  = { 0x5D, 0x4B, 0x65, 0xE3, 0x8B, 0x31, 0xCB, 0x00 };

// device-specific AES key (derived from device EUI)
static const u1_t DEVKEY[16] = { 0x89, 0xCA, 0x26, 0x18, 0xDB, 0x51, 0xE4, 0x3B, 0xA0, 0x56, 0x5B, 0xE4, 0xCE, 0x96, 0xB7, 0x15 };

static char mydata[32] = "05 10 10 2012 08 12 10 12";
static osjob_t sendjob;

//////////////////////////////////////////////////
// APPLICATION CALLBACKS
//////////////////////////////////////////////////

// provide application router ID (8 bytes, LSBF)
void os_getArtEui (u1_t* buf) {
    memcpy(buf, APPEUI, 8);
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui (u1_t* buf) {
    memcpy(buf, DEVEUI, 8);
}

// provide device key (16 bytes)
void os_getDevKey (u1_t* buf) {
    memcpy(buf, DEVKEY, 16);
}


//////////////////////////////////////////////////
// MAIN - INITIALIZATION AND STARTUP
//////////////////////////////////////////////////

// initial job
static void initfunc (osjob_t* j) {
    // reset MAC state
    LMIC_reset();
    printf("s%",DEVEUI);
    // start joining
    LMIC_startJoining();
    // init done - onEvent() callback will be invoked...
}


// application entry point
int main () {


    osjob_t initjob;
    // initialize runtime env
    os_init();
    // initialize debug library
    debug_init();
    // setup initial job
    os_setCallback(&initjob, initfunc);
    // execute scheduled jobs and events
    os_runloop();
    // (not reached)
    return 0;
}

void getData(){
	FILE *fptr1, *fptr2;
	int linectr = 1;

	char str[256];
	char temp[]="temp.txt";
	fptr1 = fopen("dataFile.txt","r+");

	if(!fptr1)
	{
		printf("Unable to open data file!!\n");
		return ;
	}

	fptr2 = fopen(temp,"w");
	if (!fptr2)
	{
		printf("Unable to open a temporary file to write!!");
		fclose(fptr1);
		return ;
	}

	if( fgets (mydata, 60, fptr1)!=NULL )
	{
		//printf("Reading data from data file\n");
		//puts(data);
	}

	while (!feof(fptr1))
	{
		strcpy(str, "\0");
		fgets(str, 256, fptr1);
		if (!feof(fptr1))
		{
			linectr++;
			fprintf(fptr2, "%s", str);
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove("dataFile.txt");
	rename(temp,"dataFile.txt");
	//printf("Removed the first line\n");
	//sleep(5);
	//printf("Sleep test\n");
	return ;
}




//////////////////////////////////////////////////
// UTILITY JOB
//////////////////////////////////////////////////

static osjob_t blinkjob;
static u1_t ledstate = 0;

static void blinkfunc (osjob_t* j) {
    // toggle LED
    ledstate = !ledstate;
    debug_led(ledstate);
    // reschedule blink job
    os_setTimedCallback(j, os_getTime()+ms2osticks(100), blinkfunc);
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        debug_str("OP_TXRXPEND, not sending\r\n");
    } else {
		//getData();
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        debug_str("Packet queued\r\n");
    }
    os_setTimedCallback(j, os_getTime()+sec2osticks(10), do_send);
}

//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////

void onEvent (ev_t ev) {
    debug_event(ev);

    switch(ev) {
      // starting to join network
      case EV_JOINING:
          // start blinking
          blinkfunc(&blinkjob);
          break;
          
      // network joined, session established
      case EV_JOINED:
          // cancel blink job
          os_clearCallback(&blinkjob);
          // switch on LED
          debug_led(1);
          // (don't schedule any new actions)
	  do_send(&sendjob);
          break;
    }
}
