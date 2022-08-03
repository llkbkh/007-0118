#pragma once
struct CoorDinate
{
	float x;
	float y;
	float z;
};

struct F_RevolvingBody //F旋转体
{
	int Field;//场地
	int Yaw;//偏航
	int turn;//转动
};

struct FTPOV
{
	CoorDinate Position;//位置
	F_RevolvingBody RevolvingBody;//旋转体
	float FovAngle;//Fov角度
};


class Algorithm_s
{
public:
	//==========================三角函数计算==============================================
	double Seek_X_Angle(CoorDinate Own, CoorDinate Target);//求X角度
	double SeekDistance(CoorDinate Own, CoorDinate Target);//求距离
	double Seek_Y_Angle(double Distance, CoorDinate Own, CoorDinate Target);//求Y角度
	double Get_Angle_deviation(double OwnAngle, double TestAngle);//取角度差

	//=========================世界矩阵计算==========================================
	FTPOV GetVisualAngle(CoorDinate Vector, CoorDinate VisualAngle, float FovAngle);//获得视角 //Vector自己坐标 // VisualAngle自己视角//FovAngle自己Fov视角
	CoorDinate MapCoordinatesToScreenCoordinates(CoorDinate LocalLocation, CoorDinate TargetLocation, float FovAngle, F_RevolvingBody RevolvingBody, CoorDinate GameWindowSize);//地图坐标到屏幕坐标
private:
private:

};

extern Algorithm_s Algorithm;