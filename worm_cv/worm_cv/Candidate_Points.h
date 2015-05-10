﻿#pragma once

#include "CONST_PARA.h"
#include <opencv2/highgui/highgui.hpp>

class Owner_Mark;

// Candidate_Center_Points结构体表示中心线候选点
class Candidate_Points{
private:
	int point_num; // 中心点的个数
	int cood[SKELETONIZE::POINT_NUM_MAX][2];//候选点坐标
	int hash_table[WORM::IMAGE_SIZE + 1];//hash表，表中每项存储的是中心线候选点x坐标小于该位置的数目
	int current_line;
	mutable int range_x[2], range_y[2];
	Candidate_Points(const Candidate_Points &){};
	Candidate_Points & operator= (const Candidate_Points &){};
	void Range_Calc(const Multi_Points & base_points) const;

public:
	Candidate_Points(){		
		Reset();
	};
	void Reset(){
		point_num = 0;
		current_line = 0;
		hash_table[0] = 0;
	}
	int Get_Point_Num() const{
		return point_num;
	}
	const double * Get_Center(const Multi_Points & points) const;
	int Branch_Analize(Multi_Points & point_find, int * branch_stamp) const;
	Multi_Points Query_Points_Nearby(const Multi_Points & base_points, const Owner_Mark & point_info) const;
	Multi_Points Query_Points_By_Pointer(const double * base_point, const double * direct_vec, const Owner_Mark & point_info) const;
	void Add_Line();
	void Add_Point_To_Line(int y);
	void Save2File(std::string file_name);
	std::string getPointStr(const Multi_Points & points) const;
	std::string getPointStr() const;
};

class Candidate_Points_Detect{
private:
	cv::Mat binary_image;
	cv::Mat distance_matrix;
	double (* laplacian_matrix)[WORM::IMAGE_SIZE];
	int Worm_XY_Range[4];
	static const double HALF_THRES;

	void Distance_Retrace(double width_max);
	void Calc_LapMat_Of_Inner_Part() const;
	inline bool Lap_Value_Small_Enough(int i, int j) const;
	void Catch_Candidate_By_LapMat(Candidate_Points &candidate_Points);
	void Contour_Range_Get(std::vector<cv::Point> worm_contour, int * Range);
	void Denoise_And_Worm_Locate();
public:
	Candidate_Points_Detect(){
		laplacian_matrix = new double[WORM::IMAGE_SIZE][WORM::IMAGE_SIZE];
	}
	~Candidate_Points_Detect(){
		delete[] laplacian_matrix;
	}

	void Save2File(std::string dir_str, std::string pic_num) const;
	double Get_Dist(double x, double y) const;
	void Detect_Points(const cv::Mat & binary_image, Candidate_Points & candidate_points, double width_max = WORM::INF);
};
