#include <stdio.h>
#include <bangtal.h>

//홈 화면, 게임 화면
SceneID home = createScene("home", "image//home.png");
SceneID game = createScene("game", "image//game.png");

//홈 화면의 종료 버튼, 도움말 버튼
ObjectID homeEnd = createObject("image//homeEnd.png"); 
ObjectID howToPlay = createObject("image//howToPlay.png"); 
ObjectID howToPlayButton = createObject("image//howToPlayButton.png"); 

//홈 화면 고리 개수 선택 버튼
ObjectID ringCount3 = createObject("image//3.png");
ObjectID ringCount4 = createObject("image//4.png");
ObjectID ringCount5 = createObject("image//5.png");
ObjectID ringCount6 = createObject("image//6.png");
ObjectID ringCount7 = createObject("image//7.png");
ObjectID ringCount8 = createObject("image//8.png");

//고리 
ObjectID ring0 = createObject("image//ring0.png");
ObjectID ring1 = createObject("image//ring1.png");
ObjectID ring2 = createObject("image//ring2.png");
ObjectID ring3 = createObject("image//ring3.png");
ObjectID ring4 = createObject("image//ring4.png");
ObjectID ring5 = createObject("image//ring5.png");
ObjectID ring6 = createObject("image//ring6.png");
ObjectID ring7 = createObject("image//ring7.png");

//회색 박스
ObjectID hanger0 = createObject("image//hanger.png");
ObjectID hanger1 = createObject("image//hanger.png");
ObjectID hanger2 = createObject("image//hanger.png");

//걸이 아래 공간 (투명)
ObjectID box0 = createObject("image//box.png");
ObjectID box1 = createObject("image//box.png");
ObjectID box2 = createObject("image//box.png");

//성공 시 화면
ObjectID clear = createObject("image//clear.png");

//게임 시간 측정 타이머
TimerID gameTimer = createTimer(0);
TimerID tempTimer = createTimer(1.0f);  

//고리의 위치 
//x -> 몇 번째 걸이인지 (0~2)
//y -> 몇 번째 고리인지 (0~7)
typedef struct {
	int x;
	int y;
} location;

location ringLocation[8];

int ringx[8];
int ringy[8];

//스택을 통해 어떤 고리가 걸려있는지 저장
int stack0[8];
int stack1[8];
int stack2[8];

int top0 = 0;
int top1 = 0;
int top2 = 0;

//게임 구동 시 사용될 변수들
int gameflag = 0;
int selectFlag = 0;
int gameSecond = 0; 
int stageNumber = -1;
int selectHanger = -1;

//답안 출력
int count; 
int arr[500][2];

void hanoi(int a, int b, int c, int N) {
	if (N == 1) {
		arr[count][0] = a;
		arr[count++][1] = c;
		return;
	}
	hanoi(a, c, b, N - 1);
	hanoi(a, b, c, 1);
	hanoi(b, a, c, N - 1);
}

//스택 푸쉬
void push(int hangerNumber, int ringNumber) {
	if (hangerNumber == 0) stack0[top0++] = ringNumber;
	else if (hangerNumber == 1) stack1[top1++] = ringNumber;
	else if (hangerNumber == 2) stack2[top2++] = ringNumber;
}

//게임 시작 시 처음 고리 위치 정해주는 함수
//선택한 고리 개수에 따라 사용될 고리의 위치를 정해준다
void locateRingInitial() {
	for (int i = 0; i < stageNumber; i++) {
		push(0, stageNumber - i - 1);
		ringLocation[i].x = 0;
		ringLocation[i].y = stageNumber - i - 1;
	}

	if (stageNumber == 3) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
	}
	else if (stageNumber == 4) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
		showObject(ring3);
	}
	else if (stageNumber == 5) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
		showObject(ring3);
		showObject(ring4);
	}
	else if (stageNumber == 6) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
		showObject(ring3);
		showObject(ring4);
		showObject(ring5);
	}
	else if (stageNumber == 7) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
		showObject(ring3);
		showObject(ring4);
		showObject(ring5);
		showObject(ring6);
	}
	else if (stageNumber == 8) {
		showObject(ring0);
		showObject(ring1);
		showObject(ring2);
		showObject(ring3);
		showObject(ring4);
		showObject(ring5);
		showObject(ring6);
		showObject(ring7);
	}	
}

//고리의 좌표를 정해주는 함수
//고리의 정해진 위치에 따라 실제 좌표를 정하고
//게임 화면 내에 위치시킨다
void locateRing() {
	for (int i = 0; i < stageNumber; i++) {
		ringx[i] = 66 + ringLocation[i].x * (66 + 340);
		ringy[i] = 101 + ringLocation[i].y * 65;
	}

	if (stageNumber == 3) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
	}
	else if (stageNumber == 4) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
		locateObject(ring3, game, ringx[3], ringy[3]);
	}
	else if (stageNumber == 5) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
		locateObject(ring3, game, ringx[3], ringy[3]);
		locateObject(ring4, game, ringx[4], ringy[4]);
	}
	else if (stageNumber == 6) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
		locateObject(ring3, game, ringx[3], ringy[3]);
		locateObject(ring4, game, ringx[4], ringy[4]);
		locateObject(ring5, game, ringx[5], ringy[5]);
	}
	else if (stageNumber == 7) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
		locateObject(ring3, game, ringx[3], ringy[3]);
		locateObject(ring4, game, ringx[4], ringy[4]);
		locateObject(ring5, game, ringx[5], ringy[5]);
		locateObject(ring6, game, ringx[6], ringy[6]);
	}
	else if (stageNumber == 8) {
		locateObject(ring0, game, ringx[0], ringy[0]);
		locateObject(ring1, game, ringx[1], ringy[1]);
		locateObject(ring2, game, ringx[2], ringy[2]);
		locateObject(ring3, game, ringx[3], ringy[3]);
		locateObject(ring4, game, ringx[4], ringy[4]);
		locateObject(ring5, game, ringx[5], ringy[5]);
		locateObject(ring6, game, ringx[6], ringy[6]);
		locateObject(ring7, game, ringx[7], ringy[7]);
	}
}

//회색 박스를 표시하는 함수
void displayHanger(int selectHanger) {
	hideObject(hanger0);
	hideObject(hanger1);
	hideObject(hanger2);
	if (selectHanger == 0) showObject(hanger0);
	else if (selectHanger == 1) showObject(hanger1);
	else if (selectHanger == 2) showObject(hanger2);
}

//게임 성공 시 실행되는 함수
//홈 화면 이동, 성공 이미지 출력
//게임 처음으로 초기화
void gameEnding() {
	enterScene(home);
	showObject(clear);

	gameflag = 0;
	selectFlag = 0;
	stageNumber = -1;
	selectHanger = -1;
	top0 = top1 = top2 = 0;

	stopTimer(gameTimer);
	stopTimer(tempTimer);
	setTimer(tempTimer, 1.0f);

	hideObject(ring0);
	hideObject(ring1);
	hideObject(ring2);
	hideObject(ring3);
	hideObject(ring4);
	hideObject(ring5);
	hideObject(ring6);
	hideObject(ring7);
}

//게임이 성공했는지 확인하는 함수
void ringCheck() {
	int checkFlag = 1;
	if (top2 != stageNumber) checkFlag = 0;
	for (int i = 0; i < stageNumber; i++)
		if (stack2[stageNumber - 1 - i] != i)
			checkFlag = 0;
	if (checkFlag == 1) gameEnding();
}

//게임 시작
void hanoiStart() {
	enterScene(game);
	locateRingInitial();
	locateRing();
	gameflag = 1;
	gameSecond = 0;
	showTimer(gameTimer);
	startTimer(tempTimer);
}

//고리의 위치를 바꾸어주는 함수
void moveRing(int a, int b) {
	selectFlag = 0;
	selectHanger = -1;
	displayHanger(-1);

	if (a == 0 && top0 != 0) {
		if (b == 1) {
			if ((top1 == 0) || (stack0[top0 - 1] < stack1[top1 - 1])) {
				ringLocation[stack0[top0 - 1]].x = 1;
				ringLocation[stack0[top0 - 1]].y = top1;
				push(1, stack0[--top0]);
			}
		}
		else if (b == 2) {
			if ((top2 == 0) || (stack0[top0 - 1] < stack2[top2 - 1])) {
				ringLocation[stack0[top0 - 1]].x = 2;
				ringLocation[stack0[top0 - 1]].y = top2;
				push(2, stack0[--top0]);
			}
		}
	}
	else if (a == 1 && top1 != 0) {
		if (b == 0) {
			if ((top0 == 0) || (stack1[top1 - 1] < stack0[top0 - 1])) {
				ringLocation[stack1[top1 - 1]].x = 0;
				ringLocation[stack1[top1 - 1]].y = top0;
				push(0, stack1[--top1]);
			}
		}
		else if (b == 2) {
			if ((top2 == 0) || (stack1[top1 - 1] < stack2[top2 - 1])) {
				ringLocation[stack1[top1 - 1]].x = 2;
				ringLocation[stack1[top1 - 1]].y = top2;
				push(2, stack1[--top1]);
			}
		}
	}
	else if (a == 2 && top2 != 0) {
		if (b == 0) {
			if ((top0 == 0) || (stack2[top2 - 1] < stack0[top0 - 1])) {
				ringLocation[stack2[top2 - 1]].x = 0;
				ringLocation[stack2[top2 - 1]].y = top0;
				push(0, stack2[--top2]);
			}
		}
		else if (b == 1) {
			if ((top1 == 0) || (stack2[top2 - 1] < stack1[top1 - 1])) {
				ringLocation[stack2[top2 - 1]].x = 1;
				ringLocation[stack2[top2 - 1]].y = top1;
				push(1, stack2[--top2]);
			}
		}
	}

	locateRing();
	ringCheck();
}

void mousecallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == homeEnd) endGame();
	else if (object == howToPlayButton) showObject(howToPlay);
	else if (object == howToPlay) hideObject(howToPlay);
	else if (object == clear) {
		hideTimer();
		hideObject(clear);
		setTimer(gameTimer, 0);
	}
	else if (object == ringCount3) { stageNumber = 3; hanoiStart(); }
	else if (object == ringCount4) { stageNumber = 4; hanoiStart(); }
	else if (object == ringCount5) { stageNumber = 5; hanoiStart(); }
	else if (object == ringCount6) { stageNumber = 6; hanoiStart(); }
	else if (object == ringCount7) { stageNumber = 7; hanoiStart(); }
	else if (object == ringCount8) { stageNumber = 8; hanoiStart(); }
	else if (gameflag == 1) {
		if (object == box0) {
			if (selectFlag == 0) {
				selectFlag = 1;
				selectHanger = 0;
				displayHanger(0);
			}
			else if (selectFlag == 1) moveRing(selectHanger, 0);
		}
		else if (object == box1) {
			if (selectFlag == 0) {
				selectFlag = 1;
				selectHanger = 1;
				displayHanger(1);
			}
			else if (selectFlag == 1) moveRing(selectHanger, 1);
		}
		if (object == box2) {
			if (selectFlag == 0) {
				selectFlag = 1;
				selectHanger = 2;
				displayHanger(2);
			}
			else if (selectFlag == 1) moveRing(selectHanger, 2);
		}
	}
}

void timercallback(TimerID timer) {
	if (timer == tempTimer) {
		stopTimer(tempTimer);
		setTimer(tempTimer, 1.0f);
		startTimer(tempTimer);
		setTimer(gameTimer, ++gameSecond);
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mousecallback);
	setTimerCallback(timercallback);

	locateObject(homeEnd, home, 666, 150);
	locateObject(howToPlay, home, 161, 91);
	locateObject(howToPlayButton, home, 515, 150);

	locateObject(ringCount3, home, 190, 300);
	locateObject(ringCount4, home, 340, 300);
	locateObject(ringCount5, home, 490, 300);
	locateObject(ringCount6, home, 640, 300);
	locateObject(ringCount7, home, 790, 300);
	locateObject(ringCount8, home, 940, 300);

	locateObject(box0, game, 46, 26);
	locateObject(box1, game, 451, 26);
	locateObject(box2, game, 856, 26);

	locateObject(hanger0, game, 46, 26);
	locateObject(hanger1, game, 451, 26);
	locateObject(hanger2, game, 856, 26);

	locateObject(clear, home, 0, 0);

	showObject(box0);
	showObject(box1);
	showObject(box2);
	showObject(homeEnd);
	showObject(howToPlayButton);

	showObject(ringCount3);
	showObject(ringCount4);
	showObject(ringCount5);
	showObject(ringCount6);
	showObject(ringCount7);
	showObject(ringCount8);

	//답안 출력
	for (int i = 2; i < 8; i++) {
		count = 0;
		hanoi(1, 2, 3, i + 1);
		printf(" if (ringCount == %d)\n\n", i + 1);
		for (int i = 0; i < count; i++) {
			printf(" %d %d |", arr[i][0], arr[i][1]);
			if (i % 10 == 9) printf("\n");
		}
		printf("\n\n");
	}

	startGame(home);

	return 0;
}