#include <iostream>
#include <windows.h>
using namespace std;

#define MAX_BULLET_SIZE 10 // 

class player;
class game_interface {
public:
	static int start();	// 처음 게임을 실행했을때 보여지는 인터페이스
	static void game_start();	// 게임시작
	static void game_inf();	// 게임 룰을 보여주는 인터페이스
	static void waitForEnter(); //사용자의 엔터키 입력을 기다리는 함수
};
class Stack {	// 총알을 스택 형식으로 쌓기 위해 스택 클래스 생성
	int top;	// 다음에 나갈 총알의 위치를 가리키는 변수
	int* bullet_stack; // 총알집 구현
public:
	Stack() { this->top = -1, this->bullet_stack = new int[MAX_BULLET_SIZE]; } // 최대 10개 탄을 창착
	int pop();
	int peek();
	void push(int bullet);
	bool is_empty();
	bool is_full();
};
class item { // 아이템 클래스
	int item1_count;	// 1번 아이템의 갯수를 저장하는 변수
	int item2_count;	// 2번 아이템의 갯수를 저장하는 변수
	int item3_count;	// 3번 아이템의 갯수를 저장하는 변수
public:
	item() { item1_count = 0; item2_count = 0; item3_count = 0; }	// 처음 객체 생성시 아이템의 갯수는 전부 0개
	void show_item();	//아이템의 갯수를 보여주는 함수
	bool check_item(int num); // 선택된 아이템이 있는지 확인하는 함수
	void adjustment_item(int item_num, bool plus_minus); // 아이템의 갯수를 조정할 때 사용하는 함수
};
class gun {
	int real_bullet_count;	// 실탄의 갯수를 저장하는 변수
	int fake_bullet_count;	// 공포탄의 갯수를 저장하는 변수
	Stack* magazine;	// 스택으로 구현되어있는 탄장
public:
	gun() { this->real_bullet_count = 0, this->fake_bullet_count = 0, this->magazine = new Stack; } // 총 객체 생성자
	void reload(gun* a_gun); //총알 장전
	void show_magazine(gun* a_gun); // 탄창에 들어있는 탄환을 보여주는 함수
	void self_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip);
	void to_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip);
	int check_bullet(gun* gun);
	friend void game_interface::game_start(); // 게임에서 총객체의 함수에 접근할 수 있도록 프렌드 설정
};
class player {
	int hp;	// 플레이어의 hp를 저장하는 변수
	bool player_turn;	// 플레이어의 턴인지 확인 하는 변수
	item* player_item;	// item클래스로 구현된 플레이어의 아이템 변수
public:
	player() { this->hp = 0, player_turn = false, this->player_item = new item; } // 플레이어 생성자
	static int maxhp; //최대 체력 변수
	static void hp_setting(player* p1, player* p2);	// 플레이어의 hp와 초기 maxhp를 세팅하는 함수
	void adjustment_hp(player* player, bool plus_minus);
	void adjustment_player_turn(player* player, bool plus_minus);
	void use_item1(player* player);
	void use_item2(player* player, gun* a_gun);
	void use_item3(player* player, bool& turn_skip);
	void show_hp();
	static void show_maxhp();	// 플레이어들의 최대 체력을 보여주기
	void show_player_item();// 현재 플레이어가 보유하고 있는 아이템의 갯수를 보여주는 함수
	friend void game_interface::game_start(); // 게임에서 플레이어 객체의 함수에 접근할 수 있도록 프렌드 설정
};

// 스택 관련 함수 구현
bool Stack::is_empty() { // 탄창이 비어있으면 false를 반환하는 함수
	return top == -1;	//top이 -1이면 비어있는 탄창
}
bool Stack::is_full() { // 탄창이 꽉차있으면 false를 반환하는 함수
	return top = MAX_BULLET_SIZE; // 최대 탄창 사이즈보다 top이 크거나 같으면 풀
}
int Stack::pop() { // 총알을 발사하는 함수
	return bullet_stack[top--];
}
int Stack::peek() { // 총알을 확인아이템을 사용했을 때 호출하는 함수
	return bullet_stack[top]; // 현재 총알 리턴
}
void Stack::push(int bullet) { // 총알을 장전하는 함수
	bullet_stack[++top] = bullet;	//top을 ++한뒤 top 위치에 총알 저장
}

// 총 관련 함수 구현
void gun::reload(gun* a_gun) { // 총을 장전하는 함수
	srand((unsigned int)time(NULL)); // 랜덤 정수 도출, rand()로 사용
	int real_count = 0, fake_count = 0; // 두 탄이 얼마나 들어갔는지에 대한 카운트
	a_gun->real_bullet_count = (rand() % 5) + 1;// 1개에서 5개의 탄환 랜덤
	a_gun->fake_bullet_count = (rand() % 5) + 1;
	for (int i = 0; i < a_gun->real_bullet_count + a_gun->fake_bullet_count; i++) { // 실탄과 공포탄의 갯수 만큼 장전, 실탄은 1 공포탄은 2
		int n = rand() % 2; // 0에서 1랜덤 숫자 발생
		if (n == 0) { //0일 경우
			if(real_count < real_bullet_count){//실탄의 총알 갯수보다 많이 들어가지 않도록 조절
				magazine->push(1);// 실탄 장전
				real_count++;
			}
			else {
				magazine->push(2);
				fake_count++;
			}
		}
		else if(n == 1) { // 1일 경우
			if (fake_count < fake_bullet_count) {// 공포탄의 총알 갯수보다 많이 들어가지 않도록 조절
				magazine->push(2); // 공포탄 장전
				fake_count++;
			}
			else {
				magazine->push(1);
				real_count++;
			}
		}
	}
}
void gun::show_magazine(gun* a_gun) { // 현재 장전되어있는 총알의 갯수를 보여주는 함수
	cout << "실탄 : " << a_gun->real_bullet_count << "개\n";
	cout << "공포탄 : " << a_gun->fake_bullet_count << "개\n";
}
void gun::self_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip) { // 자신에게 총을 쏠 경우
	int jment = a_gun->magazine->pop();
	if (jment == 1) { // 장전된 총이 실탄
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
	}
	else if (jment == 2) { // 장전된 총이 실탄이 아닐 경우
		cout << "장전된 총알은 공포탄이였습니다.\n";
		Sleep(1000);
		who_player->adjustment_player_turn(who_player, true); // 자신의 턴 계속
		to_player->adjustment_player_turn(to_player, false);
	}
	turn_skip = false;
}
void gun::to_shoot(player* who_player, player* to_player, gun* a_gun, bool& turn_skip) { //상대에게 쏘는 함수
	int jment = a_gun->magazine->pop();
	if (jment == 1) { // 장전된 총이 실탄
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
	}
	else if (jment == 2) { // 장전된 총이 실탄이 아닐 경우
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
	}
	turn_skip = false;
}
int gun::check_bullet(gun* gun) { // 현재 장전된 총알이 무엇인지 확인하는 함수 
	return gun->magazine->peek(); 
}

//아이템 관련 함수 구현
void item::show_item() { //보유하고 있는 아이템의 갯수를 출력하는 함수
	cout << "hot6 갯수 : " << item1_count << "개\n";
	cout << "slap 갯수 : " << item2_count << "개\n";
	cout << "strn 갯수 : " << item3_count << "개\n";
}
bool item::check_item(int num) { //해당 번호의 아이템을 보유하고 있는지 확인하는 함수
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
void player::use_item1(player* player) { // hot6사용하는 함수
	if(player->hp < maxhp) // 플레이어의 현재 체력이 최대 체력과 같은 경우 사용 불가
		if (player->player_item->check_item(1) == true) {
			cout << "hp가 회복되었습니다.\n";
			player->hp++;
			player->player_item->adjustment_item(1, false);
		}
		else {
			cout << "아이템을 보유하고 있지 않습니다.\n";
		}
	else {
		cout << "현재 체력이 최대 체력을 넘을 수 없습니다.\n";
	}
}
void player::use_item2(player* player, gun* a_gun) { //slap 사용하는 함수
	if (player->player_item->check_item(2) == true) { // 아이템을 갖고 있는지 확인
		if (a_gun->check_bullet(a_gun) == 1) { // 장전된 총알 확인
			cout << "현재 장전되어있는 탄환은 실탄 입니다\n";
		}
		else if(a_gun->check_bullet(a_gun) == 2){
			cout << "현재 장전되어있는 탄환은 공포탄 입니다\n";
		}
		player->player_item->adjustment_item(2, false);
	}
	else {
		cout << "아이템을 보유하고 있지 않습니다.\n";
	}
}
void player::use_item3(player* player, bool& turn_skip) { //strn아이템 사용하는 함수
	if (player->player_item->check_item(3) == true) {
		cout << "상대의 움직임을 봉인 합니다.\n";
		turn_skip = true;
		player->player_item->adjustment_item(3, false);
	}
	else {
		cout << "아이템을 보유하고 있지 않습니다.\n";
	}
}
void item::adjustment_item(int item_num, bool plus_minus) { // 플레이어의 아이템을 조정하는 함수
	if (plus_minus == true) { // 추가 일 경우
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
	else if (plus_minus == false) { //감소 일 경우
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

//플레이어 관련 함수 구현
int player::maxhp = 0; // 스태틱 변수인 maxhp를 사용하기 위해 초기화
void player::hp_setting(player* p1, player* p2) { // 플레이어들의 hp를 세팅하는 함수
	srand((unsigned int)time(NULL));
	maxhp = (rand() + 1) % 5 + 1;
	p1->hp = maxhp;
	p2->hp = p1->hp;
}
void player::adjustment_hp(player* player, bool plus_minus) { // hp를 조정하는 함수
	if (plus_minus == true) {
		player->hp++;
	}
	else if (plus_minus == false) {
		player->hp--;
	}
}
void player::adjustment_player_turn(player* player, bool plus_minus) {// 플레이어 턴을 조정하는 함수
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
	waitForEnter();
}
void game_interface::game_start() { // 게임 실행 함수
	player* p1 = new player;	// 플레이어 객체 생성
	player* p2 = new player;
	gun* a_gun = new gun;	// 새로운 총 객체 생성
	bool turn_skip = false; //플레이어의 턴을 스킵할지 말지 결정하는 변수
	int choice; // 총을 쏠지 아이템을 사용할지 정하는 변수
	int choice2; // 총을 자신이나 상대한테 쏠지, 어떤 아이템을 선택해서 쏠지 결정하는 변수
	srand((unsigned int)time(NULL)); // 랜덤 정수를 생성하기 위한 함수
	player::hp_setting(p1, p2);// 플레이어들의 hp 셋팅
	player::show_maxhp();	// 최대 체력 출력

	while ((p1->hp != 0) and (p2->hp != 0)) { // 두 플레이어의 어느 한쪽의 체력이 0이 될때까지 반복
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
		a_gun->reload(a_gun); // 총 재장전
		Sleep(1000);
		a_gun->show_magazine(a_gun); // 총알에 장전된 실탄과 공포탄의 갯수 출력

		while (a_gun->magazine->is_empty() == false) { // 탄알집이 비어있지 않을 때까지 반복
			choice = 0; // 선택변수 초기화
			choice2 = 0; // 선택변수 초기화2
			if ((p1->player_turn == true) and (p2->player_turn == false)) { // 플레이어1의 턴
				cout << "player1의 차례입니다\n";
				Sleep(1000);
				p1->show_player_item(); // 플레이어가 가지고 있는 아이템의 갯수 보여주기
				p1->show_hp();	// 플레이어의 체력을 보여주기
				cout << "총을 쏠지 아이템을 사용할지 선택해주세요\n";
				cout << "총 : 1 아이템 : 2\n";
				cin >> choice;
				if (choice == 1) { // 총을 쏜다를 선택 했을경우
					cout << "자신에게 총을 쏠지 상대에게 총을 쏠지 선택해주세요\n";
					cout << "자신 : 1  상대 : 2\n";
					cin >> choice2;
					if (choice2 == 1) {// 자신한테 총을 쏠 경우
						a_gun->self_shoot(p1, p2, a_gun, turn_skip);
						if (p1->hp == 0) { //p1의 체력이 0이되었을 경우 반복 종료
							break;
						}
					}
					else if (choice2 == 2) {//상대에게 총을 쏠 경우
						a_gun->to_shoot(p1, p2, a_gun, turn_skip);
						if (p2->hp == 0) { //p1의 체력이 0이 되었을 경우 반복종료
							break;
						}
					}
					else {
						cout << "올바른 번호를 입력해주세요\n";
					}
				}
				else if (choice == 2) { // 아이템을 사용할 경우
					cout << "사용할 아이템의 번호를 선택해주세요\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1) { // 1번 아이템 사용시
						p1->use_item1(p1);
					}
					else if (choice2 == 2) { // 2번 아이템 사용시
						p1->use_item2(p1, a_gun);
					}
					else if (choice2 == 3) { // 3번 아이템 사용시
						p1->use_item3(p1, turn_skip);
					}
					else {
						cout << "잘못 입력했습니다\n";
					}
				}
				else {
					cout << "올바른 번호를 입력해주세요\n";
				}
			}
			else if ((p2->player_turn == true) and (p1->player_turn == false)) { // 플레이어2의 턴
				cout << "player2의 차례입니다\n";
				Sleep(1000);
				p2->show_player_item(); // 플레이어가 가지고 있는 아이템의 갯수 보여주기
				p2->show_hp();	// 플레이어의 체력을 보여주기
				cout << "총을 쏠지 아이템을 사용할지 선택해주세요\n";
				cout << "총 : 1 아이템 : 2\n";
				cin >> choice;
				if (choice == 1) { // 총을 쏜다를 선택 했을경우
					cout << "자신에게 총을 쏠지 상대에게 총을 쏠지 선택해주세요\n";
					cout << "자신 : 1  상대 : 2\n";
					cin >> choice2;
					if (choice2 == 1) {// 자신한테 총을 쏠 경우
						a_gun->self_shoot(p2, p1, a_gun, turn_skip);
						if (p2->hp == 0) { //p2의 체력이 0이되었을 경우 반복 종료
							break;
						}
					}
					else if (choice2 == 2) {//상대에게 총을 쏠 경우
						a_gun->to_shoot(p2, p1, a_gun, turn_skip);
						if (p1->hp == 0) { //p1의 체력이 0이 되었을 경우 반복종료
							break;
						}
					}
					else {
						cout << "올바른 번호를 입력해주세요\n";
					}
				}
				else if (choice == 2) { // 아이템을 사용할 경우
					cout << "사용할 아이템의 번호를 선택해주세요\n";
					cout << "hot6 : 1, slap : 2, strn : 3\n";
					cin >> choice2;
					if (choice2 == 1) { // 1번 아이템 사용시
						p2->use_item1(p2);
					}
					else if (choice2 == 2) { // 2번 아이템 사용시
						p2->use_item2(p2, a_gun);
					}
					else if (choice2 == 3) { // 3번 아이템 사용시
						p2->use_item3(p2, turn_skip);
					}
					else {
						cout << "잘못 입력했습니다\n";
					}
				}
				else {
					cout << "올바른 번호를 입력해주세요\n";
				}
			}
			else {
				cout << "알 수 없는 오류로 게임을 종료합니다.";
				waitForEnter();
				break;
			}
		}
	}
	if ((p1->hp <= 0) and (p2->hp > 0)) { // p2가 승리 했을 경우
		cout << "p2가 승리하였습니다.\n";
		waitForEnter();
		exit(0);
	}
	else if ((p2->hp <= 0) and (p1->hp > 0)) { // p1이 승리 했을 경우
		cout << "p1이 승리하였습니다.\n";
		waitForEnter();
		exit(0);
	}
	else if ((p2->hp <= 0) and (p1->hp <= 0)) { // 무승부 했을 경우
		cout << "무승부 입니다\n";
		waitForEnter();
		exit(0);
	}
	else {
		cout << "알 수 없는 시스템 오류가 발생했습니다.";
		waitForEnter();
		exit(0);
	}
}
void game_interface::waitForEnter() { // 엔터키를 입력 받을 때까지 대기 하는 함수 현재 작동X
	char input;
	cout << "Enter 키를 눌러주세요\n";
	input = getc(stdin);
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