/*
//-------------------------------------
Summary		: Class for NatNet on C++
Date		: 4/10/2014
Requirement	: NatNet 2.2.0.0
			�@ws2_32.lib�CNatNetLibStatic.lib
Caution		: getState_CB()�Ńf�[�^���Q�b�g����ꍇ��
				�R�[���o�b�N�֐��Ńf�[�^���󂯎���Ă���̂ŁC
				�I�u�W�F�N�g��2�ȏ���ꍇ�͋C�����Ă�������
			�@ReceiveData()��getState()���g���ꍇ��
				�I�u�W�F�N�g�̖��͂Ȃ��ł����C
				����(20130510)�f�[�^��0�����A���Ă��Ȃ��̂Ŏg���܂���D
			�@���[�V�����L���v�`�`���̍��W�n�Ƃ̈Ⴂ�ɒ���
				����(20130510)�͎������̓��������ėp��������x�C�������z�Ƃ��Ă��܂��D
			  
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
} State;	//���̈ʒu�p��
#endif

void GetEulerAngles(double qx, double qy, double qz, double qw,
						double& pitch, double& roll, double& yaw);
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);

static State receiveState_CB;


class moCapComm {
public:

	moCapComm();	//�R���X�g���N�^
	~moCapComm();	//�f�X�g���N�^

	//Server�Ƃ̒ʐM�̊J�n
	//�Ԃ�l�̓G���[�R�[�h�C0�Ȃ�success
	int InitializeAsClient (char* myIPAddress, char* serverIPAddress, unsigned int myServersCommandPort, unsigned int myServersDataPort);
	
	//position�̏�����
	//True�Ȃ琬���CFalse�Ŏ��s
	bool InitializePosition();

	//�ŐV�f�[�^�̎擾
	//�Ԃ�l��true�Ȃ�擾�����Cfalse�Ȃ玸�s
	bool ReceiveData ();

	//Server�Ƃ̒ʐM�̏I��
	void CloseSocket ();

	//ReceiveData�֐��Ŏ擾�����f�[�^��get�֐�
	State getState();

	//�R�[���o�b�N�֐��Ŏ擾�����f�[�^��get�֐�
	State getState_CB();	


private:

	NatNetClient* theClient;
	sFrameOfMocapData* data;

	State m_receiveState;
	State m_receiveState_CB;
	State m_initState;

};


#endif 











