#ifndef __RULESET_H__
#define __RULESET_H__

#include <map>
#include <vector>
#include <algorithm>
#include "Rule.h"

// Rule�� ������ ����Ͽ� ���������� fuzzy inference ������ �ϰ� ��
class RuleSet{
private:
	int mRuleArrSize; // Rule�� ����Ǵ� �迭�� ũ��
	int mRuleCount; // ���� ��ϵ� Rule�� ��
	Rule* mRuleArr; // Rule�� ����Ǵ� �迭
	map<string, double> mMapMax; // ����� �Ǵ� FuzzySet �̸���, �װ��� �ִ밪�� ���ε�
	map<string, FuzzySet*> mMapResult; // ����� �Ǵ� FuzzySet �̸���, �װ��� �����Ͱ� ���ε�
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

	// ��ü�� �����ϴ� �Լ�. ��������ڿ� =���꿡�� ����
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

	// RuleSet�� Rule�� �߰���
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

		// �ʱ�ȭ
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

		if (printMsg) printf("\n  defuzzification: %lf\n", defuzz); // ����� 0~100����. ���� 300�� ����

		if (printMsg) printf("=================================\n");

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//for (map<string, double>::iterator i = mMapMax.begin(); i != mMapMax.end(); i++)
		//	printf("  max(\"%s\"): %lf, ", i->first.c_str(), i->second);
		//printf("defuzz: %lf\n", defuzz);
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		return defuzz;
	}

	// defuzzification() ���� ���Ǵ� �Լ�. y = f(x)
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

	// ������ȭ �Լ�
	// ���� : �ݵ�� getResultValue() �Լ� �Ŀ� ����� ��!
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