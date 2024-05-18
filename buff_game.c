#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> // 난수 생성
#include <malloc.h>

#define MAX_BUFF_STACK 11 // 최대 버프 사용가능 횟수
#define MAX_MONSTER_QUEUE 4 // 최대 몬스터 큐(스테이지당 3마리가 출현하고 원형 큐로 이루어져 있기 때문에 4로 설정)

// 구조체 선언
typedef struct buff_stack {//스택 구조체
	int top;
	int* Stack;
}Buff_Stack;
typedef struct player { //플레이어 구조체
	int hp;
}Player;
typedef struct Monster{ // 몬스터 구조체
	int hp; // 몬스터 체력
	char name[15]; // 몬스터 이름
	int max_buff; // 몬스터가 사용가능한 버프 횟수
}Monster;
typedef struct monster_queue { // 몬스터 큐 구조체
	int rear; 
	int front;
	Monster* QueueElements;
}Monster_Queue;

//생성 및 초기화 함수들
Buff_Stack* create_buff_stack() { // 버프 스택을 생성하는 함수
	Buff_Stack* buff = (Buff_Stack*)malloc(sizeof(Buff_Stack)); // 동적 할당
	memset(buff, 0, sizeof(Buff_Stack)); // 초기화
	buff->Stack = (int*)malloc(sizeof(MAX_BUFF_STACK)); // 스택 변수에 최대 버프 사용가능 횟수만큼 동적할당
	memset(buff->Stack, 0, sizeof(int)); //스택 초기화
	buff->top = -1; // top초기화

	return buff; //buff주소값 리턴
}
Monster_Queue* create_monster_queue() { //몬스터 큐 생성하는 함수
	Monster_Queue* monster_queue = (Monster_Queue*)malloc(sizeof(Monster_Queue)); // 동적할당
	memset(monster_queue, 0, sizeof(Monster_Queue)); // 초기화
	monster_queue->QueueElements = (Monster*)malloc(sizeof(MAX_MONSTER_QUEUE)); // 큐 변수에 최대 몬스터 큐사이즈 만큼 동적할당
	memset(monster_queue->QueueElements, 0, sizeof(Monster)); // 큐 초기화
	monster_queue->rear = 0;
	monster_queue->front = 0;

	return monster_queue; // 몬스터 큐 리턴
}

// 버프 스택 관련 함수
int Stack_Is_Empty(Buff_Stack* buff_stack) { // top이 -1이면 빈 스택
	return buff_stack->top == -1;
}
int Stack_Is_Full(Buff_Stack* buff_stack) { // top이 최대 스택이면 가득 찬 스택
	return buff_stack->top == MAX_BUFF_STACK;
}
int buff_stack_pop(Buff_Stack* buff_stack) { // 스택에서 pop하는 함수
	if (Stack_Is_Empty(buff_stack) == 1) {
		return 0;
	}
	return buff_stack->Stack[(buff_stack->top)--];
}
void buff_stack_push(Buff_Stack* buff_stack, int buff) {
	if (Stack_Is_Full(buff_stack) == 1) {
		printf("버프를 사용할 수 있는 최대 횟수를 넘었습니다.");
	}
	else {
		buff_stack->Stack[++(buff_stack->top)] = buff;
	}
}

// 몬스터 큐 관련 함수
int Moster_Is_Empty(Monster_Queue* monster_queue) {
	return (monster_queue->rear == monster_queue->front);
}
int Monster_Is_Full(Monster_Queue* monster_queue) {
	return ((monster_queue->rear + 1) % MAX_MONSTER_QUEUE == monster_queue->front);
}
void Monster_enqueue(Monster_Queue* monster_queue, Monster monster) {
	if (Monster_Is_Full(monster_queue) == 1) {
		printf("몬스터 큐가 가득 찼습니다.\n");
	}
	monster_queue->rear = (monster_queue->rear + 1) % MAX_MONSTER_QUEUE;
	monster_queue->QueueElements[monster_queue->rear] = monster;
}
Monster monster_dequeue(Monster_Queue* monster_queue) {
	if (Moster_Is_Empty(monster_queue) == 1) {
		printf("몬스터 큐에 몬스터가 없습니다.");
	}
	monster_queue->front = (monster_queue->front + 1) % MAX_MONSTER_QUEUE;
	return monster_queue->QueueElements[monster_queue->front];

}

//스테이지에 맞춰 몬스터의 종류와 정보들을 담는 함수들
void stage1_monster_Queue(Monster_Queue* monster_queue) {
	Monster slime;
	Monster goblin;

	slime.hp = 1;
	goblin.hp = 2;
	strcpy(slime.name, "슬라임");
	strcpy(goblin.name, "고블린");
	slime.max_buff = 2;
	goblin.max_buff = 3;

	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, slime);
	}
	Monster_enqueue(monster_queue, goblin);

}
void stage2_monster_Queue(Monster_Queue* monster_queue) {
	Monster slime;
	Monster goblin;

	slime.hp = 1;
	goblin.hp = 2;
	strcpy(slime.name, "슬라임");
	strcpy(goblin.name, "고블린");
	slime.max_buff = 2;
	goblin.max_buff = 3;

	Monster_enqueue(monster_queue, slime);
	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, goblin);
	}
}
void stage3_monster_Queue(Monster_Queue* monster_queue) {
	Monster Oak;
	Monster goblin;

	Oak.hp = 3;
	goblin.hp = 2;
	strcpy(Oak.name, "오크");
	strcpy(goblin.name, "고블린");
	Oak.max_buff = 4;
	goblin.max_buff = 3;

	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, goblin);
	}
	Monster_enqueue(monster_queue, Oak);

}
void stage4_monster_Queue(Monster_Queue* monster_queue) {
	Monster Oak;
	Monster Dracula;

	Oak.hp = 3;
	Dracula.hp = 4;
	strcpy(Oak.name, "오크");
	strcpy(Dracula.name, "드라큘라 백작");
	Oak.max_buff = 4;
	Dracula.max_buff = 5;

	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, Oak);
	}
	Monster_enqueue(monster_queue, Dracula);
}
void stage5_monster_Queue(Monster_Queue* monster_queue) {
	Monster Devil_King;
	Monster Dracula;

	Devil_King.hp = 5;
	Dracula.hp = 4;
	strcpy(Devil_King.name, "마왕");
	strcpy(Dracula.name, "드라큘라 백작");
	Devil_King.max_buff = 10;
	Dracula.max_buff = 5;

	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, Dracula);
	}
	Monster_enqueue(monster_queue, Devil_King);
}

//게임 시작 함수
int stage(int stage_level) { // 게임 시작
	Player player; // 변수 선언
	Monster monster;
	Monster_Queue* monster_queue = create_monster_queue();
	Buff_Stack* monster_buff_stack = create_buff_stack();
	Buff_Stack* player_buff_stack = create_buff_stack();

	int count = 0; // 사용자가 알맞은 버프를 사용하는지 확인 하는 변수
	int use_skill;
	srand(time(0)); //현재 시간을 기반으로 난수 생성

	if (stage_level == 1) { // 스테이지에 맞춰 몬스터 큐에 몬스터 삽입
		printf("좋아 준비는 다들 잘했겠지? 그럼 출발하자!\n");
		stage1_monster_Queue(monster_queue);
		player.hp = 2;
	}
	else if (stage_level == 2) {
		printf("순조로운 토벌이네 이 느낌 그대로~~\n");
		stage2_monster_Queue(monster_queue);
		player.hp = 3;
	}
	else if (stage_level == 3) {
		printf("전보단 확실히 어두워진 것 같네 모두 발밑 조심해...\n");
		stage3_monster_Queue(monster_queue);
		player.hp = 4;
	}
	else if (stage_level == 4) {
		printf("위험한 몬스터 천지잖아...! 모두 만만의 준비를 해두는게 좋을 껏 같애...\n");
		stage4_monster_Queue(monster_queue);
		player.hp = 5;
	}
	else if (stage_level == 5) {
		printf("이 앞에 엄청난 기운이 느껴져 우리의 모험이 끝에 도달한 것 같애...!");
		stage5_monster_Queue(monster_queue);
		player.hp = 5;
	}
	while (Moster_Is_Empty(monster_queue) == 0) {
		printf("용사파티가 이동합니다.\n");
		Sleep(1000);
		monster = monster_dequeue(monster_queue); // 몬스터 출현
		printf("%s 등장!\n", monster.name);

		while (monster.hp != 0) { // 몬스터의 체력이 0이 될때 까지 반복
			for (int i = 0; i < monster.max_buff; i++) { // 몬스터의 최대 버프 사용 가능 버프 사용
				int random_monster_buff = (rand() % monster.max_buff) + 1; //몬스터가 사용할 수 있는 버프에 맞춰 랜덤 버프 사용
				if (random_monster_buff == 1) {
					printf("%s은(는) 단단해지기를 사용하려 한다!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 2) {
					printf("%s은(는) 회복을 하려한다!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 3) {
					printf("%s은(는) 공격을 하려한다!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 4) {
					printf("%s은(는) 독을 사용하려 한다!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 5) {
					printf("%s은(는) 흡혈을 사용하려 한다!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
			}
			printf("위험해! 적이 사용하려한 스킬에 대비해 버프 스킬을 사용하자!\n");
			for (int i = 0; i < monster.max_buff; i++) { // 플레이어 버프 입력
				use_skill = 0;
				printf("1. 방깎, 2. 저주, 3. 방어, 4. 해독, 5. 수혈");
				scanf("%d", &use_skill);
				buff_stack_push(player_buff_stack, use_skill);
			}
			for (int i = 0; i < monster.max_buff; i++) {
				if ((buff_stack_pop(monster_buff_stack) == buff_stack_pop(player_buff_stack))) {
					count++;
				}
			}
			if (count == monster.max_buff) {
				printf("잘했어! 몬스터가 방심한 사이 공격하자!\n");
				monster.hp--;
				if (monster.hp == 0) {
					printf("%s은(는) 쓰러졌다.\n", monster.name);
					printf("좋아 몬스터를 쓰러트렸으니깐 앞으로 나아가자.\n");
				}
				Sleep(1000);
				system("cls");
			} // 문제 발생
			else {
				printf("실수한 것 같아 어떻하지... 어떻하지....\n");
				printf("으악!\n");
				player.hp--;
				if (player.hp == 0) {
					free(monster_queue);
					free(monster_buff_stack);
					free(player_buff_stack);
					return 0;
				}
				else {
					printf("용사의 체력이 감소했습니다\n");
					printf("남은 체력 : %d\n", player.hp);
					Sleep(2000);
					system("cls");
				}
			}
			count = 0;
		}
	}
	printf("좋아 모든 몬스터를 쓰러트린 것 같군 다음 층으로 넘어갈까?\n");
	free(monster_queue);
	free(monster_buff_stack);
	free(player_buff_stack);
	return 1;
}
void win() {
	printf("마왕을 무찔렀습니다!");
	Sleep(1000);
}
void lose() {
	printf("몬스터에게 패배해 용사가 죽었습니다...\n다음 용사를 찾으러 가볼까요.....?\n");
	Sleep(1000);
}
void stage_judgement() { //  스테이지와 플레이어의 승패를 구분하는 함수
	int stage_jment = 1; // 현재 스테이지 판단 처음 시작 시 스테이지 1
	int next_stage = 0; // 다음 스테이지로 넘어갈지 판단하는 변수
	while (1) {
		if (stage_jment == 1) {
			next_stage = stage(1);
			if (next_stage == 1) {
				stage_jment += 1;
			}
			else {
				lose();
				break;
			}
		}
		else if (stage_jment == 2) {
			next_stage = stage(2);
			if (next_stage == 1) {
				stage_jment += 1;
			}
			else {
				lose();
				break;
			}
		}
		else if (stage_jment == 3) {
			next_stage = stage(3);
			if (next_stage == 1) {
				stage_jment += 1;
			}
			else {
				lose();
				break;
			}
		}
		else if (stage_jment == 4) {
			next_stage = stage(4);
			if (next_stage == 1) {
				stage_jment += 1;
			}
			else {
				lose();
				break;
			}
		}
		else if (stage_jment == 5) {
			next_stage = stage(5);
			if (next_stage == 1) {
				win();
				break;
			}
			else {
				lose();
				break;
			}
		}
	}
}
void game_story() {
	printf("게임의 스토리는 이렇습니다!\n");
	Sleep(1000);
}
void game_rule() {
	printf("게임의 룰은 이렇습니다!\n");
	Sleep(1000);
}

int main(void) {
	int choice;
	while (TRUE) {
		printf("골라 주세요 1.게임 시작, 2. 게임 스토리, 3. 게임 룰, 4. 게임 종료");
		scanf("%d", &choice);
		if (choice == 1) {
			stage_judgement();
		}
		else if (choice == 2) {
			game_story();
		}
		else if (choice == 3) {
			game_rule();
		}
		else if (choice == 4) {
			break;
		}
		else {
			printf("올바른 번호를 입력해주세요\n");
		}
	}
	return 0;
}