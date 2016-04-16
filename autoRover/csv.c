#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gps.h"
#include "csv.h"
struct TargetCords* GetTargetCords(char* fileName){
	FILE* file = fopen(fileName, "r");
	char line[256];
	double cords[2];
	struct TargetCords *target = malloc(sizeof(struct TargetCords));
	target->Back = 0;

	printf("Loading all cords from CSV\n");
	while (fgets(line, sizeof(line), file)) {
		CordSpliter(line,cords);
		target->Longitude = cords[1];
		target->Latitude = cords[0];
		target->Next = malloc(sizeof(struct TargetCords));
		(target->Next)->Back = target;
		target = target->Next;
		target->Next = 0;
	}
	free(target->Next);
	target->Next = 0;

	//Reverses link list.
	while(target->Back)
		target = target->Back;
	fclose(file);
	return target;
}

void CordSpliter(char* line, double* cords){
	char position = 0;
	char number[64] = {0};
	int target = 0;
	int length = strlen(line);
	int i;
	for(i = 0; i < length; i++){
		if(line[i]==','){
			position = 0;
			cords[((target++)%2)] = atof(number);
			int k;
			for(k = 0; number[k] != 0; k++){
				number[k] = 0;
			}
		}
		else if ((line[i] >= '0' && line[i] <='9')||(line[i]=='.')||(line[i]=='-')){
			number[(position++)] = line[i];
		}
	}
}
