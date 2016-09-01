


#include "moCapComm.h"
#include <stdio.h>



//==========================================================================
//コンストラクタ
//==========================================================================
moCapComm::moCapComm(){
	
	//if(theClient)
 //   {
 //       theClient->Uninitialize();
 //       delete theClient;
 //   }

    // create NatNet client
	//theClient->Uninitialize();
	//delete theClient;

    theClient = new NatNetClient(0);

	ZeroMemory(&m_receiveState, sizeof(m_receiveState));
	ZeroMemory(&m_receiveState_CB, sizeof(m_receiveState_CB));
	ZeroMemory(&receiveState_CB, sizeof(receiveState_CB));
	ZeroMemory(&m_initState, sizeof(m_initState));


}

//==========================================================================
//デストラクタ
//==========================================================================
moCapComm::~moCapComm(){

	// Done - clean up.
	if(theClient){theClient->Uninitialize();}

}



int moCapComm::InitializeAsClient (char* myIPAddress, char* serverIPAddress, unsigned int myServersCommandPort, unsigned int myServersDataPort){

    // Init Client and connect to NatNet server
    // to use NatNet default port assigments
	int returnCode = theClient->Initialize(myIPAddress, serverIPAddress, myServersCommandPort, myServersDataPort);
	if (returnCode == 0){

        printf("Initialize succeed!!");

    }
    else
    {
        printf("Initialize Error");
    }

	theClient->SetDataCallback( DataHandler, theClient );	// this function will receive data from the server


	return returnCode;

}

bool moCapComm::InitializePosition (){

	Sleep(100);
	m_initState = receiveState_CB;

	if( abs(receiveState_CB.x - m_initState.x)<1.0 &&
		abs(receiveState_CB.y - m_initState.y)<1.0 &&
		abs(receiveState_CB.z - m_initState.z)<1.0){
			return true;
	}
	else{return false;}


}


State moCapComm::getState_CB(){

	//InitializePosition()
	//	していればm_initStateに初期位置の値が入っている
	//	していなければm_initStateは0のまま

	m_receiveState_CB.x = receiveState_CB.x - m_initState.x; 
	m_receiveState_CB.y = receiveState_CB.y - m_initState.y;
	m_receiveState_CB.z = receiveState_CB.z - m_initState.z;

	m_receiveState_CB.roll	= receiveState_CB.roll	 - m_initState.roll;
	m_receiveState_CB.pitch= receiveState_CB.pitch - m_initState.pitch;
	m_receiveState_CB.yaw	= receiveState_CB.yaw	 - m_initState.yaw;

	return m_receiveState_CB;
	
}

bool moCapComm::ReceiveData (){

	
	data = theClient->GetLastFrameOfData();

	if(data==NULL){return false;}

	m_receiveState.x = data->RigidBodies[0].z*1000.0;
	m_receiveState.y = data->RigidBodies[0].x*1000.0;
	m_receiveState.z = data->RigidBodies[0].y*1000.0;
	
	//GetEulerAngles(data->RigidBodies[0].qx,
	//				data->RigidBodies[0].qy,
	//				data->RigidBodies[0].qz,
	//				data->RigidBodies[0].qw,
	//				data[5],
	//				data[4],
	//				data[3]);



	return true;
	
}

State moCapComm::getState(){

	m_receiveState.x = m_receiveState.x - m_initState.x; 
	m_receiveState.y = m_receiveState.y - m_initState.y;
	m_receiveState.z = m_receiveState.z - m_initState.z;

	m_receiveState.roll	= m_receiveState.roll	- m_initState.roll;
	m_receiveState.pitch	= m_receiveState.pitch - m_initState.pitch;
	m_receiveState.yaw		= m_receiveState.yaw	- m_initState.yaw;

	return 	m_receiveState; 
	
}


void moCapComm::CloseSocket (){

	theClient->Uninitialize();

}



// DataHandler receives data from the server
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
	NatNetClient* pClient = (NatNetClient*) pUserData;
	
	// Rigid Bodies
	for(int i=0; i < (data->nRigidBodies); i++)
	{
		//printf("Rigid Body [ID=%d  Error=%3.2f]\n", data->RigidBodies[i].ID, data->RigidBodies[i].MeanError);
		//printf("\tx\ty\tz\tqx\tqy\tqz\tqw\n");
		//printf("\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",


		receiveState_CB.x = data->RigidBodies[i].z*1000.0;
		receiveState_CB.y = data->RigidBodies[i].x*1000.0;
		receiveState_CB.z = data->RigidBodies[i].y*1000.0;

		GetEulerAngles((double)(data->RigidBodies[i].qx),
						(double)(data->RigidBodies[i].qy),
						(double)(data->RigidBodies[i].qz),
						(double)(data->RigidBodies[i].qw),
						receiveState_CB.pitch, receiveState_CB.roll, receiveState_CB.yaw);

		}


}


//http://ja.softuses.com/137514

void GetEulerAngles(double qx, double qy, double qz, double qw, double& pitch, double& roll, double& yaw)
{
    const double w2 = qw*qw;
    const double x2 = qx*qx;
    const double y2 = qy*qy;
    const double z2 = qz*qz;
    const double unitLength = w2 + x2 + y2 + z2;    // Normalised == 1, otherwise correction divisor.
    const double abcd = qw*qx + qy*qz;
    const double eps = 1e-7;					// TODO: pick from your math lib instead of hardcoding.
    const double pi = 3.14159265358979323846;   // TODO: pick from your math lib instead of hardcoding.

	if (abcd > (0.5-eps)*unitLength)
    {
        pitch = 2 * atan2(qy, qw);
        roll = pi;
        yaw = 0;
    }
    else if (abcd < (-0.5+eps)*unitLength)
    {
        pitch = -2 * ::atan2(qy, qw);
        roll = -pi;
        yaw = 0;
    }
    else
    {
        const double adbc = qw*qz - qx*qy;
        const double acbd = qw*qy - qx*qz;
        pitch = ::atan2(2*adbc, 1 - 2*(z2+x2));
        roll = ::asin(2*abcd/unitLength);
        yaw = ::atan2(2*acbd, 1 - 2*(y2+x2));
    }
}


















