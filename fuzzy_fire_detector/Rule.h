#ifndef __RULE_H__
#define __RULE_H__

#include <string>
#include "FuzzySet.h"
using namespace std;

// ��Ģ�� �����Ѵ�.
// ���� ���, if X is A1 and Y is B2, then W is C1 �̶�� ������ ��Ģ�̴�.
// �� ��Ģ���� ������ ���� A1, B2 �� 2���̹Ƿ� mCondSize == 2 �ȴ�.
class Rule{
private:
	int mCondSize; // �����̵Ǵ� FuzzySet���� ��� �迭�� ũ��
	int mFuzzysetArrCount; // ���� ��ϵ� ���� FuzzySet�� ��. �ִ�� mCondSize���� Ŀ�� �� ����
	FuzzySet** mFuzzysetArr; // �����̵Ǵ� FuzzySet���� ��� �迭
	FuzzySet* mFuzzysetResult; // C1�̶�� FuzzySet�� ����� ��Ÿ���ٸ� C1�� ������
	string mFuzzysetResultKey; // C1�̶�� FuzzySet�� ����� ��Ÿ���ٸ� ���ڿ� "C1"�� ������
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

	// ��ü�� �����ϴ� �Լ�. ��������ڿ� =���꿡�� ����
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

	// ������ �Ǵ� FuzzySet ���
	void addCondFuzzySet(FuzzySet* fset){
		if (mFuzzysetArrCount == mCondSize)
			printf("error: FuzzySet Array is full\n");
		else{
			mFuzzysetArr[mFuzzysetArrCount] = fset;
			mFuzzysetArrCount++;
		}
	}

	// ������ �Ǵ� FuzzySet �迭����, Ư�� index�� FuzzySet�� ��ȯ��
	FuzzySet* getCondFuzzySet(int index){
		if (mFuzzysetArr != NULL && mFuzzysetArrCount > index)
			return mFuzzysetArr[index];
		else{
			printf("error: getCondFuzzySet() failed!\n");
			return NULL;
		}
	}

	// mCondSize ��ȯ
	int getCondSize(){
		return mCondSize;
	}

	// mFuzzysetResultKey ��ȯ
	string getFuzzySetResultKey(){
		return mFuzzysetResultKey;
	}

	// mFuzzysetResult ��ȯ
	FuzzySet* getFuzzysetResult(){
		return mFuzzysetResult;
	}

	// ����� �Ǵ� FuzzySet ��� (���� �ϳ��� ��� ����)
	void registerResultFuzzySet(FuzzySet* fset, string key){
		mFuzzysetResult = fset;
		mFuzzysetResultKey = key;
	}
};

#endif