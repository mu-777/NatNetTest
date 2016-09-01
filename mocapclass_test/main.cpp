

#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <math.h>

#include "moCapComm.h"

#ifndef RAD2DEG
#define RAD2DEG 57.29577951308232876846364344191
#endif

#define MY_IPADDRESS		"192.168.0.118"
#define SERVER_IPADDRESS	"192.168.0.4"
#define MY_SERVERCOMMANDPORT 1510
#define MY_SERVERDATAPORT	 1511


void main(){
	
	moCapComm myReceive;
	State receiveState;
	State receiveState_CB;	

	//initialize connection
	int isInit=0;
	isInit = myReceive.InitializeAsClient(MY_IPADDRESS, SERVER_IPADDRESS, MY_SERVERCOMMANDPORT,MY_SERVERDATAPORT);
	while(isInit != 0){	printf("\r init ERROR : %i\n",isInit); }

	//initialize position
	while(!myReceive.InitializePosition()){	printf("\r init position ERROR \n"); }

	//main loop
	while(1){

	////getState()�̏ꍇ
	//	if(myReceive.ReceiveData()){
	//		
	//		receiveState = myReceive.getState();
	//		
	//		printf("\n receiveState \n");
	//		std::cout<<"x:"<<receiveState.x
	//					<<"	y:"<<receiveState.y
	//					<<"	z:"<<receiveState.z<<std::endl;

	//		std::cout<<"roll:"<<receiveState.roll*RAD2DEG2
	//					<<"	pitch:"<<receiveState.pitch*RAD2DEG2
	//					<<"	yaw:"<<receiveState.yaw*RAD2DEG2<<std::endl;

	//	}
	//	else{
	//		std::cout<<"ERROR"<<std::endl;
	//	}


	//getState_CB()�̏ꍇ
		receiveState_CB = myReceive.getState_CB();

		printf("\n receiveState_CB \n");
		std::cout<<"x:"<<receiveState_CB.x
					<<"	y:"<<receiveState_CB.y
					<<"	z:"<<receiveState_CB.z<<std::endl;

		std::cout<<"roll:"<<receiveState_CB.roll*RAD2DEG
					<<"	pitch:"<<receiveState_CB.pitch*RAD2DEG
					<<"	yaw:"<<receiveState_CB.yaw*RAD2DEG<<std::endl;


	}

	

}
