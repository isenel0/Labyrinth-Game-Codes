#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#define MAX 10 
#define gameTime 30
typedef struct kullanici{
	char name[MAX];
	char surName[MAX];
	char userName[MAX];
	char passWord[MAX];
	int score;
}kullanici;
char **getLabFromFile(char *FileName);
void printLab(char**lab);
void playManuel(char**lab,char **elementPositions,kullanici *user);
void playAuto(char**lab,char **elementPositions,kullanici *user,int x,int y,time_t startTime,char **labelMatris,char *bag);
void findXPlace(char **lab,int *a,int *b);
void moveX(char **lab,char c,int *xRow,int *xColumn,int *flag); 
void checkXPosition(char **lab,int xRow,int xColumn,char *elementBag,int *endCondition,int *point);
char **getElementPosition(char **lab);
void showBag(char *bag);
void calculatePoint(char *bag,int *point);
void loginUser(int login,kullanici *user);
int isUserRegistered(char *username,char *password,int isLogin);
int registerUser(kullanici *user);
void saveScore(kullanici *user);
void printScores();
int main(){
	int i,login,menuButton,quitGame,gameMode,x,y;
	time_t startTime;
	char **myLab;
	char **elementPositions;
	char **labelMat;
	myLab = (char**)calloc(MAX , sizeof(char*));
	elementPositions = (char**)calloc(MAX , sizeof(char*));
	labelMat = (char**)calloc(MAX , sizeof(char*));
	char *elements =(char*)calloc(MAX,sizeof(char));
	for(i=0;i<MAX;i++){
	myLab[i] = (char*)calloc(MAX , sizeof(char));	
	elementPositions[i] = (char*)calloc(MAX , sizeof(char));
	labelMat[i] = (char*)calloc(MAX , sizeof(char));
	}
	char *fileName;
	fileName =(char*)calloc(MAX , sizeof(char));
	kullanici user;
	quitGame = 0;
	setlocale(LC_ALL, "Turkish");
	printf("Hello, enter 1 for login, 2 for register.");
	scanf("%d",&login);
	loginUser(login,&user);
	while(quitGame == 0){
	printf("\n1-How to Play?(1)\n2-Highest Scores(2)\n3-Start the game(3)\n4-Close the game(4)\nYour choice:");
	scanf("%d",&menuButton);
	switch(menuButton){
		case 1:
			printf("In our game, you start at the position marked with X inside a labyrinth. The general goal is to collect opposing protons and electrons to accumulate as many points as possible.\nHowever, within the labyrinth, there are counterparts of these elements. A counterpart and the original element neutralize each other.\nK stands for black holes, entering which ends the game. C represents the exit. The game is played against time.\nAt the end of the game, your bag should only contain opposing protons and electrons.\n");
			break;
		case 2:
			printScores();
			break;
		case 3:
		    printf("Enter file name:(map1.txt):\nFile Name:",MAX);
	        scanf("%s",fileName);
	        printf("Which game mode you want to play?\n1-Manuel(1)\n2-Automatic(2)\n");
	        scanf("%d",&gameMode);
	        myLab = getLabFromFile(fileName);
	        elementPositions = getElementPosition(myLab);
	        printLab(myLab);
	        if(gameMode == 1)
	        playManuel(myLab,elementPositions,&user);
	        else if (gameMode == 2){
	        findXPlace(myLab,&x,&y);
	        startTime=time(NULL); 
	        playAuto(myLab,elementPositions,&user,x,y,startTime,labelMat,elements);
			}
			break;
		case 4:
			quitGame = 1;
	}
}	
	return 0;
}
char  **getLabFromFile(char *FileName){
	int i,rowCounter,columnCounter;
	char a;
	char **lab;
	lab = (char**)calloc(MAX , sizeof(char*));
	for(i=0;i<MAX;i++)
	lab[i] = (char*)calloc(MAX , sizeof(char));
	FILE *fptr = fopen(FileName,"r");
	if(fptr == NULL){
		printf("An error occured with opening file.");
		return;
	}
	columnCounter = 0;
	rowCounter = 0;
	while((a = fgetc(fptr)) != EOF){ 
		if(a != '\n' && a != ' '){
		lab[rowCounter][columnCounter] = a;
		columnCounter++;
		}else if(a == '\n'){
			rowCounter ++;
			columnCounter = 0;
		}
	}
	return lab;
}
void printLab(char**lab){
	int i,j;
	for(i=0;lab[i][0] != '\0';i++){
		for(j=0;lab[i][j] != '\0';j++){
			printf("%c ",lab[i][j]);
		}
		printf("\n");
	}
}
void playManuel(char **lab,char **elementPositions,kullanici *user){
	time_t start_time, current_time;
    double elapsed_time;
	int point = 0;
	int endCondition = 1;
	int flag=0;
	int counter = 0;
	char c;
	char *elementBag =(char*)calloc(MAX,sizeof(char));
	int xRow,xColumn;
	printf("Game started in manuel mode. Use arrow keys.\n");
	findXPlace(lab,&xRow,&xColumn);
	start_time = time(NULL);
	while(endCondition){
		current_time = time(NULL);
        elapsed_time = difftime(current_time,start_time);
        if(counter % 2 == 0 && flag == 1)
        printf("Kalan zaman : %f saniye\n",gameTime - elapsed_time);
		if(counter % 2 == 0 && flag == 1)
	    printLab(lab);
		c = getch();
		moveX(lab,c,&xRow,&xColumn,&flag);
		if(flag == 1){
		system("cls");
		checkXPosition(elementPositions,xRow,xColumn,elementBag,&endCondition,&point);
		printf("\n");
		if(endCondition != 0)
		showBag(elementBag);
		}
		counter += 1;
		if(c == 27){
			endCondition = 0;
			calculatePoint(elementBag,&point);
		}
		if(elapsed_time >= gameTime){
			printf("Out of time.");
		    calculatePoint(elementBag,&point);
			endCondition = 0;
		}
    }
    user->score = point;
    saveScore(user);
}
void findXPlace(char **lab,int*a,int*b){
	int i,j;
	int flag =0;
	for(i=0;lab[i][0] != '\0' && flag == 0;i++){
		for(j=0;lab[i][j] != '\0' && flag == 0;j++){
			if(lab[i][j] == 'X'){
				*a = i;
				*b = j;
			}
		}
	}	  
}
void moveX(char **lab,char c,int *xRow,int *xColumn,int *flag){
	int y=0;
	switch (c) {
                case 75:
                	if(*xColumn>=1&&lab[*xRow][*xColumn-1] != '1' && lab[*xRow][*xColumn-1] != '\0'){
                		lab[*xRow][*xColumn] = '0';
                		lab[*xRow][*xColumn-1] = 'X';
                		*xColumn-=1;
                		y = 1;
					}
                    break;
                case 77:
                	if(lab[*xRow][*xColumn+1] != '1' && lab[*xRow][*xColumn+1] != '\0'){
                		lab[*xRow][*xColumn] = '0';
                		lab[*xRow][*xColumn+1] = 'X';
                		*xColumn+=1;
                		y = 1;
				 }
                    break;
                case 72:
                	if(*xRow>=1&&lab[*xRow-1][*xColumn] != '1' && lab[*xRow-1][*xColumn] != '\0'){
                		lab[*xRow][*xColumn] = '0';
                		lab[*xRow-1][*xColumn] = 'X';
                		*xRow-=1;      
						y= 1;      		
					}
                    break;
                case 80:
                	if(lab[*xRow+1][*xColumn] != '1' && lab[*xRow+1][*xColumn] != '\0'){
                		lab[*xRow][*xColumn] = '0';
                		lab[*xRow+1][*xColumn] = 'X';
                		*xRow+=1;
                		y=1;
					}
                    break;
            }
if( y == 1)
*flag = 1;
else{
	*flag = 0;
}
}
char **getElementPosition(char **lab){
	int i,j;
	char **elements;
	elements = (char**)calloc(MAX,sizeof(char*));
	for(i=0;i<MAX;i++){
	elements[i] = (char*)calloc(2,sizeof(char));
	}
	for(i=0;lab[i][0] != '\0';i++){
		for(j=0;lab[i][j] != '\0';j++){
			if(lab[i][j] != '0' && lab[i][j] != '1')
			elements[i][j] = lab[i][j];
		}
	}
	return elements;
}
void checkXPosition(char **lab,int xRow,int xColumn,char *elementBag,int *endCondition,int *point){
	int i=0;
	while(elementBag[i] != '\0')
	i++;
	switch(lab[xRow][xColumn]){
		case 'P': 
		elementBag[i] = 'P';
		lab[xRow][xColumn] = '0';
		break;
		case 'p':
		elementBag[i] = 'p';
		lab[xRow][xColumn] = '0';
		break;
		case 'E':
		elementBag[i] = 'E';
		lab[xRow][xColumn] = '0';
		break;
		case 'e':
		elementBag[i] = 'e';
		lab[xRow][xColumn] = '0';
		break;
		case 'K':
		printf("\nGame over.\n");
		*point = 0;
		*endCondition = 0;
		break;
		case 'C':
		printf("\nYou finished the game succesfully.\n");
		calculatePoint(elementBag,point);
		*endCondition = 0;
		break;
	}
}
void showBag(char *bag){
	int i=0;
	printf("Your elements:");
	while(bag[i] != '\0'){
	printf("%c ",bag[i]);
	i++;
	}
	printf("\n");
}
void calculatePoint(char *bag,int *point){
	int countE,counte;
	int countp,countP;
	int i;
	countE =0;counte = 0;countp=0;countP=0;
	for(i=0;bag[i] != '\0';i++){
		if(bag[i] == 'E')
		countE++;
		if(bag[i] == 'p')
		countp++;
		if(bag[i] == 'P')
		countP++;
		if(bag[i] == 'e')
		counte++;		
		}
		if(countE > 0 && counte > 0){
			while(countE != 0 && counte != 0){
				countE--;
				counte--;
			}
		}
		if(countP > 0 && countp > 0){
			while(countP != 0 && countp != 0){
				countP--;
				countp--;
			}
		}
	if(countP + counte == 0){
	if(countp > 0 && countE > 0){
		if(countp > countE)
		*point = countE;
		else
		*point = countp;		
	}}else{	
		*point = 0;
	}
	printf("Your score :%d.\n",*point);
}
void loginUser(int login,kullanici *user){
	int flag = 0;
	switch(login){
		case 1:
			while(flag == 0){
			printf("Username:");
			scanf("%s",user->userName);
			printf("Password:");
			scanf("%s",user->passWord);
			if(isUserRegistered(user->userName,user->passWord,1)){
			    printf("Login succesfull.\n");
			    flag = 1;
			}else{
				printf("Check your login infos and try again.");
			}}
		break;
		case 2:
			while(flag == 0){
		    printf("Name:");
		    scanf("%s",user->name);
		    printf("Surname:");
		    scanf("%s",user->surName);
		    printf("Username:");
		    scanf("%s",user->userName);
		    printf("Password:");
		    scanf("%s",user->passWord);
		    if(registerUser(user))
		    flag = 1;
		}
		break;
	}
}
int isUserRegistered(char *username,char *password,int isLogin){
	FILE *fptr = fopen("userInfos.bin","rb");
	kullanici user;
	if(isLogin){
	while (fread(&user, sizeof(user), 1, fptr) == 1) {
        if (strcmp(user.userName, username) == 0 && strcmp(user.passWord, password) == 0) {
        	fclose(fptr);
            return 1; 
        }
    }
    fclose(fptr);
    return 0;
	}else{
	while (fread(&user, sizeof(user), 1, fptr) == 1){
        if (strcmp(user.userName, username) == 0) {
        	fclose(fptr);
            return 1; 
        }
    }
    fclose(fptr);
    return 0;	
	}	
}
int registerUser(kullanici *user){
	FILE *fptr = fopen("userInfos.bin","ab");
	if(isUserRegistered(user->userName,user->passWord,0) == 1){
		printf("This username is already in use!");
		printf("Please check your infos and try again.\n");
		fclose(fptr);
		return 0;
	}else{
		fwrite(user,sizeof(struct kullanici),1,fptr);
		printf("Registered succesfully.\n");
		fclose(fptr);
		return 1;
	}
}
void saveScore(kullanici *user){
	FILE *fptr = fopen("scores.bin","ab");
	fwrite(user,sizeof(struct kullanici),1,fptr);
	fclose(fptr);
}
void printScores(){
	int i,j,count;
	kullanici temp;
	kullanici *table;
	table = (struct kullanici*)calloc(MAX,sizeof(struct kullanici));
	i=0;count=0;
	FILE *fptr = fopen("scores.bin","rb");
	kullanici user;
	printf("Highest Scores:\n");
    while (fread(&user, sizeof(user), 1, fptr) == 1){
        table[i] = user;
    	i++;
    }
    count = i;
     for (i = 0; i < count-1; i++) {
        for (j = 0; j < count-i-1; j++) {
            if (table[j].score < table[j+1].score) {
                temp = table[j];
                table[j] = table[j+1];
                table[j+1] = temp;
            }
        }
    }
    for(i=0;i<count;i++){
    if(i < 5)
    printf("%s : %d point\n",table[i].userName,table[i].score);	}
}
void playAuto(char**lab,char **elementPositions,kullanici *user,int x,int y,time_t startTime,char **labelMat,char *bag){
	time_t currentTime;
    double elapsedTime;
	int flag=0;
	int point = 0;
	int endCondition=1;
	currentTime = time(NULL);
	elapsedTime = difftime(currentTime,startTime);
	if(elapsedTime >= gameTime){
			printf("Game over.");
		    calculatePoint(bag,&point);
			return;
		}
	labelMat[x][y] = 1;
	system("cls");
	printf("Time left: %f\n",gameTime - elapsedTime);
	showBag(bag);
	printLab(lab);
	sleep(1);
	printf("\n");
	moveX(lab,77,&x,&y,&flag);
	if(flag == 1 && labelMat[x][y] == '\0')
	{
		checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
		if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
		return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}else if(flag == 1){
	moveX(lab,75,&x,&y,&flag);
	flag = 0;
	}
	moveX(lab,80,&x,&y,&flag);
	if(flag == 1 && labelMat[x][y] == '\0')
	{
		checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
		if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
		return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}else if(flag == 1){
		moveX(lab,72,&x,&y,&flag);
		flag = 0;
	}
    moveX(lab,75,&x,&y,&flag);
	if(flag == 1 && labelMat[x][y] == '\0')
	{
		checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
		if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
		return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}else if(flag == 1){
	moveX(lab,77,&x,&y,&flag);
	flag = 0;
	}
	moveX(lab,72,&x,&y,&flag);
	if(flag == 1 && labelMat[x][y] == '\0')
	{
		checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
		if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
		return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}else if(flag == 1){
		moveX(lab,80,&x,&y,&flag);
		flag = 0;
	}
    moveX(lab,75,&x,&y,&flag);
    if(flag == 1){
    	checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
    	if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
    	return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}
	moveX(lab,72,&x,&y,&flag);
    if(flag == 1){
    	checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
    	if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
    	return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}
	moveX(lab,77,&x,&y,&flag);
    if(flag == 1){
    	checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
    	if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
    	return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}
	moveX(lab,80,&x,&y,&flag);
    if(flag == 1){
    	checkXPosition(elementPositions,x,y,bag,&endCondition,&point);
    	if(endCondition == 0){
	    user->score = point;
        saveScore(user);	
	    return;
		}
    	return playAuto(lab,elementPositions,user,x,y,startTime,labelMat,bag);
	}
    return;	
}
