#ifndef __INTERFACE_TO_ALGORITHM_H__
#define __INTERFACE_TO_ALGORITHM_H__

#include <vector>
#include "MembershipFunction.h"
#include "FuzzySet.h"
#include "Rule.h"
#include "RuleSet.h"

class InterfaceToAlgorithm{
private:
	static InterfaceToAlgorithm* singleton;
	static const int ruleSize = 243; // 규칙의 전체 수
	const int inputParSize = 5; // 입력 파라미터 개수

	// 1. 먼저 각 FuzzySet에 대한 Membership Function을 만든다.
	// 종형 함수를 사용한다. 파라미터는 (너비, 중심점 좌표, 최대값) 순이다.
	BellMemberFunction mfTempLow, mfTempMid, mfTempHigh;
	BellMemberFunction mfHumidLow, mfHumidMid, mfHumidHigh;
	BellMemberFunction mfGasLow, mfGasMid, mfGasHigh;
	BellMemberFunction mfSmokeLow, mfSmokeMid, mfSmokeHigh;
	BellMemberFunction mfVideoLow, mfVideoMid, mfVideoHigh;
	BellMemberFunction mfFireX, mfFireO;

	// 2. FuzzySet을 정의하고 Membership Function을 등록한다.
	FuzzySet tempLow, tempMid, tempHigh, humidLow, humidMid, humidHigh, gasLow, gasMid, gasHigh, smokeLow, smokeMid, smokeHigh, videoLow, videoMid, videoHigh, fireO, fireX;

	FuzzySet* tempArr[3];
	FuzzySet* HumidArr[3];
	FuzzySet* gasArr[3];
	FuzzySet* smokeArr[3];
	FuzzySet* videoArr[3];

	RuleSet ruleSet;

public:
	InterfaceToAlgorithm() :
		mfTempLow(40, 20, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfTempMid(40, 40, 1), mfTempHigh(40, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		mfHumidLow(30, 30, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfHumidMid(30, 45, 1), mfHumidHigh(30, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		mfGasLow(10, 5, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfGasMid(10, 10, 1), mfGasHigh(10, 15, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		mfSmokeLow(10, 5, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfSmokeMid(10, 10, 1), mfSmokeHigh(10, 15, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		mfVideoLow(50, 25, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfVideoMid(50, 50, 1), mfVideoHigh(50, 75, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		mfFireX(120, 0, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfFireO(120, 100, 1, BellMemberFunction::TYPE::UPPER_BOUND),
		tempLow("TL"), tempMid("TM"), tempHigh("TH"), humidLow("HL"), humidMid("HM"), humidHigh("HH"), gasLow("GL"), gasMid("GM"), gasHigh("GH"), smokeLow("SL"), smokeMid("SM"), smokeHigh("SH"), videoLow("VL"), videoMid("VM"), videoHigh("VH"), fireO("fireO"), fireX("fireX"),
		ruleSet(ruleSize)
	{
		singleton = NULL;

		tempArr[0] = &tempLow, tempArr[1] = &tempMid, tempArr[2] = &tempHigh;
		HumidArr[0] = &humidLow, HumidArr[1] = &humidMid, HumidArr[2] = &humidHigh;
		gasArr[0] = &gasLow, gasArr[1] = &gasMid, gasArr[2] = &gasHigh;
		smokeArr[0] = &smokeLow, smokeArr[1] = &smokeMid, smokeArr[2] = &smokeHigh;
		videoArr[0] = &videoLow, videoArr[1] = &videoMid, videoArr[2] = &videoHigh;
	}

	void initialize(){
		tempLow.setMemberFunction(&mfTempLow);
		tempMid.setMemberFunction(&mfTempMid);
		tempHigh.setMemberFunction(&mfTempHigh);
		humidLow.setMemberFunction(&mfHumidLow);
		humidMid.setMemberFunction(&mfHumidMid);
		humidHigh.setMemberFunction(&mfHumidHigh);
		gasLow.setMemberFunction(&mfGasLow);
		gasMid.setMemberFunction(&mfGasMid);
		gasHigh.setMemberFunction(&mfGasHigh);
		smokeLow.setMemberFunction(&mfSmokeLow);
		smokeMid.setMemberFunction(&mfSmokeMid);
		smokeHigh.setMemberFunction(&mfSmokeHigh);
		videoLow.setMemberFunction(&mfVideoLow);
		videoMid.setMemberFunction(&mfVideoMid);
		videoHigh.setMemberFunction(&mfVideoHigh);
		fireO.setMemberFunction(&mfFireO);
		fireX.setMemberFunction(&mfFireX);

		// 3. 규칙을 정의한다.
		Rule rule[ruleSize];
		for (int i = 0; i < ruleSize; i++){
			rule[i] = Rule(inputParSize); // Rule(5)인 이유는 각 규칙마다 조건이 5개 들어가기 때문
		}

		// '화재'일 경우
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				for (int k = 0; k < 3; k++){
					for (int m = 0; m < 3; m++){
						for (int n = 0; n < 3; n++){
							int idx = i*3*3*3*3+j*3*3*3+k*3*3+m*3+n;
							rule[idx].addCondFuzzySet(tempArr[i]);
							rule[idx].addCondFuzzySet(HumidArr[j]);
							rule[idx].addCondFuzzySet(gasArr[k]);
							rule[idx].addCondFuzzySet(smokeArr[m]);
							rule[idx].addCondFuzzySet(videoArr[n]);
							rule[idx].registerResultFuzzySet(&fireO, "fireO");
						}
					}
				}
			}
		}

		// '비화재'일 경우
		int noFireRuleNum[18] = { 0, 3, 6, 27, 28, 30, 33, 36, 54, 55, 57, 58, 60, 63, 66, 108, 135, 138 };
		for (int i = 0; i < 18; i++){
			rule[noFireRuleNum[i]].registerResultFuzzySet(&fireX, "fireX");
		}

		// 4. 앞에서 만든 81개의 규칙을 RuleSet에 집어넣는다.
		for (int i = 0; i < ruleSize; i++){
			ruleSet.addRule(rule[i]);
		}
	}

	double getDefuzz(double tem, double hum, double gas, double smoke, double video){
		vector<double> cond = { tem, hum, gas, smoke, video };
		return ruleSet.getResultValue(cond, false);
	}

	static InterfaceToAlgorithm& getInstance(){
		if (singleton == NULL)
			singleton = new InterfaceToAlgorithm();

		return *singleton;
	}
};

#endif