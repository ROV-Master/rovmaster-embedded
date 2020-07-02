/**
 * @desc: PID控制器
 */

#define LOG_TAG "PID"

#include "PID.h"
#include <elog.h>
#include <math.h>

AllControler Total_Controller; //总控制器PID

/*
1偏差限幅标志；  2积分限幅标志；3积分分离标志；   4期望；
5反馈            6偏差；        7上次偏差；       8偏差限幅值；
9积分分离偏差值；10积分值       11积分限幅值；    12控制参数Kp；
13控制参数Ki；   14控制参数Kd； 15控制器总输出；  16上次控制器总输出
17总输出限幅度； 18变积分控制时的积分增益
*/
const float Control_Unit[18][20] = {
    /*                                     Kp     Ki     Kd                     */
    /*1 2  3  4  5  6   7  8   9 10   11   12     13     14  15 16   17  18*/
    {0, 1, 0, 0, 0, 0, 0, 500, 0, 0, 200, 0.50, 2.5000, 1.80, 0, 0, 5000, 1, 1, 1}, //Roll_Gyro;横滚角速度   1
    {0, 1, 0, 0, 0, 0, 0, 500, 0, 0, 200, 0.75, 3.5000, 1.80, 0, 0, 5000, 1, 1, 1}, //Pitch_Gyro;俯仰角速度  2
    {0, 1, 0, 0, 0, 0, 0, 250, 0, 0, 100, 1.00, 0.5000, 0.00, 0, 0, 3000, 1, 1, 1}, //Yaw_Gyro;偏航角速度    3

    {0, 1, 0, 0, 0, 0, 0, 30, 0, 0, 80, 4.00, 0.0000, 0.00, 0, 0, 3000, 1, 1, 1},  //Pitch_Angle;俯仰角度    4
    {0, 1, 0, 0, 0, 0, 0, 45, 0, 0, 150, 5.00, 0.0000, 0.00, 0, 0, 3000, 1, 1, 1}, //Yaw_Angle;偏航角		 5
    {0, 1, 0, 0, 0, 0, 0, 30, 0, 0, 80, 4.00, 0.0000, 0.00, 0, 0, 3000, 1, 1, 1},  //Roll_Angle;横滚角       6

    /*                                     Kp    Ki   Kd                     */
    /*1 2  3  4  5  6   7  8   9 10   11   12    13   14  15 16   17  18*/
    {0, 1, 0, 0, 0, 0, 0, 400, 0, 0, 500, 5.0, 0.300, 0.1, 0, 0, 5000, 1, 1, 1}, //High_Speed;海拔攀升速度   7
    {0, 1, 0, 0, 0, 0, 0, 200, 0, 0, 100, 0.5, 0.000, 0, 0x0, 0, 300, 1, 1, 1},  //High_Position;海拔高度位置 8

    {0, 1, 0, 0, 0, 0, 0, 200, 0, 0, 100, 0.5, 0.000, 0, 0x0, 0, 300, 1, 1, 1},  //location  定位补偿 9


};

/**
  * @brief  PID_Init(PID参数初始化函数)
  * @param  PID_Controler PID控制器、Controler_Label PID参数表
  */
void PID_Init(PID_Controler *Controler, Controler_Label Label)
{
    Controler->Err_Limit_Flag = (uint8_t)(Control_Unit[Label][0]);            //1偏差限幅标志
    Controler->Integrate_Limit_Flag = (uint8_t)(Control_Unit[Label][1]);      //2积分限幅标志
    Controler->Integrate_Separation_Flag = (uint8_t)(Control_Unit[Label][2]); //3积分分离标志
    Controler->Expect = Control_Unit[Label][3];                               //4期望
    Controler->FeedBack = Control_Unit[Label][4];                             //5反馈值
    Controler->Err = Control_Unit[Label][5];                                  //6偏差
    Controler->Last_Err = Control_Unit[Label][6];                             //7上次偏差
    Controler->Err_Max = Control_Unit[Label][7];                              //8偏差限幅值
    Controler->Integrate_Separation_Err = Control_Unit[Label][8];             //9积分分离偏差值
    Controler->Integrate = Control_Unit[Label][9];                            //10积分值
    Controler->Integrate_Max = Control_Unit[Label][10];                       //11积分限幅值
    Controler->Kp = Control_Unit[Label][11];                                  //12控制参数Kp
    Controler->Ki = Control_Unit[Label][12];                                  //13控制参数Ki
    Controler->Kd = Control_Unit[Label][13];                                  //14控制参数Ki
    Controler->Control_OutPut = Control_Unit[Label][14];                      //15控制器总输出
    Controler->Last_Control_OutPut = Control_Unit[Label][15];                 //16上次控制器总输出
    Controler->Control_OutPut_Limit = Control_Unit[Label][16];                //17上次控制器总输出
    Controler->Scale_Kp = Control_Unit[Label][17];
    Controler->Scale_Ki = Control_Unit[Label][18];
    Controler->Scale_Kd = Control_Unit[Label][19];
}

void Total_PID_Init(void)
{
    PID_Init(&Total_Controller.Pitch_Angle_Control, Pitch_Angle_Controler);
    PID_Init(&Total_Controller.Pitch_Gyro_Control, Pitch_Gyro_Controler);
    PID_Init(&Total_Controller.Roll_Angle_Control, Roll_Angle_Controler);
    PID_Init(&Total_Controller.Roll_Gyro_Control, Roll_Gyro_Controler);
    PID_Init(&Total_Controller.Yaw_Angle_Control, Yaw_Angle_Controler);
    PID_Init(&Total_Controller.Yaw_Gyro_Control, Yaw_Gyro_Controler);
    PID_Init(&Total_Controller.High_Position_Control, High_Position_Controler);
    PID_Init(&Total_Controller.High_Speed_Control, High_Speed_Controler);
    PID_Init(&Total_Controller.Location_X_Control, Location_X_Conrtoler);
    PID_Init(&Total_Controller.Location_Y_Control, Location_Y_Conrtoler);
}

float PID_Control(PID_Controler *Controler)
{
    /*******偏差计算*********************/
    Controler->Last_Err = Controler->Err;                     //保存上次偏差
    Controler->Err = Controler->Expect - Controler->FeedBack; //期望减去反馈得到偏差
    if (Controler->Err_Limit_Flag == 1)                       //偏差限幅度标志位
    {
        if (Controler->Err >= Controler->Err_Max)
            Controler->Err = Controler->Err_Max;
        if (Controler->Err <= -Controler->Err_Max)
            Controler->Err = -Controler->Err_Max;
    }
    /*******积分计算*********************/
    if (Controler->Integrate_Separation_Flag == 1) //积分分离标志位
    {
        if (fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
            Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
    }
    else
    {
        Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
    }
    /*******积分限幅*********************/
    if (Controler->Integrate_Limit_Flag == 1) //积分限制幅度标志
    {
        if (Controler->Integrate >= Controler->Integrate_Max)
            Controler->Integrate = Controler->Integrate_Max;
        if (Controler->Integrate <= -Controler->Integrate_Max)
            Controler->Integrate = -Controler->Integrate_Max;
    }
    /*******总输出计算*********************/
    Controler->Last_Control_OutPut = Controler->Control_OutPut; //输出值递推
    Controler->Control_OutPut =
        Controler->Scale_Kp * Controler->Kp * Controler->Err      //比例
        + Controler->Integrate                                    //积分
        + Controler->Kd * (Controler->Err - Controler->Last_Err); //微分
    /*******总输出限幅*********************/
    if (Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
        Controler->Control_OutPut = Controler->Control_OutPut_Limit;
    if (Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
        Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
    /*******返回总输出*********************/
    return Controler->Control_OutPut;
}

float PID_Control_Yaw(PID_Controler *Controler)
{
    /*******偏差计算*********************/
    Controler->Last_Err = Controler->Err;                     //保存上次偏差
    Controler->Err = Controler->Expect - Controler->FeedBack; //期望减去反馈得到偏差  FeedBack
    /***********************偏航角偏差超过+-180处理*****************************/
    //if(Controler->Err < -180)  Controler->Err = Controler->Err + 360;
    //if(Controler->Err > 180)  Controler->Err = Controler->Err - 360;

    if (Controler->Err_Limit_Flag == 1) //偏差限幅度标志位
    {
        if (Controler->Err >= Controler->Err_Max)
            Controler->Err = Controler->Err_Max;
        if (Controler->Err <= -Controler->Err_Max)
            Controler->Err = -Controler->Err_Max;
    }
    /*******积分计算*********************/
    if (Controler->Integrate_Separation_Flag == 1) //积分分离标志位
    {
        if (fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
            Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
    }
    else
    {
        Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
    }
    /*******积分限幅*********************/
    if (Controler->Integrate_Limit_Flag == 1) //积分限制幅度标志
    {
        if (Controler->Integrate >= Controler->Integrate_Max)
            Controler->Integrate = Controler->Integrate_Max;
        if (Controler->Integrate <= -Controler->Integrate_Max)
            Controler->Integrate = -Controler->Integrate_Max;
    }
    /*******总输出计算*********************/
    Controler->Last_Control_OutPut = Controler->Control_OutPut; //输出值递推
    Controler->Control_OutPut =
        Controler->Scale_Kp * Controler->Kp * Controler->Err      //比例  位置式
        + Controler->Integrate                                    //积分
        + Controler->Kd * (Controler->Err - Controler->Last_Err); //微分
    /*******总输出限幅*********************/
    if (Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
        Controler->Control_OutPut = Controler->Control_OutPut_Limit;
    if (Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
        Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
    /*******返回总输出*********************/
    return Controler->Control_OutPut;
}
