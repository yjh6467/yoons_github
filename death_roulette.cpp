#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <string>
using namespace std;

#define MAX_BULLET_SIZE 10

class game_interface {
public:
	static int start();	// ó�� ������ ���������� �������� �������̽�
	static void game_start();	// ���ӽ���
	static void game_inf();	// ���� ���� �����ִ� �������̽�
};
class Stack {	// �Ѿ��� ���� �������� �ױ� ���� ���� Ŭ���� ����
	int top;	// ������ ���� �Ѿ��� ��ġ�� ����Ű�� ����
	bool* bullet_stack; // �Ѿ��� ����
public:
	Stack() { this->top = -1, this->bullet_stack = new bool[MAX_BULLET_SIZE]; } // �ִ� 10�� ź�� â��
	bool pop();
	bool peek();
	void push(bool bullet);
	bool is_empty();
	bool is_full();
};
class player {
	int hp;	// �÷��̾��� hp�� �����ϴ� ����
	bool player_turn;	// �÷��̾��� ������ Ȯ�� �ϴ� ����
	item* player_item;	// itemŬ������ ������ �÷��̾��� ������ ����
public:
	player() { this->hp = 0, player_turn = false, this->player_item = new item; } // �÷��̾� ������
	static int maxhp;
	static void hp_setting(player* p1, player* p2);	// �÷��̾��� hp�� �ʱ� maxhp�� �����ϴ� �Լ�
	void adjustment_hp(player* player, bool plus_minus);
	void adjustment_player_turn(player* player, bool plus_minus);
	void show_hp();
	static void show_maxhp();	// �÷��̾���� �ִ� ü���� �����ֱ�
	void show_player_item();// ���� �÷��̾ �����ϰ� �ִ� �������� ������ �����ִ� �Լ�
	friend void game_interface::game_start(); // ���ӿ��� �÷��̾� ��ü�� �Լ��� ������ �� �ֵ��� ������ ����
};
class item { // ������ Ŭ����
	int item1_count;	// 1�� �������� ������ �����ϴ� ����
	int item2_count;	// 2�� �������� ������ �����ϴ� ����
	int item3_count;	// 3�� �������� ������ �����ϴ� ����
public:
	item() { item1_count = 0; item2_count = 0; item3_count = 0; }	// ó�� ��ü ������ �������� ������ ���� 0��
	void show_item();	//�������� ������ �����ִ� �Լ�
	bool check_item(int num); // ���õ� �������� �ִ��� Ȯ���ϴ� �Լ�
	void use_item(player* player, int num);
	void adjustment_item(int item_num, bool plus_minus); // �������� ������ ������ �� ����ϴ� �Լ�
};
class gun {
	int real_bullet_count;	// ��ź�� ������ �����ϴ� ����
	int fake_bullet_count;	// ����ź�� ������ �����ϴ� ����
	Stack* magazine;	// �������� �����Ǿ��ִ� ź��
public:
	gun() { this->real_bullet_count = 0, this->fake_bullet_count = 0, this->magazine = new Stack; } // �� ��ü ������
	void reload(); //�Ѿ� ����
	void show_magazine(); // źâ�� ����ִ� źȯ�� �����ִ� �Լ�
	void self_shoot(player* who_player, player *to_player, bool &turn_skip);
	void to_shoot(player* who_player, player* to_player, bool& turn_skip);
	friend void game_interface::game_start(); // ���ӿ��� �Ѱ�ü�� �Լ��� ������ �� �ֵ��� ������ ����
};

// ���� ���� �Լ� ����
bool Stack::is_empty() { // źâ�� ��������� false�� ��ȯ�ϴ� �Լ�
	return top == -1;	//top�� -1�̸� ����ִ� źâ
}
bool Stack::is_full() { // źâ�� ���������� false�� ��ȯ�ϴ� �Լ�
	return top >= MAX_BULLET_SIZE; // �ִ� źâ ������� top�� ũ�ų� ������ Ǯ
}
bool Stack::pop() { // �Ѿ��� �߻��ϴ� �Լ�
	return bullet_stack[top--];
}
bool Stack::peek() { // �Ѿ��� Ȯ�ξ������� ������� �� ȣ���ϴ� �Լ�
	return bullet_stack[top]; // ���� �Ѿ� ����
}
void Stack::push(bool bul) { // �Ѿ��� �����ϴ� �Լ�
	this->bullet_stack[++top] = bul;	//top�� ++�ѵ� top ��ġ�� �Ѿ� ����
}

// �� ���� �Լ� ����
void gun::reload() { // ���� �����ϴ� �Լ�
	srand((unsigned int)time(NULL)); // ���� ���� ���� rand()�� ���
	real_bullet_count = (rand() % 5) + 1; // 1������ 5���� źȯ ����
	fake_bullet_count = (rand() % 5) + 1;
	for (int i = 0; i < real_bullet_count; i++) { // ��ź�� ���� ��ŭ ����
		magazine->push(true);
	}
	for (int j = 0; j < fake_bullet_count; j++) { // ����ź�� ���� ��ŭ ����
		magazine->push(false);
	}
}
void gun::show_magazine() { // ���� �����Ǿ��ִ� �Ѿ��� ������ �����ִ� �Լ�
	cout << "��ź : " << real_bullet_count << "��\n";
	cout << "����ź : " << fake_bullet_count << "��\n";
}
void gun::self_shoot(player* who_player, player* to_player, bool &turn_skip){
	if (magazine->pop() == true) { // ������ ���� ��ź
		cout << "������ �Ѿ��� ��ź�̿����ϴ�\n";
		Sleep(1000);
		who_player->adjustment_hp(who_player, false);	// ���� �÷��̾� ü�� -1
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // �� ��ŵ�� ���� ��� �ڽ��� �� �ѹ���
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false); // �ƴ� ��� ��������� �Ѿ
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
	else if (magazine->pop() == false) { // ������ ���� ��ź�� �ƴ� ���
		cout << "������ �Ѿ��� ����ź�̿����ϴ�.\n";
		Sleep(1000);
		who_player->adjustment_player_turn(who_player, true); // �ڽ��� �� ���
		to_player->adjustment_player_turn(to_player, false);
		turn_skip = false;
	}
}
void gun::to_shoot(player* who_player, player* to_player, bool &turn_skip) {
	if (magazine->pop() == true) { // ������ ���� ��ź
		cout << "������ �Ѿ��� ��ź�̿����ϴ�\n";
		Sleep(1000);
		to_player->adjustment_hp(to_player, false);	// ��� �÷��̾� ü�� -1
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // �� ��ŵ�� ���� ��� �ڽ��� �� �ѹ���
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false);; // �ƴ� ��� ��������� �Ѿ
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
	else if (magazine->pop() == false) { // ������ ���� ��ź�� �ƴ� ���
		cout << "������ �Ѿ��� ����ź�̿����ϴ�.\n";
		Sleep(1000);
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // �� ��ŵ�� ���� ��� �ڽ��� �� �ѹ���
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false);; // �ƴ� ��� ��������� �Ѿ
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
}

//������ ���� �Լ� ����
void item::show_item() { //�����ϰ� �ִ� �������� ������ ����ϴ� �Լ�
	cout << "hot6 ���� : " << item1_count << "��\n";
	cout << "slap ���� : " << item2_count << "��\n";
	cout << "strn ���� : " << item3_count << "��\n";
}
bool item::check_item(int num) { //�ش� ��ȣ�� �������� �����ϰ� �ִ��� Ȯ���ϴ� �Լ�
	if (num == 1 and item1_count > 0) {
		return true;
	}
	else if (num == 2 and item2_count > 0) {
		return true;
	}
	else if (num == 3 and item3_count > 0) {
		return true;
	}
	else {
		return false;
	}
}
void item::use_item(player* player, int num) {
	if (num == 1 and player->player_item->check_item(1) == true) {
		cout << "hp�� ȸ���Ǿ����ϴ�.\n";
		p2->hp++;
		p2->player_item->adjustment_item(1, false);
	}
	else if (choice2 == 2 and p2->player_item->check_item(2) == true) {
		if (a_gun->magazine->peek() == true) {
			cout << "���� �����Ǿ��ִ� źȯ�� ��ź �Դϴ�\n";
		}
		else {
			cout << "���� �����Ǿ��ִ� źȯ�� ����ź �Դϴ�\n";
		}
		p2->player_item->adjustment_item(2, false);
	}
	else if (choice2 == 3 and p2->player_item->check_item(3) == true) {
		cout << "����� �������� ���� �մϴ�.\n";
		turn_skip = true;
		p2->player_item->adjustment_item(3, false);
	}
	else {
		cout << "�߸� �Է��߰ų� ���� ������������ �����ϴ�.\n";
	}
}
void item::adjustment_item(int item_num, bool plus_minus) { // �÷��̾��� �������� �����ϴ� �Լ�
	if (plus_minus == true) {
		if (item_num == 1) {
			item1_count++;
		}
		else if (item_num == 2) {
			item2_count++;

		}
		else if (item_num == 3) {
			item3_count++;
		}
	}
	else if (plus_minus == false and item1_count > 0) {
		if (item_num == 1) {
			item1_count--;

		}
		else if (item_num == 2 and item3_count > 0) {
			item2_count--;

		}
		else if (item_num == 3 and item3_count > 0) {
			item3_count--;
		}
	}
}

//�÷��̾� ���� �Լ� ����
int player::maxhp = 0; // ����ƽ ������ maxhp�� ����ϱ� ����
void player::hp_setting(player* p1, player* p2) {
	srand((unsigned int)time(NULL));
	maxhp = (rand() + 1) % 5 + 1;
	p1->hp = maxhp;
	p2->hp = p1->hp;
}
void player::adjustment_hp(player* player, bool plus_minus) {
	if (plus_minus == true) {
		player->hp++;
	}
	else if (plus_minus == false) {
		player->hp--;
	}
}
void player::adjustment_player_turn(player* player, bool plus_minus) {
	if (plus_minus == true) {
		player->player_turn = true;
	}
	else if (plus_minus == false) {
		player->player_turn = false;
	}
}
void player::show_maxhp() { // �÷��̾���� �ִ� ü���� �����ִ� �Լ�
	cout << "���� �÷��̾��� �ִ� ü����" << maxhp << "�Դϴ�.\n";
}
void player::show_hp() { // ���� �÷��̾��� ü���� �����ִ� �Լ�
	cout << "���� �÷��̾��� ü����" << hp << "�Դϴ�.\n";
}
void player::show_player_item() { //���� �÷��̾ ������ �������� ������ �����ִ� �Լ�
	cout << "���� �÷��̾ ������ �������� ������\n";
	player_item->show_item();
}

//�������̽� ���� �Լ� ����
int game_interface::start() { // ������ �������� �� �÷��̾�� ���� ����, ���� ��, ���� ���Ḧ ���� �޴� �Լ�
	cout << "----------------------------------------------\n";
	cout << "|  1. ���ӽ��� 2. ���� ����   3. ���� ����   |\n";
	cout << "----------------------------------------------\n";
	int choice;
	cin >> choice;

	if (choice == 1) {
		return 1;
	}
	else if (choice == 2) {
		return 2;
	}
	else if (choice == 3) {
		return 3;
	}
	else {
		return 0;
	}
}
void game_interface::game_inf() { // ������ ���� �����ִ� �Լ�
	cout << "������ �������� �����帲\n";
	cout << "����Ű�� �Է��ϼ���\n";
	int ch = getc(stdin);
}
void game_interface::game_start() { // ���� ���� �Լ�
	player* p1 = new player;	// �÷��̾� ��ü ����
	player* p2 = new player;
	gun* a_gun = new gun;	// ���ο� �� ��ü ����
	bool turn_skip = false; //�÷��̾��� ���� ��ŵ���� ���� �����ϴ� ����
	srand((unsigned int)time(NULL)); // ���� ������ �����ϱ� ���� �Լ�
	player::hp_setting(p1, p2);// �÷��̾���� hp ����
	char ch; // ������ ���� �Ǿ��� �� �÷��̾�� ����Ű�� �Է� �ޱ� ���� ����

	player::show_maxhp();	// �ִ� ü�� ���
	while (p1->hp != 0 or p2->hp != 0) { // �� �÷��̾��� ��� ������ ü���� 0�� �ɶ����� �ݺ�
		for (int i = 1; i < 4; i++) {
			p1->player_item->adjustment_item(i, rand() % 2);	// �÷��̾��� �������� �������� 0������ 1�� �߰�
			p2->player_item->adjustment_item(i, rand() % 2);
		}
		p1->player_turn = rand() % 2;	// �÷��̾��� ���� ���ʸ� �������� ����
		if (p1->player_turn == 1) {
			p2->player_turn = 0;
		}
		else {
			p2->player_turn = 1;
		}

		cout << "�Ѿ��� ���� �մϴ�.\n";
		a_gun->reload(); // �� ������
		Sleep(1000);
		a_gun->show_magazine(); // �Ѿ˿� ������ ��ź�� ����ź�� ���� ���

		while (a_gun->magazine->is_empty() == false) {
			int choice = 0; // ���� ���� �������� ������� ���ϴ� ����
			int choice2 = 0; // ���� �ڽ��̳� ������� ����, � �������� �����ؼ� ���� �����ϴ� ����
			if (p1->player_turn == true and p2->player_turn == false) {
				cout << "player1�� �����Դϴ�\n";
				Sleep(1000);
				p1->show_player_item(); // �÷��̾ ������ �ִ� �������� ���� �����ֱ�
				p1->show_hp();	// �÷��̾��� ü���� �����ֱ�
				cout << "���� ���� �������� ������� �������ּ���\n";
				cout << "�� : 1 ������ : 2\n";
				cin >> choice;
				if (choice == 1) {
					cout << "�ڽſ��� ���� ���� ��뿡�� ���� ���� �������ּ���\n";
					cout << "�ڽ� : 1  ��� : 2\n";
					cin >> choice2;
					if (choice2 == 1) {
						a_gun->self_shoot(p1, p2, turn_skip);
						if (p1->hp == 0) {
							break;
						}
					}
					else if (choice2 == 2) {
						a_gun->to_shoot(p1, p2, turn_skip);
						if (p2->hp == 0) {
							break;
						}
					}
					else {
						cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
					}
				}
				else if (choice == 2) {
					cout << "����� �������� ��ȣ�� �������ּ���\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1 and p1->player_item->check_item(1) == true) {
						cout << "hp�� ȸ���Ǿ����ϴ�.\n";
						p1->hp++;
						p1->player_item->adjustment_item(1, false);
					}
					else if (choice2 == 2 and p1->maxhp < p1->hp and p1->player_item->check_item(2) == true) {
						if (a_gun->magazine->peek() == true) {
							cout << "���� �����Ǿ��ִ� źȯ�� ��ź �Դϴ�\n";
							Sleep(1000);
						}
						else {
							cout << "���� �����Ǿ��ִ� źȯ�� ����ź �Դϴ�\n";
							Sleep(1000);
						}
						p1->player_item->adjustment_item(2, false);
					}
					else if (choice2 == 3 and p1->player_item->check_item(3) == true) {
						cout << "����� �������� ���� �մϴ�.\n";
						Sleep(1000);
						turn_skip = true;
						p1->player_item->adjustment_item(3, false);
					}
					else {
						cout << "�߸� �Է��߰ų� ���� ������������ �����ϴ�.\n";
					}
				}
				else {
					cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
				}
			}
			else if (p2->player_turn == true and p1->player_turn == false) {
				cout << "player2�� �����Դϴ�\n";
				Sleep(1000);
				p2->show_player_item(); // �÷��̾ ������ �ִ� �������� ���� �����ֱ�
				p2->show_hp();	// �÷��̾��� ü���� �����ֱ�
				cout << "���� ���� �������� ������� �������ּ���\n";
				cout << "�� : 1 ������ : 2\n";
				cin >> choice;
				if (choice == 1) {
					cout << "�ڽſ��� ���� ���� ��뿡�� ���� ���� �������ּ���\n";
					cout << "�ڽ� : 1  ��� : 2\n";
					cin >> choice2;
					if (choice2 == 1) { // �ڽſ��� ��ٸ� �������� ���
						if (choice2 == 1) {
							a_gun->self_shoot(p2, p1, turn_skip);
							if (p2->hp == 0) {
								break;
							}
						}
						else if (choice2 == 2) {
							a_gun->to_shoot(p2, p1, turn_skip);
							if (p1->hp == 0) {
								break;
							}
						}
						else {
							cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
						}

					}
				}
				else if (choice == 2) {
					cout << "����� �������� ��ȣ�� �������ּ���\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1 and p2->player_item->check_item(1) == true) {
						cout << "hp�� ȸ���Ǿ����ϴ�.\n";
						p2->hp++;
						p2->player_item->adjustment_item(1, false);
					}
					else if (choice2 == 2 and p2->player_item->check_item(2) == true) {
						if (a_gun->magazine->peek() == true) {
							cout << "���� �����Ǿ��ִ� źȯ�� ��ź �Դϴ�\n";
						}
						else {
							cout << "���� �����Ǿ��ִ� źȯ�� ����ź �Դϴ�\n";
						}
						p2->player_item->adjustment_item(2, false);
					}
					else if (choice2 == 3 and p2->player_item->check_item(3) == true) {
						cout << "����� �������� ���� �մϴ�.\n";
						turn_skip = true;
						p2->player_item->adjustment_item(3, false);
					}
					else {
						cout << "�߸� �Է��߰ų� ���� ������������ �����ϴ�.\n";
					}
				}
				else {
					cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
				}
			}
			else {
				cout << "�� �� ���� ������ ������ �����մϴ�.";
				ch = cin.get();
				break;
			}
		}
	}
	if (p1->hp <= 0 and p2->hp > 0) {
		cout << "p2�� �¸��Ͽ����ϴ�.\n";
		ch = cin.get();
	}
	else if (p2->hp <= 0 and p1->hp > 0) {
		cout << "p1�� �¸��Ͽ����ϴ�.\n";
		ch = cin.get();
	}
	else if (p2->hp <= 0 and p1->hp <= 0) {
		cout << "���º� �Դϴ�\n";
		ch = cin.get();
	}
	else {
		cout << "�� �� ���� �ý��� ������ �߻��߽��ϴ�.";
		ch = cin.get();
	}
}
// ���� �Լ�
int main() {
	while (true) {
		int choice = game_interface::start();
		if (choice == 1) {
			game_interface::game_start();
		}
		else if (choice == 2) {
			game_interface::game_inf();
		}
		else if (choice == 3) {
			break;
		}
		else {
			cout << "1���� 3�� ���ڸ� ����ּ���\n";
		}
	}
}