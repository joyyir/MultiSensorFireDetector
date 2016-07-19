#ifndef __FUZZYSET_H__
#define __FUZZYSET_H__

#include <cstdio>
#include "MembershipFunction.h"

// FuzzySet을 정의한다.
// 예를 들어, if X is A1 and Y is B2, then W is C1 이라는 규칙이 있다면
// A1, B2, C1이 FuzzySet에 해당된다.
// 각 FuzzySet은 멤버십 함수를 갖는다.
class FuzzySet{
private:
	MembershipFunction *mMemberFunc; // 멤버십 함수
public:
	FuzzySet(){
		mMemberFunc = NULL;
	}

	// 멤버십 함수를 등록한다.
	void setMemberFunction(MembershipFunction* memberFunc){
		mMemberFunc = memberFunc;
	};

	// 멤버십 함수의 결과를 반환. y = f(x)
	double getValue(double x){
		if (mMemberFunc != NULL)
			return mMemberFunc->getValue(x);
		else{
			printf("error: Member Function is not assigned\n");
			return 0;
		}
	}
};

#endif