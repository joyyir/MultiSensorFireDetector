#ifndef __RULESET_H__
#define __RULESET_H__

#include <map>
#include <vector>
#include <algorithm>
#include "Rule.h"

// Rule을 여러개 등록하여 최종적으로 fuzzy inference 연산을 하게 됨
class RuleSet{
private:
	int mRuleArrSize; // Rule이 저장되는 배열의 크기
	int mRuleCount; // 현재 등록된 Rule의 수
	Rule* mRuleArr; // Rule이 저장되는 배열
	map<string, double> mMapMax; // 결과가 되는 FuzzySet 이름과, 그것의 최대값이 매핑됨
	map<string, FuzzySet*> mMapResult; // 결과가 되는 FuzzySet 이름과, 그것의 포인터가 매핑됨
public:
	RuleSet(){
		mRuleArr = NULL;
	}
	RuleSet(int ruleArrSize){
		mRuleArrSize = ruleArrSize;
		mRuleCount = 0;
		mRuleArr = new Rule[ruleArrSize];
	}
	RuleSet(RuleSet& ruleSet){
		copyObject(ruleSet);
	}

	~RuleSet(){
		if (mRuleArr != NULL) delete[] mRuleArr;
	}

	void operator=(RuleSet& ruleSet){
		if (mRuleArr != NULL){
			delete[] mRuleArr;
			mRuleArr = NULL;
		}
		copyObject(ruleSet);
	}

	// 객체를 복사하는 함수. 복사생성자와 =연산에서 사용됨
	void copyObject(RuleSet& ruleSet){
		mRuleArrSize = ruleSet.mRuleArrSize;
		mRuleCount = ruleSet.mRuleCount;
		if (ruleSet.mRuleArr != NULL){
			mRuleArr = new Rule[ruleSet.mRuleArrSize];
			for (int i = 0; i < ruleSet.mRuleArrSize; i++){
				mRuleArr[i] = ruleSet.mRuleArr[i];
			}
		}
		else mRuleArr = NULL;
	}

	// RuleSet에 Rule을 추가함
	void addRule(Rule rule){
		if (mRuleCount == mRuleArrSize)
			printf("error: Rule Array is full\n");
		else{
			mRuleArr[mRuleCount] = rule;
			mRuleCount++;
		}
	}

	double getResultValue(vector<double> cond, bool printMsg){
		int condSize;
		double minVal, defuzz;

		// 초기화
		for (map<string, double>::iterator i = mMapMax.begin(); i != mMapMax.end(); i++){
			i->second = 0;
		}

		if(printMsg) printf("===== getResultValue Result =====\n");

		for (int i = 0; i < mRuleCount; i++){
			minVal = 99999;
			condSize = mRuleArr[i].getCondSize();
			for (int j = 0; j < condSize; j++){
				minVal = min(minVal, mRuleArr[i].getCondFuzzySet(j)->getValue(cond[j]));
				//printf("    %lf ==> %lf\n", cond[j], mRuleArr[i].getCondFuzzySet(j)->getValue(cond[j]));
			}
			if (printMsg) printf("  Rule %d: %s = %lf\n", i+1, mRuleArr[i].getFuzzySetResultKey().c_str(), minVal);

			mMapResult[mRuleArr[i].getFuzzySetResultKey()] = mRuleArr[i].getFuzzysetResult();

			if (mMapMax[mRuleArr[i].getFuzzySetResultKey()] < minVal)
				mMapMax[mRuleArr[i].getFuzzySetResultKey()] = minVal;
		}
		if (printMsg) printf("\n");
		for (map<string, double>::iterator i = mMapMax.begin(); i != mMapMax.end(); i++)
			if (printMsg) printf("  max(\"%s\"): %lf\n", i->first.c_str(), i->second);

		defuzz = defuzzification(100, 300);

		if (printMsg) printf("\n  defuzzification: %lf\n", defuzz); // 결과는 0~100까지. 샘플 300개 추출

		if (printMsg) printf("=================================\n");

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//for (map<string, double>::iterator i = mMapMax.begin(); i != mMapMax.end(); i++)
		//	printf("  max(\"%s\"): %lf, ", i->first.c_str(), i->second);
		//printf("defuzz: %lf\n", defuzz);
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		return defuzz;
	}

	// defuzzification() 에서 사용되는 함수. y = f(x)
	double resultFunction(double x){
		string fsetResultName;
		double memberFuncMax, maxVal = 0, tempVal;
		FuzzySet* fsetResult;

		for (map<string, double>::iterator i = mMapMax.begin(); i != mMapMax.end(); i++){
			fsetResultName = i->first;
			memberFuncMax = i->second;
			fsetResult = mMapResult[fsetResultName];
			tempVal = (fsetResult->getValue(x) > memberFuncMax) ? memberFuncMax : fsetResult->getValue(x);
			maxVal = max(maxVal, tempVal);
		}

		return maxVal;
	}

	// 역퍼지화 함수
	// 주의 : 반드시 getResultValue() 함수 후에 사용할 것!
	double defuzzification(double endVal, int resolution){
		double top = 0, bottom = 0;
		double x = 0;

		for (int i = 0; i < resolution; i++){
			//printf("  defuzz: x = %lf, y = %lf\n", x, resultFunction(x));
			top += resultFunction(x) * x;
			bottom += resultFunction(x);
			x += endVal/(resolution-1);
		}

		return top/bottom;
	}
};

#endif