#ifndef __RULE_H__
#define __RULE_H__

#include <string>
#include "FuzzySet.h"
using namespace std;

// 규칙을 정의한다.
// 예를 들어, if X is A1 and Y is B2, then W is C1 이라는 문장이 규칙이다.
// 이 규칙에서 조건의 수는 A1, B2 총 2개이므로 mCondSize == 2 된다.
class Rule{
private:
	int mCondSize; // 조건이되는 FuzzySet들을 담는 배열의 크기
	int mFuzzysetArrCount; // 현재 등록된 조건 FuzzySet의 수. 최대로 mCondSize까지 커질 수 있음
	FuzzySet** mFuzzysetArr; // 조건이되는 FuzzySet들을 담는 배열
	FuzzySet* mFuzzysetResult; // C1이라는 FuzzySet이 결과로 나타난다면 C1을 저장함
	string mFuzzysetResultKey; // C1이라는 FuzzySet이 결과로 나타난다면 문자열 "C1"을 저장함
public:
	Rule(){
		mCondSize = 0;
		mFuzzysetArrCount = 0;
		mFuzzysetArr = NULL;
	}
	Rule(int condSize){
		mCondSize = condSize;
		mFuzzysetArrCount = 0;
		mFuzzysetArr = new FuzzySet*[condSize];
	}
	Rule(Rule& rule){
		copyObject(rule);
	}
	~Rule(){
		if (mFuzzysetArr != NULL) delete[] mFuzzysetArr;
	}

	void operator=(Rule& rule){
		if (mFuzzysetArr != NULL){
			delete[] mFuzzysetArr;
			mFuzzysetArr = NULL;
		}
		copyObject(rule);
	}

	// 객체를 복사하는 함수. 복사생성자와 =연산에서 사용됨
	void copyObject(Rule& rule){
		mCondSize = rule.mCondSize;
		mFuzzysetArrCount = rule.mFuzzysetArrCount;
		mFuzzysetResult = rule.mFuzzysetResult;
		mFuzzysetResultKey = rule.mFuzzysetResultKey;
		if (rule.mFuzzysetArr != NULL){
			mFuzzysetArr = new FuzzySet*[rule.mCondSize];
			for (int i = 0; i < rule.mCondSize; i++)
				mFuzzysetArr[i] = rule.mFuzzysetArr[i];
		}
		else mFuzzysetArr = NULL;
	}

	// 조건이 되는 FuzzySet 등록
	void addCondFuzzySet(FuzzySet* fset){
		if (mFuzzysetArrCount == mCondSize)
			printf("error: FuzzySet Array is full\n");
		else{
			mFuzzysetArr[mFuzzysetArrCount] = fset;
			mFuzzysetArrCount++;
		}
	}

	// 조건이 되는 FuzzySet 배열에서, 특정 index의 FuzzySet을 반환함
	FuzzySet* getCondFuzzySet(int index){
		if (mFuzzysetArr != NULL && mFuzzysetArrCount > index)
			return mFuzzysetArr[index];
		else{
			printf("error: getCondFuzzySet() failed!\n");
			return NULL;
		}
	}

	// mCondSize 반환
	int getCondSize(){
		return mCondSize;
	}

	// mFuzzysetResultKey 반환
	string getFuzzySetResultKey(){
		return mFuzzysetResultKey;
	}

	// mFuzzysetResult 반환
	FuzzySet* getFuzzysetResult(){
		return mFuzzysetResult;
	}

	// 결과가 되는 FuzzySet 등록 (오직 하나만 등록 가능)
	void registerResultFuzzySet(FuzzySet* fset, string key){
		mFuzzysetResult = fset;
		mFuzzysetResultKey = key;
	}
};

#endif