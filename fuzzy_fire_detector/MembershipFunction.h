#ifndef __MEMBERSHIP_FUNCTION_H__
#define __MEMBERSHIP_FUNCTION_H__

#include <cstdio>
#include <cmath>
#include <string>
using namespace std;

// 멤버십 함수 inferface
// 모든 멤버십 함수는 이 인터페이스를 상속받는다.
class MembershipFunction{
public:
	virtual double getValue(double x) = 0; // y = f(x)
};

// 삼각형모양 멤버십 함수
class TriangleMemberFunction : public MembershipFunction{
private:
	double mWidth; // 너비
	double mCenter; // 중심점 좌표
	double mPeak; // 최고점 함수값
public:
	TriangleMemberFunction(double width, double center, double peak){
		mWidth = width;
		mCenter = center;
		mPeak = peak;
	}

	virtual double getValue(double x){
		double result;
		if (x <= mCenter)
			result = (2*mPeak / mWidth) * (x - mCenter) + mPeak;
		else
			result = -(2*mPeak / mWidth) * (x - mCenter) + mPeak;

		return (result > 0) ? result : 0;
	}
};

class BellMemberFunction : public MembershipFunction{
private:
	double mWidth; // 너비
	double mCenter; // 중심점 좌표
	double mPeak; // 최고점 함수값
	int mType;
public:
	static const enum TYPE { NORMAL, LOWER_BOUND, UPPER_BOUND };

	BellMemberFunction(double width, double center, double peak){
		mWidth = width;
		mCenter = center;
		mPeak = peak;
		mType = TYPE::NORMAL;
	}

	BellMemberFunction(double width, double center, double peak, int type) : BellMemberFunction(width, center, peak) {
		mType = type;
	}

	virtual double getValue(double x){
		double result;

		switch (mType){
		case TYPE::NORMAL:
			result = mPeak * (1 - pow(x - mCenter, 2)/pow(mWidth/2, 2)) * exp(-pow(x-mCenter, 2)/pow(mWidth/2, 2));
			return result > 0 ? result : 0;
		case TYPE::LOWER_BOUND:
			if (x < mCenter) return mPeak;
			else {
				result = mPeak * (1 - pow(x - mCenter, 2)/pow(mWidth/2, 2)) * exp(-pow(x-mCenter, 2)/pow(mWidth/2, 2));
				return result > 0 ? result : 0;
			}
		case TYPE::UPPER_BOUND:
			if (x > mCenter) return mPeak;
			else {
				result = mPeak * (1 - pow(x - mCenter, 2)/pow(mWidth/2, 2)) * exp(-pow(x-mCenter, 2)/pow(mWidth/2, 2));
				return result > 0 ? result : 0;
			}
		default:
			printf("error: BellMemberFunction::getValue(double) error!\n");
			return 0;
		}
		
	}
};

#endif