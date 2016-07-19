#include <cstdio>

#include "MembershipFunction.h"
#include "FuzzySet.h"
#include "Rule.h"
#include "RuleSet.h"

// �Ķ���� 2�� ����
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
		fprintf(file, "���� %.0lf%%,", humidArr[i]);
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

// �Ķ���� 1�� ����
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

void readFromCSV(vector<double>& time, vector<double>& temp, vector<double>& humid, vector<double>& gas){
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

	// ù �� ���� �ѱ��.
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

		//while (token != NULL){
		//	printf("%s, ", token);
		//	token = strtok(NULL, ",\n");
		//}
		//printf("\n");
	}

	fclose(file);
}

void writeToCSV(vector<double>& time, vector<double>& temp, vector<double>& humid, vector<double>& gas, vector<double>& defuzz){
	const char filename[256] = "result.csv";
	int rowSize = time.size();
	FILE *file;

	if ((file = fopen(filename, "w")) == NULL){
		printf("error: fopen() error!\n");
		return;
	}

	fprintf(file, "time,temp,humid,gas,defuzz\n");
	for (int i = 0; i < rowSize; i++){
		fprintf(file, "%lf,%lf,%lf,%lf,%lf\n", time[i], temp[i], humid[i], gas[i], defuzz[i]);
	}

	fclose(file);
}

int main(){
	// 1. ���� �� FuzzySet�� ���� Membership Function�� �����.
	// ���� �Լ��� ����Ѵ�. �Ķ���ʹ� (�ʺ�, �߽��� ��ǥ, �ִ밪) ���̴�.
	BellMemberFunction mfTempLow(40, 20, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfTempMid(40, 40, 1), mfTempHigh(40, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfHumidLow(30, 30, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfHumidMid(30, 45, 1), mfHumidHigh(30, 60, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfGasLow(10, 5, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfGasMid(10, 10, 1), mfGasHigh(10, 15, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfVideoLow(50, 25, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfVideoMid(50, 50, 1), mfVideoHigh(50, 75, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	BellMemberFunction mfFireX(120, 0, 1, BellMemberFunction::TYPE::LOWER_BOUND), mfFireO(120, 100, 1, BellMemberFunction::TYPE::UPPER_BOUND);
	
	// 2. FuzzySet�� �����ϰ� Membership Function�� ����Ѵ�.
	FuzzySet tempLow, tempMid, tempHigh, humidLow, humidMid, humidHigh, gasLow, gasMid, gasHigh, videoLow, videoMid, videoHigh, fireO, fireX;
	tempLow.setMemberFunction(&mfTempLow);
	tempMid.setMemberFunction(&mfTempMid);
	tempHigh.setMemberFunction(&mfTempHigh);
	humidLow.setMemberFunction(&mfHumidLow);
	humidMid.setMemberFunction(&mfHumidMid);
	humidHigh.setMemberFunction(&mfHumidHigh);
	gasLow.setMemberFunction(&mfGasLow);
	gasMid.setMemberFunction(&mfGasMid);
	gasHigh.setMemberFunction(&mfGasHigh);
	videoLow.setMemberFunction(&mfVideoLow);
	videoMid.setMemberFunction(&mfVideoMid);
	videoHigh.setMemberFunction(&mfVideoHigh);
	fireO.setMemberFunction(&mfFireO);
	fireX.setMemberFunction(&mfFireX);
	
	FuzzySet* tempArr[3] = { &tempLow, &tempMid, &tempHigh };
	FuzzySet* HumidArr[3] = { &humidLow, &humidMid, &humidHigh };
	FuzzySet* gasArr[3] = { &gasLow, &gasMid, &gasHigh };
	FuzzySet* videoArr[2] = { &videoLow, &videoHigh };

	// 3. ��Ģ�� �����Ѵ�. (81����)
	Rule rule[81];
	for (int i = 0; i < 81; i++){
		rule[i] = Rule(4); // Rule(4)�� ������ �� ��Ģ���� ������ 4�� ���� ����
	}
	
	// 'ȭ��'�� ���
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 3; k++){
				for (int m = 0; m < 3; m++){
					int idx = i*3*3*3+j*3*3+k*3+m;
					rule[idx].addCondFuzzySet(tempArr[i]);
					rule[idx].addCondFuzzySet(tempArr[j]);
					rule[idx].addCondFuzzySet(tempArr[k]);
					rule[idx].addCondFuzzySet(tempArr[m]);
					rule[idx].registerResultFuzzySet(&fireO, "fireO");
				}
			}
		}
	}

	// '��ȭ��'�� ���
	rule[0].registerResultFuzzySet(&fireX, "fireX");
	rule[9].registerResultFuzzySet(&fireX, "fireX");
	rule[18].registerResultFuzzySet(&fireX, "fireX");
	rule[19].registerResultFuzzySet(&fireX, "fireX");
	rule[22].registerResultFuzzySet(&fireX, "fireX");
	rule[45].registerResultFuzzySet(&fireX, "fireX");
	
	// 4. �տ��� ���� 81���� ��Ģ�� RuleSet�� ����ִ´�.
	RuleSet ruleSet(81);
	for (int i = 0; i < 81; i++){
		ruleSet.addRule(rule[i]);
	}

	//vector<double> cond = { 25, 60, 5, 10 }; // ��� ���� (�µ� 25C, ������ 60%, ���� 5, ������ 10%)
	//vector<double> cond = { 25, 60, 5, 80 }; // ��� ���ǿ��� �����ݸ� ���� (�µ� 25C, ������ 60%, ���� 5, ������ 80%)
	//vector<double> cond = { 53, 60, 5, 10 }; // ��� ���ǿ��� �µ��� ���� (�µ� 40C, ������ 60%, ���� 5, ������ 10%)
	//vector<double> cond = { 25, 55, 5, 40 };
	//ruleSet.getResultValue(cond, true); // fuzzy inference ������ ���Ѵ�.

	//writeCSV2(&ruleSet, 101);

	const double video = 10;
	vector<double> cond = { 0, 0, 0, video };
	vector<double> vecTime, vecTemp, vecHumid, vecGas, vecDefuzz;
	readFromCSV(vecTime, vecTemp, vecHumid, vecGas);
	int cnt = vecTime.size();
	for (int i = 0; i < cnt; i++){
		//printf("======================================================================\n");
		//printf("time: %.2lf, temp: %.2lf��, humid: %.2lf%%, gas: %.2lf\n", vecTime[i], vecTemp[i], vecHumid[i], vecGas[i]);
		cond[0] = vecTemp[i];
		cond[1] = vecHumid[i];
		cond[2] = vecGas[i];
		vecDefuzz.push_back(ruleSet.getResultValue(cond, false));
	}
	writeToCSV(vecTime, vecTemp, vecHumid, vecGas, vecDefuzz);
	return 0;
}