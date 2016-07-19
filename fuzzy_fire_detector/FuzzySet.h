#ifndef __FUZZYSET_H__
#define __FUZZYSET_H__

#include <cstdio>
#include "MembershipFunction.h"

// FuzzySet�� �����Ѵ�.
// ���� ���, if X is A1 and Y is B2, then W is C1 �̶�� ��Ģ�� �ִٸ�
// A1, B2, C1�� FuzzySet�� �ش�ȴ�.
// �� FuzzySet�� ����� �Լ��� ���´�.
class FuzzySet{
private:
	MembershipFunction *mMemberFunc; // ����� �Լ�
public:
	FuzzySet(){
		mMemberFunc = NULL;
	}

	// ����� �Լ��� ����Ѵ�.
	void setMemberFunction(MembershipFunction* memberFunc){
		mMemberFunc = memberFunc;
	};

	// ����� �Լ��� ����� ��ȯ. y = f(x)
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