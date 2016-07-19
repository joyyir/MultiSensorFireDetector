#include <cstdio>

#include "MembershipFunction.h"
#include "FuzzySet.h"
#include "Rule.h"
#include "RuleSet.h"

// 파라미터 2개 변경
void writeCSV(RuleSet* rset, int sample){
	const double startVal = 10, endVal = 70;
	const double interval = (endVal-startVal)/(sample-1);
	const double gas = 5, video = 10;
	const vector<double> humidArr = { 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70 };

	FILE *file;
	double temp;
	vector<double> cond = { 0, 0, gas, video };

	file = fopen("result.csv", "w");
	temp = startVal;
	fprintf(file, "gas: %lf, video: %lf\n", gas, video);
	fprintf(file, "temperature,");
	for (int i = 0; i < sample; i++){
		fprintf(file, "%lf,", temp);
		temp += interval;
	}
	fprintf(file, "\n");

	for (int i = 0; i < humidArr.size(); i++){
		fprintf(file, "습도 %.0lf%%,", humidArr[i]);
		cond[0] = startVal;
		cond[1] = humidArr[i];
		for (int j = 0; j < sample; j++){
			rset->getResultValue(cond, false);
			fprintf(file, "%lf,", rset->defuzzification(100, 300));
			cond[0] += interval;
		}
		fprintf(file, "\n");
	}
	
	fclose(file);
}

// 파라미터 1개 변경
void writeCSV2(RuleSet* rset, int sample){
	const double startVal = 0, endVal = 100;
	const double interval = (endVal-startVal)/(sample-1);
	const double temper = 25, humid = 55, gas = 5;// , video = 10;

	FILE *file;
	double temp;
	vector<double> cond = { temper, humid, gas, startVal };

	file = fopen("result.csv", "w");
	temp = startVal;
	fprintf(file, "temp: %lf, humid: %lf, gas: %lf\n", temper, humid, gas);
	fprintf(file, "image,");
	for (int i = 0; i < sample; i++){
		fprintf(file, "%lf,", temp);
		temp += interval;
	}
	fprintf(file, "\n");
	fprintf(file, "defuzz,");

	cond[3] = startVal;
	for (int i = 0; i < sample; i++){
		rset->getResultValue(cond, false);
		fprintf(file, "%lf,", rset->defuzzification(100, 300));
		cond[3] += interval;
	}
	fprintf(file, "\n");

	fclose(file);
}

void readFromCSV(vector<double>& time, vector<double>& temp, vector<double>& humid, vector<double>& gas, vector<double>& smoke){
	const char filename[256] = "input.csv";
	char line[256];
	string strLine;
	FILE *file;
	char *token;
	double tempVal;

	if ((file = fopen(filename, "r")) == NULL){
		printf("error: The file doesn't exist!\n");
		return;
	}

	// 첫 두 줄은 넘긴다.
	fgets(line, 255, file);
	fgets(line, 255, file);

	while (!feof(file)){
		fgets(line, 255, file);
		//printf("line(%d): %s", strlen(line), line);
		token = strtok(line, ",\n");
		sscanf(token, "%lf", &tempVal);
		time.push_back(tempVal);

		token = strtok(NULL, ",\n");
		sscanf(token, "%lf", &tempVal);
		temp.push_back(tempVal);

		token = strtok(NULL, ",\n");
		sscanf(token, "%lf", &tempVal);
		humid.push_back(tempVal);

		token = strtok(NULL, ",\n");
		sscanf(token, "%lf", &tempVal);
		gas.push_back(tempVal);

		token = strtok(NULL, ",\n");
		sscanf(token, "%lf", &tempVal);
		smoke.push_back(tempVal);

		//while (token != NULL){
		//	printf("%s, ", token);
		//	token = strtok(NULL, ",\n");
		//}
		//printf("\n");
	}

	fclose(file);
}

void writeToCSV(vector<double>& time, vector<double>& temp, vector<double>& humid, vector<double>& gas, vector<double>& smoke, vector<double>& defuzz){
	const char filename[256] = "result.csv";
	int rowSize = time.size();
	FILE *file;

	if ((file = fopen(filename, "w")) == NULL){
		printf("error: fopen() error!\n");
		return;
	}

	fprintf(file, "time,temp,humid,gas,smoke,defuzz\n");
	for (int i = 0; i < rowSize; i++){
		fprintf(file, "%lf,%lf,%lf,%lf,%lf,%lf\n", time[i], temp[i], humid[i], gas[i], smoke[i], defuzz[i]);
	}

	fclose(file);
}

int main(){
	const int ruleSize = 243; // 규칙의 전체 수
	const int inputParSize = 5; // 입력 파라미터 개수

	// 1. 먼저 각 FuzzySet에 대한 Membership Function을 만든다.
	// 종형 함수를 사용한다. 파라미터는 (너비, 중심점 좌표, 최대값) 순이다.
	BellMemberFunction mfTempLow(40, 20, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfTempMid(40, 40, 1), mfTempHigh(40, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfHumidLow(30, 30, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfHumidMid(30, 45, 1), mfHumidHigh(30, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfGasLow(10, 5, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfGasMid(10, 10, 1), mfGasHigh(10, 15, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfSmokeLow(10, 5, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfSmokeMid(10, 10, 1), mfSmokeHigh(10, 15, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfVideoLow(50, 25, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfVideoMid(50, 50, 1), mfVideoHigh(50, 75, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfFireX(120, 0, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfFireO(120, 100, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	
	// 2. FuzzySet을 정의하고 Membership Function을 등록한다.
	FuzzySet tempLow("TL"), tempMid("TM"), tempHigh("TH"), humidLow("HL"), humidMid("HM"), humidHigh("HH"), gasLow("GL"), gasMid("GM"), gasHigh("GH"), smokeLow("SL"), smokeMid("SM"), smokeHigh("SH"), videoLow("VL"), videoMid("VM"), videoHigh("VH"), fireO("fireO"), fireX("fireX");
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
	
	FuzzySet* tempArr[3] = { &tempLow, &tempMid, &tempHigh };
	FuzzySet* HumidArr[3] = { &humidLow, &humidMid, &humidHigh };
	FuzzySet* gasArr[3] = { &gasLow, &gasMid, &gasHigh };
	FuzzySet* smokeArr[3] = { &smokeLow, &smokeMid, &smokeHigh };
	FuzzySet* videoArr[3] = { &videoLow, &videoMid, &videoHigh };

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
		rule[ noFireRuleNum[i] ].registerResultFuzzySet(&fireX, "fireX");
	}
	
	// 4. 앞에서 만든 81개의 규칙을 RuleSet에 집어넣는다.
	RuleSet ruleSet(ruleSize);
	for (int i = 0; i < ruleSize; i++){
		ruleSet.addRule(rule[i]);
	}

	//vector<double> cond = { 25, 60, 5, 5, 10 }; // 평상 조건 (온도 25C, 상대습도 60%, 가스 5, 영상기반 10%)
	//vector<double> cond = { 25, 60, 5, 5, 80 }; // 평상 조건에서 영상기반만 높음 (온도 25C, 상대습도 60%, 가스 5, 영상기반 80%)
	//vector<double> cond = { 53, 60, 5, 5, 10 }; // 평상 조건에서 온도만 높음 (온도 40C, 상대습도 60%, 가스 5, 영상기반 10%)
	//vector<double> cond = { 25, 55, 5, 5, 40 };
	//ruleSet.getResultValue(cond, true); // fuzzy inference 연산을 취한다.

	//writeCSV2(&ruleSet, 101);

	const double video = 10;
	vector<double> cond = { 0, 0, 0, 0, video };
	vector<double> vecTime, vecTemp, vecHumid, vecGas, vecSmoke, vecDefuzz;
	readFromCSV(vecTime, vecTemp, vecHumid, vecGas, vecSmoke);
	int cnt = vecTime.size();
	for (int i = 0; i < cnt; i++){
	//	//printf("======================================================================\n");
	//	//printf("time: %.2lf, temp: %.2lf도, humid: %.2lf%%, gas: %.2lf, smoke: %.2lf\n", vecTime[i], vecTemp[i], vecHumid[i], vecGas[i], vecSmoke[i]);
		cond[0] = vecTemp[i];
		cond[1] = vecHumid[i];
		cond[2] = vecGas[i];
		cond[3] = vecSmoke[i];
		vecDefuzz.push_back(ruleSet.getResultValue(cond, false));
	}
	writeToCSV(vecTime, vecTemp, vecHumid, vecGas, vecSmoke, vecDefuzz);
	return 0;
}