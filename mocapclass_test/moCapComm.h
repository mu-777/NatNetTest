/*
//-------------------------------------
Summary		: Class for NatNet on C++
Date		: 4/10/2014
Requirement	: NatNet 2.2.0.0
			　ws2_32.lib，NatNetLibStatic.lib
Caution		: getState_CB()でデータをゲットする場合は
				コールバック関数でデータを受け取っているので，
				オブジェクトを2個以上作る場合は気をつけてください
			　ReceiveData()とgetState()を使う場合は
				オブジェクトの問題はないですが，
				現在(20130510)データが0しか帰ってこないので使えません．
			　モーションキャプチチャの座標系との違いに注意
				現在(20130510)は実験室の入り口から汎用口方向をx，上方向をzとしています．
			  
//-------------------------------------
*/
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <math.h>

#include "NatNetTypes.h"
#include "NatNetClient.h"


#ifndef __MOCAPCOMM_H__
#define __MOCAPCOMM_H__


//-------------------------
#ifndef State
typedef struct{
		double x;	//[mm]
		double y;	//[mm]
		double z;	//[mm]
		double roll;	//[rad]
		double pitch;	//[rad]
		double yaw;		//[rad]
} State;	//手先の位置姿勢
#endif

void GetEulerAngles(double qx, double qy, double qz, double qw,
						double& pitch, double& roll, double& yaw);
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);

static State receiveState_CB;


class moCapComm {
public:

	moCapComm();	//コンストラクタ
	~moCapComm();	//デストラクタ

	//Serverとの通信の開始
	//返り値はエラーコード，0ならsuccess
	int InitializeAsClient (char* myIPAddress, char* serverIPAddress, unsigned int myServersCommandPort, unsigned int myServersDataPort);
	
	//positionの初期化
	//Trueなら成功，Falseで失敗
	bool InitializePosition();

	//最新データの取得
	//返り値はtrueなら取得成功，falseなら失敗
	bool ReceiveData ();

	//Serverとの通信の終了
	void CloseSocket ();

	//ReceiveData関数で取得したデータのget関数
	State getState();

	//コールバック関数で取得したデータのget関数
	State getState_CB();	


private:

	NatNetClient* theClient;
	sFrameOfMocapData* data;

	State m_receiveState;
	State m_receiveState_CB;
	State m_initState;

};


#endif 











