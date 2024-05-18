#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> // ���� ����
#include <malloc.h>

#define MAX_BUFF_STACK 11 // �ִ� ���� ��밡�� Ƚ��
#define MAX_MONSTER_QUEUE 4 // �ִ� ���� ť(���������� 3������ �����ϰ� ���� ť�� �̷���� �ֱ� ������ 4�� ����)

// ����ü ����
typedef struct buff_stack {//���� ����ü
	int top;
	int* Stack;
}Buff_Stack;
typedef struct player { //�÷��̾� ����ü
	int hp;
}Player;
typedef struct Monster{ // ���� ����ü
	int hp; // ���� ü��
	char name[15]; // ���� �̸�
	int max_buff; // ���Ͱ� ��밡���� ���� Ƚ��
}Monster;
typedef struct monster_queue { // ���� ť ����ü
	int rear; 
	int front;
	Monster* QueueElements;
}Monster_Queue;

//���� �� �ʱ�ȭ �Լ���
Buff_Stack* create_buff_stack() { // ���� ������ �����ϴ� �Լ�
	Buff_Stack* buff = (Buff_Stack*)malloc(sizeof(Buff_Stack)); // ���� �Ҵ�
	memset(buff, 0, sizeof(Buff_Stack)); // �ʱ�ȭ
	buff->Stack = (int*)malloc(sizeof(MAX_BUFF_STACK)); // ���� ������ �ִ� ���� ��밡�� Ƚ����ŭ �����Ҵ�
	memset(buff->Stack, 0, sizeof(int)); //���� �ʱ�ȭ
	buff->top = -1; // top�ʱ�ȭ

	return buff; //buff�ּҰ� ����
}
Monster_Queue* create_monster_queue() { //���� ť �����ϴ� �Լ�
	Monster_Queue* monster_queue = (Monster_Queue*)malloc(sizeof(Monster_Queue)); // �����Ҵ�
	memset(monster_queue, 0, sizeof(Monster_Queue)); // �ʱ�ȭ
	monster_queue->QueueElements = (Monster*)malloc(sizeof(MAX_MONSTER_QUEUE)); // ť ������ �ִ� ���� ť������ ��ŭ �����Ҵ�
	memset(monster_queue->QueueElements, 0, sizeof(Monster)); // ť �ʱ�ȭ
	monster_queue->rear = 0;
	monster_queue->front = 0;

	return monster_queue; // ���� ť ����
}

// ���� ���� ���� �Լ�
int Stack_Is_Empty(Buff_Stack* buff_stack) { // top�� -1�̸� �� ����
	return buff_stack->top == -1;
}
int Stack_Is_Full(Buff_Stack* buff_stack) { // top�� �ִ� �����̸� ���� �� ����
	return buff_stack->top == MAX_BUFF_STACK;
}
int buff_stack_pop(Buff_Stack* buff_stack) { // ���ÿ��� pop�ϴ� �Լ�
	if (Stack_Is_Empty(buff_stack) == 1) {
		return 0;
	}
	return buff_stack->Stack[(buff_stack->top)--];
}
void buff_stack_push(Buff_Stack* buff_stack, int buff) {
	if (Stack_Is_Full(buff_stack) == 1) {
		printf("������ ����� �� �ִ� �ִ� Ƚ���� �Ѿ����ϴ�.");
	}
	else {
		buff_stack->Stack[++(buff_stack->top)] = buff;
	}
}

// ���� ť ���� �Լ�
int Moster_Is_Empty(Monster_Queue* monster_queue) {
	return (monster_queue->rear == monster_queue->front);
}
int Monster_Is_Full(Monster_Queue* monster_queue) {
	return ((monster_queue->rear + 1) % MAX_MONSTER_QUEUE == monster_queue->front);
}
void Monster_enqueue(Monster_Queue* monster_queue, Monster monster) {
	if (Monster_Is_Full(monster_queue) == 1) {
		printf("���� ť�� ���� á���ϴ�.\n");
	}
	monster_queue->rear = (monster_queue->rear + 1) % MAX_MONSTER_QUEUE;
	monster_queue->QueueElements[monster_queue->rear] = monster;
}
Monster monster_dequeue(Monster_Queue* monster_queue) {
	if (Moster_Is_Empty(monster_queue) == 1) {
		printf("���� ť�� ���Ͱ� �����ϴ�.");
	}
	monster_queue->front = (monster_queue->front + 1) % MAX_MONSTER_QUEUE;
	return monster_queue->QueueElements[monster_queue->front];

}

//���������� ���� ������ ������ �������� ��� �Լ���
void stage1_monster_Queue(Monster_Queue* monster_queue) {
	Monster slime;
	Monster goblin;

	slime.hp = 1;
	goblin.hp = 2;
	strcpy(slime.name, "������");
	strcpy(goblin.name, "���");
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
	strcpy(slime.name, "������");
	strcpy(goblin.name, "���");
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
	strcpy(Oak.name, "��ũ");
	strcpy(goblin.name, "���");
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
	strcpy(Oak.name, "��ũ");
	strcpy(Dracula.name, "���ŧ�� ����");
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
	strcpy(Devil_King.name, "����");
	strcpy(Dracula.name, "���ŧ�� ����");
	Devil_King.max_buff = 10;
	Dracula.max_buff = 5;

	for (int i = 0; i < 2; i++) {
		Monster_enqueue(monster_queue, Dracula);
	}
	Monster_enqueue(monster_queue, Devil_King);
}

//���� ���� �Լ�
int stage(int stage_level) { // ���� ����
	Player player; // ���� ����
	Monster monster;
	Monster_Queue* monster_queue = create_monster_queue();
	Buff_Stack* monster_buff_stack = create_buff_stack();
	Buff_Stack* player_buff_stack = create_buff_stack();

	int count = 0; // ����ڰ� �˸��� ������ ����ϴ��� Ȯ�� �ϴ� ����
	int use_skill;
	srand(time(0)); //���� �ð��� ������� ���� ����

	if (stage_level == 1) { // ���������� ���� ���� ť�� ���� ����
		printf("���� �غ�� �ٵ� ���߰���? �׷� �������!\n");
		stage1_monster_Queue(monster_queue);
		player.hp = 2;
	}
	else if (stage_level == 2) {
		printf("�����ο� ����̳� �� ���� �״��~~\n");
		stage2_monster_Queue(monster_queue);
		player.hp = 3;
	}
	else if (stage_level == 3) {
		printf("������ Ȯ���� ��ο��� �� ���� ��� �߹� ������...\n");
		stage3_monster_Queue(monster_queue);
		player.hp = 4;
	}
	else if (stage_level == 4) {
		printf("������ ���� õ���ݾ�...! ��� ������ �غ� �صδ°� ���� �� ����...\n");
		stage4_monster_Queue(monster_queue);
		player.hp = 5;
	}
	else if (stage_level == 5) {
		printf("�� �տ� ��û�� ����� ������ �츮�� ������ ���� ������ �� ����...!");
		stage5_monster_Queue(monster_queue);
		player.hp = 5;
	}
	while (Moster_Is_Empty(monster_queue) == 0) {
		printf("�����Ƽ�� �̵��մϴ�.\n");
		Sleep(1000);
		monster = monster_dequeue(monster_queue); // ���� ����
		printf("%s ����!\n", monster.name);

		while (monster.hp != 0) { // ������ ü���� 0�� �ɶ� ���� �ݺ�
			for (int i = 0; i < monster.max_buff; i++) { // ������ �ִ� ���� ��� ���� ���� ���
				int random_monster_buff = (rand() % monster.max_buff) + 1; //���Ͱ� ����� �� �ִ� ������ ���� ���� ���� ���
				if (random_monster_buff == 1) {
					printf("%s��(��) �ܴ������⸦ ����Ϸ� �Ѵ�!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 2) {
					printf("%s��(��) ȸ���� �Ϸ��Ѵ�!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 3) {
					printf("%s��(��) ������ �Ϸ��Ѵ�!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 4) {
					printf("%s��(��) ���� ����Ϸ� �Ѵ�!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
				else if (random_monster_buff == 5) {
					printf("%s��(��) ������ ����Ϸ� �Ѵ�!\n", monster.name);
					buff_stack_push(monster_buff_stack, random_monster_buff);
					Sleep(100);
				}
			}
			printf("������! ���� ����Ϸ��� ��ų�� ����� ���� ��ų�� �������!\n");
			for (int i = 0; i < monster.max_buff; i++) { // �÷��̾� ���� �Է�
				use_skill = 0;
				printf("1. ���, 2. ����, 3. ���, 4. �ص�, 5. ����");
				scanf("%d", &use_skill);
				buff_stack_push(player_buff_stack, use_skill);
			}
			for (int i = 0; i < monster.max_buff; i++) {
				if ((buff_stack_pop(monster_buff_stack) == buff_stack_pop(player_buff_stack))) {
					count++;
				}
			}
			if (count == monster.max_buff) {
				printf("���߾�! ���Ͱ� ����� ���� ��������!\n");
				monster.hp--;
				if (monster.hp == 0) {
					printf("%s��(��) ��������.\n", monster.name);
					printf("���� ���͸� ����Ʈ�����ϱ� ������ ���ư���.\n");
				}
				Sleep(1000);
				system("cls");
			} // ���� �߻�
			else {
				printf("�Ǽ��� �� ���� �����... �����....\n");
				printf("����!\n");
				player.hp--;
				if (player.hp == 0) {
					free(monster_queue);
					free(monster_buff_stack);
					free(player_buff_stack);
					return 0;
				}
				else {
					printf("����� ü���� �����߽��ϴ�\n");
					printf("���� ü�� : %d\n", player.hp);
					Sleep(2000);
					system("cls");
				}
			}
			count = 0;
		}
	}
	printf("���� ��� ���͸� ����Ʈ�� �� ���� ���� ������ �Ѿ��?\n");
	free(monster_queue);
	free(monster_buff_stack);
	free(player_buff_stack);
	return 1;
}
void win() {
	printf("������ ���񷶽��ϴ�!");
	Sleep(1000);
}
void lose() {
	printf("���Ϳ��� �й��� ��簡 �׾����ϴ�...\n���� ��縦 ã���� �������.....?\n");
	Sleep(1000);
}
void stage_judgement() { //  ���������� �÷��̾��� ���и� �����ϴ� �Լ�
	int stage_jment = 1; // ���� �������� �Ǵ� ó�� ���� �� �������� 1
	int next_stage = 0; // ���� ���������� �Ѿ�� �Ǵ��ϴ� ����
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
	printf("������ ���丮�� �̷����ϴ�!\n");
	Sleep(1000);
}
void game_rule() {
	printf("������ ���� �̷����ϴ�!\n");
	Sleep(1000);
}

int main(void) {
	int choice;
	while (TRUE) {
		printf("��� �ּ��� 1.���� ����, 2. ���� ���丮, 3. ���� ��, 4. ���� ����");
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
			printf("�ùٸ� ��ȣ�� �Է����ּ���\n");
		}
	}
	return 0;
}