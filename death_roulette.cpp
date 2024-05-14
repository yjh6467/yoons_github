#include <iostream>
#include <windows.h>
using namespace std;

#define MAX_BULLET_SIZE 10 // 

class player;
class game_interface {
public:
	static int start();	// ó�� ������ ���������� �������� �������̽�
	static void game_start();	// ���ӽ���
	static void game_inf();	// ���� ���� �����ִ� �������̽�
	static void waitForEnter(); //������� ����Ű �Է��� ��ٸ��� �Լ�
};
class Stack {	// �Ѿ��� ���� �������� �ױ� ���� ���� Ŭ���� ����
	int top;	// ������ ���� �Ѿ��� ��ġ�� ����Ű�� ����
	int* bullet_stack; // �Ѿ��� ����
public:
	Stack() { this->top = -1, this->bullet_stack = new int[MAX_BULLET_SIZE]; } // �ִ� 10�� ź�� â��
	int pop();
	int peek();
	void push(int bullet);
	bool is_empty();
	bool is_full();
};
class item { // ������ Ŭ����
	int item1_count;	// 1�� �������� ������ �����ϴ� ����
	int item2_count;	// 2�� �������� ������ �����ϴ� ����
	int item3_count;	// 3�� �������� ������ �����ϴ� ����
public:
	item() { item1_count = 0; item2_count = 0; item3_count = 0; }	// ó�� ��ü ������ �������� ������ ���� 0��
	void show_item();	//�������� ������ �����ִ� �Լ�
	bool check_item(int num); // ���õ� �������� �ִ��� Ȯ���ϴ� �Լ�
	void adjustment_item(int item_num, bool plus_minus); // �������� ������ ������ �� ����ϴ� �Լ�
};
class gun {
	int real_bullet_count;	// ��ź�� ������ �����ϴ� ����
	int fake_bullet_count;	// ����ź�� ������ �����ϴ� ����
	Stack* magazine;	// �������� �����Ǿ��ִ� ź��
public:
	gun() { this->real_bullet_count = 0, this->fake_bullet_count = 0, this->magazine = new Stack; } // �� ��ü ������
	void reload(gun* a_gun); //�Ѿ� ����
	void show_magazine(gun* a_gun); // źâ�� ����ִ� źȯ�� �����ִ� �Լ�
	void self_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip);
	void to_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip);
	int check_bullet(gun* gun);
	friend void game_interface::game_start(); // ���ӿ��� �Ѱ�ü�� �Լ��� ������ �� �ֵ��� ������ ����
};
class player {
	int hp;	// �÷��̾��� hp�� �����ϴ� ����
	bool player_turn;	// �÷��̾��� ������ Ȯ�� �ϴ� ����
	item* player_item;	// itemŬ������ ������ �÷��̾��� ������ ����
public:
	player() { this->hp = 0, player_turn = false, this->player_item = new item; } // �÷��̾� ������
	static int maxhp; //�ִ� ü�� ����
	static void hp_setting(player* p1, player* p2);	// �÷��̾��� hp�� �ʱ� maxhp�� �����ϴ� �Լ�
	void adjustment_hp(player* player, bool plus_minus);
	void adjustment_player_turn(player* player, bool plus_minus);
	void use_item1(player* player);
	void use_item2(player* player, gun* a_gun);
	void use_item3(player* player, bool& turn_skip);
	void show_hp();
	static void show_maxhp();	// �÷��̾���� �ִ� ü���� �����ֱ�
	void show_player_item();// ���� �÷��̾ �����ϰ� �ִ� �������� ������ �����ִ� �Լ�
	friend void game_interface::game_start(); // ���ӿ��� �÷��̾� ��ü�� �Լ��� ������ �� �ֵ��� ������ ����
};

// ���� ���� �Լ� ����
bool Stack::is_empty() { // źâ�� ��������� false�� ��ȯ�ϴ� �Լ�
	return top == -1;	//top�� -1�̸� ����ִ� źâ
}
bool Stack::is_full() { // źâ�� ���������� false�� ��ȯ�ϴ� �Լ�
	return top = MAX_BULLET_SIZE; // �ִ� źâ ������� top�� ũ�ų� ������ Ǯ
}
int Stack::pop() { // �Ѿ��� �߻��ϴ� �Լ�
	return bullet_stack[top--];
}
int Stack::peek() { // �Ѿ��� Ȯ�ξ������� ������� �� ȣ���ϴ� �Լ�
	return bullet_stack[top]; // ���� �Ѿ� ����
}
void Stack::push(int bullet) { // �Ѿ��� �����ϴ� �Լ�
	bullet_stack[++top] = bullet;	//top�� ++�ѵ� top ��ġ�� �Ѿ� ����
}

// �� ���� �Լ� ����
void gun::reload(gun* a_gun) { // ���� �����ϴ� �Լ�
	srand((unsigned int)time(NULL)); // ���� ���� ����, rand()�� ���
	int real_count = 0, fake_count = 0; // �� ź�� �󸶳� �������� ���� ī��Ʈ
	a_gun->real_bullet_count = (rand() % 5) + 1;// 1������ 5���� źȯ ����
	a_gun->fake_bullet_count = (rand() % 5) + 1;
	for (int i = 0; i < a_gun->real_bullet_count + a_gun->fake_bullet_count; i++) { // ��ź�� ����ź�� ���� ��ŭ ����, ��ź�� 1 ����ź�� 2
		int n = rand() % 2; // 0���� 1���� ���� �߻�
		if (n == 0) { //0�� ���
			if(real_count < real_bullet_count){//��ź�� �Ѿ� �������� ���� ���� �ʵ��� ����
				magazine->push(1);// ��ź ����
				real_count++;
			}
			else {
				magazine->push(2);
				fake_count++;
			}
		}
		else if(n == 1) { // 1�� ���
			if (fake_count < fake_bullet_count) {// ����ź�� �Ѿ� �������� ���� ���� �ʵ��� ����
				magazine->push(2); // ����ź ����
				fake_count++;
			}
			else {
				magazine->push(1);
				real_count++;
			}
		}
	}
}
void gun::show_magazine(gun* a_gun) { // ���� �����Ǿ��ִ� �Ѿ��� ������ �����ִ� �Լ�
	cout << "��ź : " << a_gun->real_bullet_count << "��\n";
	cout << "����ź : " << a_gun->fake_bullet_count << "��\n";
}
void gun::self_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip) { // �ڽſ��� ���� �� ���
	int jment = a_gun->magazine->pop();
	if (jment == 1) { // ������ ���� ��ź
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
	}
	else if (jment == 2) { // ������ ���� ��ź�� �ƴ� ���
		cout << "������ �Ѿ��� ����ź�̿����ϴ�.\n";
		Sleep(1000);
		who_player->adjustment_player_turn(who_player, true); // �ڽ��� �� ���
		to_player->adjustment_player_turn(to_player, false);
	}
	turn_skip = false;
}
void gun::to_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip) { //��뿡�� ��� �Լ�
	int jment = a_gun->magazine->pop();
	if (jment == 1) { // ������ ���� ��ź
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
	}
	else if (jment == 2) { // ������ ���� ��ź�� �ƴ� ���
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
	}
	turn_skip = false;
}
int gun::check_bullet(gun* gun) { // ���� ������ �Ѿ��� �������� Ȯ���ϴ� �Լ� 
	return gun->magazine->peek(); 
}

//������ ���� �Լ� ����
void item::show_item() { //�����ϰ� �ִ� �������� ������ ����ϴ� �Լ�
	cout << "hot6 ���� : " << item1_count << "��\n";
	cout << "slap ���� : " << item2_count << "��\n";
	cout << "strn ���� : " << item3_count << "��\n";
}
bool item::check_item(int num) { //�ش� ��ȣ�� �������� �����ϰ� �ִ��� Ȯ���ϴ� �Լ�
	if ((num == 1) and (item1_count > 0)) {
		return true;
	}
	else if ((num == 2) and (item2_count > 0)) {
		return true;
	}
	else if ((num == 3) and (item3_count > 0)) {
		return true;
	}
	else {
		return false;
	}
}
void player::use_item1(player* player) { // hot6����ϴ� �Լ�
	if(player->hp < maxhp) // �÷��̾��� ���� ü���� �ִ� ü�°� ���� ��� ��� �Ұ�
		if (player->player_item->check_item(1) == true) {
			cout << "hp�� ȸ���Ǿ����ϴ�.\n";
			player->hp++;
			player->player_item->adjustment_item(1, false);
		}
		else {
			cout << "�������� �����ϰ� ���� �ʽ��ϴ�.\n";
		}
	else {
		cout << "���� ü���� �ִ� ü���� ���� �� �����ϴ�.\n";
	}
}
void player::use_item2(player* player, gun* a_gun) { //slap ����ϴ� �Լ�
	if (player->player_item->check_item(2) == true) { // �������� ���� �ִ��� Ȯ��
		if (a_gun->check_bullet(a_gun) == 1) { // ������ �Ѿ� Ȯ��
			cout << "���� �����Ǿ��ִ� źȯ�� ��ź �Դϴ�\n";
		}
		else if(a_gun->check_bullet(a_gun) == 2){
			cout << "���� �����Ǿ��ִ� źȯ�� ����ź �Դϴ�\n";
		}
		player->player_item->adjustment_item(2, false);
	}
	else {
		cout << "�������� �����ϰ� ���� �ʽ��ϴ�.\n";
	}
}
void player::use_item3(player* player, bool& turn_skip) { //strn������ ����ϴ� �Լ�
	if (player->player_item->check_item(3) == true) {
		cout << "����� �������� ���� �մϴ�.\n";
		turn_skip = true;
		player->player_item->adjustment_item(3, false);
	}
	else {
		cout << "�������� �����ϰ� ���� �ʽ��ϴ�.\n";
	}
}
void item::adjustment_item(int item_num, bool plus_minus) { // �÷��̾��� �������� �����ϴ� �Լ�
	if (plus_minus == true) { // �߰� �� ���
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
	else if (plus_minus == false) { //���� �� ���
		if ((item_num == 1) and (item1_count > 0)) {
			item1_count--;

		}
		else if ((item_num == 2) and (item2_count > 0)) {
			item2_count--;

		}
		else if ((item_num == 3) and (item3_count > 0)) {
			item3_count--;
		}
	}
}

//�÷��̾� ���� �Լ� ����
int player::maxhp = 0; // ����ƽ ������ maxhp�� ����ϱ� ���� �ʱ�ȭ
void player::hp_setting(player* p1, player* p2) { // �÷��̾���� hp�� �����ϴ� �Լ�
	srand((unsigned int)time(NULL));
	maxhp = (rand() + 1) % 5 + 1;
	p1->hp = maxhp;
	p2->hp = p1->hp;
}
void player::adjustment_hp(player* player, bool plus_minus) { // hp�� �����ϴ� �Լ�
	if (plus_minus == true) {
		player->hp++;
	}
	else if (plus_minus == false) {
		player->hp--;
	}
}
void player::adjustment_player_turn(player* player, bool plus_minus) {// �÷��̾� ���� �����ϴ� �Լ�
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
	waitForEnter();
}
void game_interface::game_start() { // ���� ���� �Լ�
	player* p1 = new player;	// �÷��̾� ��ü ����
	player* p2 = new player;
	gun* a_gun = new gun;	// ���ο� �� ��ü ����
	bool turn_skip = false; //�÷��̾��� ���� ��ŵ���� ���� �����ϴ� ����
	int choice; // ���� ���� �������� ������� ���ϴ� ����
	int choice2; // ���� �ڽ��̳� ������� ����, � �������� �����ؼ� ���� �����ϴ� ����
	srand((unsigned int)time(NULL)); // ���� ������ �����ϱ� ���� �Լ�
	player::hp_setting(p1, p2);// �÷��̾���� hp ����
	player::show_maxhp();	// �ִ� ü�� ���

	while ((p1->hp != 0) and (p2->hp != 0)) { // �� �÷��̾��� ��� ������ ü���� 0�� �ɶ����� �ݺ�
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
		a_gun->reload(a_gun); // �� ������
		Sleep(1000);
		a_gun->show_magazine(a_gun); // �Ѿ˿� ������ ��ź�� ����ź�� ���� ���

		while (a_gun->magazine->is_empty() == false) { // ź������ ������� ���� ������ �ݺ�
			choice = 0; // ���ú��� �ʱ�ȭ
			choice2 = 0; // ���ú��� �ʱ�ȭ2
			if ((p1->player_turn == true) and (p2->player_turn == false)) { // �÷��̾�1�� ��
				cout << "player1�� �����Դϴ�\n";
				Sleep(1000);
				p1->show_player_item(); // �÷��̾ ������ �ִ� �������� ���� �����ֱ�
				p1->show_hp();	// �÷��̾��� ü���� �����ֱ�
				cout << "���� ���� �������� ������� �������ּ���\n";
				cout << "�� : 1 ������ : 2\n";
				cin >> choice;
				if (choice == 1) { // ���� ��ٸ� ���� �������
					cout << "�ڽſ��� ���� ���� ��뿡�� ���� ���� �������ּ���\n";
					cout << "�ڽ� : 1  ��� : 2\n";
					cin >> choice2;
					if (choice2 == 1) {// �ڽ����� ���� �� ���
						a_gun->self_shoot(p1, p2, a_gun, turn_skip);
						if (p1->hp == 0) { //p1�� ü���� 0�̵Ǿ��� ��� �ݺ� ����
							break;
						}
					}
					else if (choice2 == 2) {//��뿡�� ���� �� ���
						a_gun->to_shoot(p1, p2, a_gun, turn_skip);
						if (p2->hp == 0) { //p1�� ü���� 0�� �Ǿ��� ��� �ݺ�����
							break;
						}
					}
					else {
						cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
					}
				}
				else if (choice == 2) { // �������� ����� ���
					cout << "����� �������� ��ȣ�� �������ּ���\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1) { // 1�� ������ ����
						p1->use_item1(p1);
					}
					else if (choice2 == 2) { // 2�� ������ ����
						p1->use_item2(p1, a_gun);
					}
					else if (choice2 == 3) { // 3�� ������ ����
						p1->use_item3(p1, turn_skip);
					}
					else {
						cout << "�߸� �Է��߽��ϴ�\n";
					}
				}
				else {
					cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
				}
			}
			else if ((p2->player_turn == true) and (p1->player_turn == false)) { // �÷��̾�2�� ��
				cout << "player2�� �����Դϴ�\n";
				Sleep(1000);
				p2->show_player_item(); // �÷��̾ ������ �ִ� �������� ���� �����ֱ�
				p2->show_hp();	// �÷��̾��� ü���� �����ֱ�
				cout << "���� ���� �������� ������� �������ּ���\n";
				cout << "�� : 1 ������ : 2\n";
				cin >> choice;
				if (choice == 1) { // ���� ��ٸ� ���� �������
					cout << "�ڽſ��� ���� ���� ��뿡�� ���� ���� �������ּ���\n";
					cout << "�ڽ� : 1  ��� : 2\n";
					cin >> choice2;
					if (choice2 == 1) {// �ڽ����� ���� �� ���
						a_gun->self_shoot(p2, p1, a_gun, turn_skip);
						if (p2->hp == 0) { //p2�� ü���� 0�̵Ǿ��� ��� �ݺ� ����
							break;
						}
					}
					else if (choice2 == 2) {//��뿡�� ���� �� ���
						a_gun->to_shoot(p2, p1, a_gun, turn_skip);
						if (p1->hp == 0) { //p1�� ü���� 0�� �Ǿ��� ��� �ݺ�����
							break;
						}
					}
					else {
						cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
					}
				}
				else if (choice == 2) { // �������� ����� ���
					cout << "����� �������� ��ȣ�� �������ּ���\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1) { // 1�� ������ ����
						p2->use_item1(p2);
					}
					else if (choice2 == 2) { // 2�� ������ ����
						p2->use_item2(p2, a_gun);
					}
					else if (choice2 == 3) { // 3�� ������ ����
						p2->use_item3(p2, turn_skip);
					}
					else {
						cout << "�߸� �Է��߽��ϴ�\n";
					}
				}
				else {
					cout << "�ùٸ� ��ȣ�� �Է����ּ���\n";
				}
			}
			else {
				cout << "�� �� ���� ������ ������ �����մϴ�.";
				waitForEnter();
				break;
			}
		}
	}
	if ((p1->hp <= 0) and (p2->hp > 0)) { // p2�� �¸� ���� ���
		cout << "p2�� �¸��Ͽ����ϴ�.\n";
		waitForEnter();
		exit(0);
	}
	else if ((p2->hp <= 0) and (p1->hp > 0)) { // p1�� �¸� ���� ���
		cout << "p1�� �¸��Ͽ����ϴ�.\n";
		waitForEnter();
		exit(0);
	}
	else if ((p2->hp <= 0) and (p1->hp <= 0)) { // ���º� ���� ���
		cout << "���º� �Դϴ�\n";
		waitForEnter();
		exit(0);
	}
	else {
		cout << "�� �� ���� �ý��� ������ �߻��߽��ϴ�.";
		waitForEnter();
		exit(0);
	}
}
void game_interface::waitForEnter() { // ����Ű�� �Է� ���� ������ ��� �ϴ� �Լ� ���� �۵�X
	char input;
	cout << "Enter Ű�� �����ּ���\n";
	input = getc(stdin);
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