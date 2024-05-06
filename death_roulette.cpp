#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <string>
using namespace std;

#define MAX_BULLET_SIZE 10

class game_interface {
public:
	static int start();	// 처음 게임을 실행했을때 보여지는 인터페이스
	static void game_start();	// 게임시작
	static void game_inf();	// 게임 룰을 보여주는 인터페이스
};
class Stack {	// 총알을 스택 형식으로 쌓기 위해 스택 클래스 생성
	int top;	// 다음에 나갈 총알의 위치를 가리키는 변수
	bool* bullet_stack; // 총알집 구현
public:
	Stack() { this->top = -1, this->bullet_stack = new bool[MAX_BULLET_SIZE]; } // 최대 10개 탄을 창착
	bool pop();
	bool peek();
	void push(bool bullet);
	bool is_empty();
	bool is_full();
};
class player {
	int hp;	// 플레이어의 hp를 저장하는 변수
	bool player_turn;	// 플레이어의 턴인지 확인 하는 변수
	item* player_item;	// item클래스로 구현된 플레이어의 아이템 변수
public:
	player() { this->hp = 0, player_turn = false, this->player_item = new item; } // 플레이어 생성자
	static int maxhp;
	static void hp_setting(player* p1, player* p2);	// 플레이어의 hp와 초기 maxhp를 세팅하는 함수
	void adjustment_hp(player* player, bool plus_minus);
	void adjustment_player_turn(player* player, bool plus_minus);
	void show_hp();
	static void show_maxhp();	// 플레이어들의 최대 체력을 보여주기
	void show_player_item();// 현재 플레이어가 보유하고 있는 아이템의 갯수를 보여주는 함수
	friend void game_interface::game_start(); // 게임에서 플레이어 객체의 함수에 접근할 수 있도록 프렌드 설정
};
class item { // 아이템 클래스
	int item1_count;	// 1번 아이템의 갯수를 저장하는 변수
	int item2_count;	// 2번 아이템의 갯수를 저장하는 변수
	int item3_count;	// 3번 아이템의 갯수를 저장하는 변수
public:
	item() { item1_count = 0; item2_count = 0; item3_count = 0; }	// 처음 객체 생성시 아이템의 갯수는 전부 0개
	void show_item();	//아이템의 갯수를 보여주는 함수
	bool check_item(int num); // 선택된 아이템이 있는지 확인하는 함수
	void use_item(player* player, int num);
	void adjustment_item(int item_num, bool plus_minus); // 아이템의 갯수를 조정할 때 사용하는 함수
};
class gun {
	int real_bullet_count;	// 실탄의 갯수를 저장하는 변수
	int fake_bullet_count;	// 공포탄의 갯수를 저장하는 변수
	Stack* magazine;	// 스택으로 구현되어있는 탄장
public:
	gun() { this->real_bullet_count = 0, this->fake_bullet_count = 0, this->magazine = new Stack; } // 총 객체 생성자
	void reload(); //총알 장전
	void show_magazine(); // 탄창에 들어있는 탄환을 보여주는 함수
	void self_shoot(player* who_player, player *to_player, bool &turn_skip);
	void to_shoot(player* who_player, player* to_player, bool& turn_skip);
	friend void game_interface::game_start(); // 게임에서 총객체의 함수에 접근할 수 있도록 프렌드 설정
};

// 스택 관련 함수 구현
bool Stack::is_empty() { // 탄창이 비어있으면 false를 반환하는 함수
	return top == -1;	//top이 -1이면 비어있는 탄창
}
bool Stack::is_full() { // 탄창이 꽉차있으면 false를 반환하는 함수
	return top >= MAX_BULLET_SIZE; // 최대 탄창 사이즈보다 top이 크거나 같으면 풀
}
bool Stack::pop() { // 총알을 발사하는 함수
	return bullet_stack[top--];
}
bool Stack::peek() { // 총알을 확인아이템을 사용했을 때 호출하는 함수
	return bullet_stack[top]; // 현재 총알 리턴
}
void Stack::push(bool bul) { // 총알을 장전하는 함수
	this->bullet_stack[++top] = bul;	//top을 ++한뒤 top 위치에 총알 저장
}

// 총 관련 함수 구현
void gun::reload() { // 총을 장전하는 함수
	srand((unsigned int)time(NULL)); // 랜덤 정수 도출 rand()로 사용
	real_bullet_count = (rand() % 5) + 1; // 1개에서 5개의 탄환 랜덤
	fake_bullet_count = (rand() % 5) + 1;
	for (int i = 0; i < real_bullet_count; i++) { // 실탄의 갯수 만큼 장전
		magazine->push(true);
	}
	for (int j = 0; j < fake_bullet_count; j++) { // 공포탄의 갯수 만큼 장전
		magazine->push(false);
	}
}
void gun::show_magazine() { // 현재 장전되어있는 총알의 갯수를 보여주는 함수
	cout << "실탄 : " << real_bullet_count << "개\n";
	cout << "공포탄 : " << fake_bullet_count << "개\n";
}
void gun::self_shoot(player* who_player, player* to_player, bool &turn_skip){
	if (magazine->pop() == true) { // 장전된 총이 실탄
		cout << "장전된 총알은 실탄이였습니다\n";
		Sleep(1000);
		who_player->adjustment_hp(who_player, false);	// 본인 플레이어 체력 -1
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // 턴 스킵이 있을 경우 자신의 턴 한번더
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false); // 아닐 경우 상대턴으로 넘어감
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
	else if (magazine->pop() == false) { // 장전된 총이 실탄이 아닐 경우
		cout << "장전된 총알은 공포탄이였습니다.\n";
		Sleep(1000);
		who_player->adjustment_player_turn(who_player, true); // 자신의 턴 계속
		to_player->adjustment_player_turn(to_player, false);
		turn_skip = false;
	}
}
void gun::to_shoot(player* who_player, player* to_player, bool &turn_skip) {
	if (magazine->pop() == true) { // 장전된 총이 실탄
		cout << "장전된 총알은 실탄이였습니다\n";
		Sleep(1000);
		to_player->adjustment_hp(to_player, false);	// 상대 플레이어 체력 -1
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // 턴 스킵이 있을 경우 자신의 턴 한번더
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false);; // 아닐 경우 상대턴으로 넘어감
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
	else if (magazine->pop() == false) { // 장전된 총이 실탄이 아닐 경우
		cout << "장전된 총알은 공포탄이였습니다.\n";
		Sleep(1000);
		if (turn_skip == true) {
			who_player->adjustment_player_turn(who_player, true); // 턴 스킵이 있을 경우 자신의 턴 한번더
			to_player->adjustment_player_turn(to_player, false);
		}
		else {
			who_player->adjustment_player_turn(who_player, false);; // 아닐 경우 상대턴으로 넘어감
			to_player->adjustment_player_turn(to_player, true);
		}
		turn_skip = false;
	}
}

//아이템 관련 함수 구현
void item::show_item() { //보유하고 있는 아이템의 갯수를 출력하는 함수
	cout << "hot6 갯수 : " << item1_count << "개\n";
	cout << "slap 갯수 : " << item2_count << "개\n";
	cout << "strn 갯수 : " << item3_count << "개\n";
}
bool item::check_item(int num) { //해당 번호의 아이템을 보유하고 있는지 확인하는 함수
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
		cout << "hp가 회복되었습니다.\n";
		p2->hp++;
		p2->player_item->adjustment_item(1, false);
	}
	else if (choice2 == 2 and p2->player_item->check_item(2) == true) {
		if (a_gun->magazine->peek() == true) {
			cout << "현재 장전되어있는 탄환은 실탄 입니다\n";
		}
		else {
			cout << "현재 장전되어있는 탄환은 공포탄 입니다\n";
		}
		p2->player_item->adjustment_item(2, false);
	}
	else if (choice2 == 3 and p2->player_item->check_item(3) == true) {
		cout << "상대의 움직임을 봉인 합니다.\n";
		turn_skip = true;
		p2->player_item->adjustment_item(3, false);
	}
	else {
		cout << "잘못 입력했거나 현재 보유아이템이 없습니다.\n";
	}
}
void item::adjustment_item(int item_num, bool plus_minus) { // 플레이어의 아이템을 조정하는 함수
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

//플레이어 관련 함수 구현
int player::maxhp = 0; // 스태틱 변수인 maxhp를 사용하기 위해
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
void player::show_maxhp() { // 플레이어들의 최대 체력을 보여주는 함수
	cout << "양측 플레이어의 최대 체력은" << maxhp << "입니다.\n";
}
void player::show_hp() { // 현재 플레이어의 체력을 보여주는 함수
	cout << "현재 플레이어의 체력은" << hp << "입니다.\n";
}
void player::show_player_item() { //현재 플레이어가 보유한 아이템의 갯수를 보여주는 함수
	cout << "현재 플레이어가 보유한 아이템의 갯수는\n";
	player_item->show_item();
}

//인터페이스 관련 함수 구현
int game_interface::start() { // 게임을 실행했을 때 플레이어에게 게임 시작, 게임 룰, 게임 종료를 선택 받는 함수
	cout << "----------------------------------------------\n";
	cout << "|  1. 게임시작 2. 게임 정보   3. 게임 종료   |\n";
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
void game_interface::game_inf() { // 게임의 룰을 보여주는 함수
	cout << "게임의 정보들을 보여드림\n";
	cout << "엔터키를 입력하세요\n";
	int ch = getc(stdin);
}
void game_interface::game_start() { // 게임 실행 함수
	player* p1 = new player;	// 플레이어 객체 생성
	player* p2 = new player;
	gun* a_gun = new gun;	// 새로운 총 객체 생성
	bool turn_skip = false; //플레이어의 턴을 스킵할지 말지 결정하는 변수
	srand((unsigned int)time(NULL)); // 랜덤 정수를 생성하기 위한 함수
	player::hp_setting(p1, p2);// 플레이어들의 hp 셋팅
	char ch; // 게임이 종료 되었을 때 플레이어에게 앤터키를 입력 받기 위한 변수

	player::show_maxhp();	// 최대 체력 출력
	while (p1->hp != 0 or p2->hp != 0) { // 두 플레이어의 어느 한쪽의 체력이 0이 될때까지 반복
		for (int i = 1; i < 4; i++) {
			p1->player_item->adjustment_item(i, rand() % 2);	// 플레이어의 아이템을 랜덤으로 0개에서 1개 추가
			p2->player_item->adjustment_item(i, rand() % 2);
		}
		p1->player_turn = rand() % 2;	// 플레이어의 시작 차례를 랜덤으로 설정
		if (p1->player_turn == 1) {
			p2->player_turn = 0;
		}
		else {
			p2->player_turn = 1;
		}

		cout << "총알을 장전 합니다.\n";
		a_gun->reload(); // 총 재장전
		Sleep(1000);
		a_gun->show_magazine(); // 총알에 장전된 실탄과 공포탄의 갯수 출력

		while (a_gun->magazine->is_empty() == false) {
			int choice = 0; // 총을 쏠지 아이템을 사용할지 정하는 변수
			int choice2 = 0; // 총을 자신이나 상대한테 쏠지, 어떤 아이템을 선택해서 쏠지 결정하는 변수
			if (p1->player_turn == true and p2->player_turn == false) {
				cout << "player1의 차례입니다\n";
				Sleep(1000);
				p1->show_player_item(); // 플레이어가 가지고 있는 아이템의 갯수 보여주기
				p1->show_hp();	// 플레이어의 체력을 보여주기
				cout << "총을 쏠지 아이템을 사용할지 선택해주세요\n";
				cout << "총 : 1 아이템 : 2\n";
				cin >> choice;
				if (choice == 1) {
					cout << "자신에게 총을 쏠지 상대에게 총을 쏠지 선택해주세요\n";
					cout << "자신 : 1  상대 : 2\n";
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
						cout << "올바른 번호를 입력해주세요\n";
					}
				}
				else if (choice == 2) {
					cout << "사용할 아이템의 번호를 선택해주세요\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1 and p1->player_item->check_item(1) == true) {
						cout << "hp가 회복되었습니다.\n";
						p1->hp++;
						p1->player_item->adjustment_item(1, false);
					}
					else if (choice2 == 2 and p1->maxhp < p1->hp and p1->player_item->check_item(2) == true) {
						if (a_gun->magazine->peek() == true) {
							cout << "현재 장전되어있는 탄환은 실탄 입니다\n";
							Sleep(1000);
						}
						else {
							cout << "현재 장전되어있는 탄환은 공포탄 입니다\n";
							Sleep(1000);
						}
						p1->player_item->adjustment_item(2, false);
					}
					else if (choice2 == 3 and p1->player_item->check_item(3) == true) {
						cout << "상대의 움직임을 봉인 합니다.\n";
						Sleep(1000);
						turn_skip = true;
						p1->player_item->adjustment_item(3, false);
					}
					else {
						cout << "잘못 입력했거나 현재 보유아이템이 없습니다.\n";
					}
				}
				else {
					cout << "올바른 번호를 입력해주세요\n";
				}
			}
			else if (p2->player_turn == true and p1->player_turn == false) {
				cout << "player2의 차례입니다\n";
				Sleep(1000);
				p2->show_player_item(); // 플레이어가 가지고 있는 아이템의 갯수 보여주기
				p2->show_hp();	// 플레이어의 체력을 보여주기
				cout << "총을 쏠지 아이템을 사용할지 선택해주세요\n";
				cout << "총 : 1 아이템 : 2\n";
				cin >> choice;
				if (choice == 1) {
					cout << "자신에게 총을 쏠지 상대에게 총을 쏠지 선택해주세요\n";
					cout << "자신 : 1  상대 : 2\n";
					cin >> choice2;
					if (choice2 == 1) { // 자신에게 쏜다른 선택했을 경우
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
							cout << "올바른 번호를 입력해주세요\n";
						}

					}
				}
				else if (choice == 2) {
					cout << "사용할 아이템의 번호를 선택해주세요\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1 and p2->player_item->check_item(1) == true) {
						cout << "hp가 회복되었습니다.\n";
						p2->hp++;
						p2->player_item->adjustment_item(1, false);
					}
					else if (choice2 == 2 and p2->player_item->check_item(2) == true) {
						if (a_gun->magazine->peek() == true) {
							cout << "현재 장전되어있는 탄환은 실탄 입니다\n";
						}
						else {
							cout << "현재 장전되어있는 탄환은 공포탄 입니다\n";
						}
						p2->player_item->adjustment_item(2, false);
					}
					else if (choice2 == 3 and p2->player_item->check_item(3) == true) {
						cout << "상대의 움직임을 봉인 합니다.\n";
						turn_skip = true;
						p2->player_item->adjustment_item(3, false);
					}
					else {
						cout << "잘못 입력했거나 현재 보유아이템이 없습니다.\n";
					}
				}
				else {
					cout << "올바른 번호를 입력해주세요\n";
				}
			}
			else {
				cout << "알 수 없는 오류로 게임을 종료합니다.";
				ch = cin.get();
				break;
			}
		}
	}
	if (p1->hp <= 0 and p2->hp > 0) {
		cout << "p2가 승리하였습니다.\n";
		ch = cin.get();
	}
	else if (p2->hp <= 0 and p1->hp > 0) {
		cout << "p1이 승리하였습니다.\n";
		ch = cin.get();
	}
	else if (p2->hp <= 0 and p1->hp <= 0) {
		cout << "무승부 입니다\n";
		ch = cin.get();
	}
	else {
		cout << "알 수 없는 시스템 오류가 발생했습니다.";
		ch = cin.get();
	}
}
// 메인 함수
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
			cout << "1에서 3의 숫자를 골라주세요\n";
		}
	}
}