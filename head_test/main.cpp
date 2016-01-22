/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "Camera.h"
#include "mjpg_streamer.h"
#include "LinuxDARwIn.h"

#define INI_FILE_PATH       "config.ini"

#define U2D_DEV_NAME        "/dev/ttyUSB0"

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

int main(void)
{
    printf( "\n===== Head tracking Tutorial for DARwIn =====\n\n");

    change_current_dir();

    minIni* ini = new minIni(INI_FILE_PATH);

         //////////////////// Framework Initialize ////////////////////////////
        LinuxCM730 linux_cm730(U2D_DEV_NAME);
        CM730 cm730(&linux_cm730);
        if(MotionManager::GetInstance()->Initialize(&cm730) == false)
        {
                printf("Fail to initialize Motion Manager!\n");
                        return 0;
        }
    MotionManager::GetInstance()->LoadINISettings(ini);
        MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());
    LinuxMotionTimer *motion_timer = new LinuxMotionTimer(MotionManager::GetInstance());
    motion_timer->Start();

        MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(false);
        MotionManager::GetInstance()->SetEnable(true);
        /////////////////////////////////////////////////////////////////////

        Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);

        Head::GetInstance()->m_Joint.SetPGain(JointData::ID_HEAD_PAN, 8);
        Head::GetInstance()->m_Joint.SetPGain(JointData::ID_HEAD_TILT, 8);
	while(1)
	{
		Head::GetInstance()->MoveByAngle(70, 30);
		sleep(1);
		Head::GetInstance()->MoveByAngle(-70, -30);
		sleep(1);
	}
}
