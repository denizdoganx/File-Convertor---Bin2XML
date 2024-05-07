#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct _record {
	char name[64]; //utf-16
 	char surname[32]; //utf-8
 	char gender;
 	char email[32];
 	char phone_number[16];
 	char address[32];
 	char level_of_education[8];
 	unsigned int income_level; // given little-endian
 	unsigned int expenditure; // given big-endian 
 	char currency_unit[16];
 	char currentMood[32];
 	float height;
 	unsigned int weight;
};

typedef struct _record record;
int findBigEndianForm(int decimalNumber);
void printOfPersonInfo(record x);
void printXMLFile(char tag[], char info[], FILE* fp, int format);
void Bin2XML(char* inputFile, char* outputFile);

int main(){
	
	Bin2XML("records.dat", "records.xml");
	return 0;
	
}

void Bin2XML(char* inputFile, char* outputFile){
	char version[50] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	char first[15] = "<records>";
	char last[15] = "</records>";
	int tempExpenditure;
	int arrayLength = 100;
	char top[15] = "<row id=\"";
	int id = 1;
	char bottom[5] = "\">";
	char lastOfRow[15] = "</row>";
	struct _record records[arrayLength];
	record *rItem = &records;
	record wItem;
	FILE *fp;
	FILE *w_fp;
	int index = 0, indexOfLastItem;
	
	fp = fopen(inputFile, "rb");
	
	if(fp == NULL){
		printf("File is not found!");
		exit(0);
	}
	
	while(!feof(fp) && index != arrayLength - 1){
		fread(rItem, sizeof(record), 1, fp);
		printOfPersonInfo(records[index]);
		if(strcmp(records[index].name, "\0") == 0 || strcmp(records[index].name, "") == 0 || strcmp(records[index].name, " ") == 0){
			break;
		} 
		index++;
		rItem++;
	}
	
	fclose(fp);
	indexOfLastItem = index;
	w_fp = fopen(outputFile,"wb");
	fwrite(version, sizeof(char), strlen(version), w_fp);
	fwrite(first, sizeof(char), strlen(first),w_fp);
	
	
	for(index = 1;index <= indexOfLastItem - 1; index++){
		
		//Here First Tag Of Row is printing xml File.
		 
		fwrite(top, sizeof(char), strlen(top), w_fp);
		fprintf(w_fp, "%d", id);
		fwrite(bottom, sizeof(char), strlen(bottom), w_fp);
		//id increasing step by step.
		id++;
		
		strcpy(wItem.name, records[index].name);
		strcpy(wItem.surname, records[index].surname);
		wItem.gender = records[index].gender;
		strcpy(wItem.email, records[index].email);
		strcpy(wItem.phone_number, records[index].phone_number);
		strcpy(wItem.address, records[index].address);
		strcpy(wItem.level_of_education, records[index].level_of_education);
		wItem.income_level = records[index].income_level;
		wItem.expenditure = records[index].expenditure;
		strcpy(wItem.currency_unit, records[index].currency_unit);
		strcpy(wItem.currentMood, records[index].currentMood);
		wItem.height = records[index].height;
		wItem.weight = records[index].weight;
		
		//Here Printing file operations	
		printXMLFile("name", wItem.name, w_fp, 0);
		printXMLFile("surname", wItem.surname, w_fp, 0);
		printXMLFile("gender", wItem.gender, w_fp, 1);
		printXMLFile("email", wItem.email, w_fp, 0);
		printXMLFile("phone_number", wItem.phone_number, w_fp, 0);
		printXMLFile("address", wItem.address, w_fp, 0);
		printXMLFile("income_level", wItem.income_level, w_fp, 2);
		fwrite("<expenditure>", sizeof(char),strlen("<expenditure>"), w_fp);
		tempExpenditure = findBigEndianForm(wItem.expenditure);
		wItem.expenditure = tempExpenditure;
		fprintf(w_fp,"%d",wItem.expenditure);
		fwrite("</expenditure>", sizeof(char),strlen("</expenditure>"), w_fp);
		printXMLFile("level_of_education", wItem.level_of_education, w_fp, 0);
		fwrite("<height>", sizeof(char),strlen("<height>"), w_fp);
		fprintf(w_fp,"%.2f",wItem.height);
		fwrite("</height>", sizeof(char),strlen("</height>"), w_fp);
		
		//Here last Tag Of Row is printing xml File. 
		fwrite(lastOfRow, sizeof(char), strlen(lastOfRow), w_fp);
	}
	
	fwrite(last, sizeof(char),strlen(last),w_fp);
	fclose(w_fp);
	
	printf("\n\nXml file is saved to the location of the project.\n");
}

void printXMLFile(char tag[], char info[], FILE* fp, int format){
	//writes the information to the xml file according to the format given to this procedure
	char y[25], z[25];
	strcpy(y, "<");
	strcat(y, tag);
	strcat(y, ">");
	strcpy(z, "</");
	strcat(z, tag);
	strcat(z, ">");
	fwrite(y, sizeof(char),strlen(y), fp);
	if(format == 0){
		fwrite(info, sizeof(char), strlen(info), fp);
	}
	else if(format == 1){
		fprintf(fp, "%c", info);
	}
	else if(format == 2){
		fprintf(fp,"%d", info);
	}
	fwrite(z, sizeof(char),strlen(z), fp);
}

void printOfPersonInfo(record x){
	printf("name = %s\n",x.name);
	printf("surname = %s\n", x.surname);
	printf("gender = %c\n", x.gender);
	printf("email = %s\n", x.email);
	printf("phone number = %s\n", x.phone_number);
	printf("address = %s\n", x.address);
	printf("level of education = %s\n", x.level_of_education);
	printf("income level = %d\n", x.income_level);
	printf("expenditure = %d\n", x.expenditure);
	printf("currency unit = %s\n", x.currency_unit);
	printf("current mood = %s\n", x.currentMood);
	printf("height = %f\n", x.height);
	printf("weight = %d\n", x.weight);
	printf("\n**************************\n");
}

int findBigEndianForm(int decimalNumber){
	//This function converts a decimal number to a binary number,
	// then reverses the binary number to find the "Big-Endian" form of the given number.
	int result = 0, lastIndex;
	long int remainder,quotient;
	int binaryNumber[100],i=1,j;
	quotient = decimalNumber;
	
	while(quotient!=0){
		binaryNumber[i++]= quotient % 2;
		quotient = quotient / 2;
	}
	
	lastIndex = i - 2;
	
	for(j = 1;j <= i - 1; j++){
		result = result + (binaryNumber[j] * (int)pow(2, lastIndex));
		lastIndex = lastIndex - 1;
	}
	if(result < 0){
		result = result*-1;
	}
	return result;
}
