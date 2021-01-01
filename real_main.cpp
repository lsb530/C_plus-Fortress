/* ============================== SFML 포트리스 게임 ============================== */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

#define A_PI 57.2957795130823208767 // 라디안
#define G 9.8f // 중력
#define LEFT 0 // 왼쪽
#define RIGHT 1 // 오른쪽

const int frameRate = 60; // 프레임 초당 60으로 고정
const float tankSpeed = 1.0f; // 탱크스피드 초기값
const float tankMaxHP = 1000.0f; // 탱크 체력 초기값
const float windowW = 800.0f; 
const float floorY = 385.0f; 

class Object : public Sprite // Object클래스 : Sprite를 묶어줌
{
public:
	Texture texture;
	Object(const string& path, const Vector2f pos = { 0.0f, 0.0f }, const Vector2f scale = { 1.0f, 1.0f }) // Object 생성자(경로,위치,크기)
	{
		texture.loadFromFile(path); // 경로불러오기
		this->setTexture(texture); // 텍스쳐지정
		this->setPosition(pos); // 위치지정
		this->setScale(scale); // 크기 지정
	}
	void center() // 탱크나 총알 등이 좌표기준이 왼쪽 상단으로 되어있는데 중간으로 바꿔주는 함수
	{
		const Vector2u size = texture.getSize();
		this->setOrigin({ size.x / 2.0f, size.y / 2.0f });
	}
};

class Bullet : public Object // 총알 클래스
{
public:
	float range, attack; // 범위, 공격력
	Bullet(const string& path, const float _range, const float _attack) : Object(path) // 경로, 범위, 공격력
	{
		this->center();
		range = _range; // 범위지정
		attack = _attack; // 공격력지정
	}
};

class Tank : public Object // 탱크 클래스
{
public:
	float attack, defense; // 공격력, 방어력
	float hp = tankMaxHP; // hp는 초기값(1000)으로 초기화
	bool direction = LEFT; // 방향은 왼쪽
	Tank(const string& path, const float _attack, const float _defense) : Object(path) // Tank클래스 생성자(경로,공격력,방어력)
	{
		this->setOrigin(20.0f, 20.0f); // 원래위치로 돌려줌(setScale하고난뒤자연스럽게)
		attack = _attack;
		defense = _defense;
	}
	void turn(bool dir) // 방향
	{
		direction = dir; // direction을 dir로 초기화
		this->setScale({ dir == LEFT ? 1.0f : -1.0f, 1.0f }); // dir이 왼쪽이면 -1.0f , 아니면 1.0f로 돌림
	}
	void damage(float dmg) // 데미지(dmg)
	{
		//cout << "HP: " << hp << " - ";
		hp -= dmg *= (1.0f - defense / 100.0f); // hp = hp - 데미지 * (1.0f - 방어력 / 100.0f);
		//cout << dmg << " = " << hp << endl;
	}
};

int main()
{
	// ==================== 소개 ==================== //
	cout << "========== 안녕하세요! 2015726100 이승복의 포트리스 게임입니다!!! ==========" << endl << endl;
	cout << "맵 밖으로 나갈순 없으며, 포탄은 항상 왼쪽 포탄으로 고정됩니다!!"<<endl << endl;
	cout << "각 포탄은 데미지가 다르며, <Q, W>버튼으로 선택이 가능합니다" << endl << endl;
	cout << "좌우 방향키로 이동이 가능하며 상하 방향키로는 각도조절이 가능합니다" << endl << endl;
	cout << "Space키를 한번 누르면 게이지를 모으며 한번 더 누르면 발사합니다" << endl << endl;
	cout << "======================= 그럼 즐거운 게임되세요!!^^♥ =======================" << endl << endl;

	// ==================== 초기 작업 ==================== //
	RenderWindow window(VideoMode(800, 600), "SFML works!", Style::Close | Style::Titlebar);
	window.setFramerateLimit(frameRate); // 프레임을 맞춰준다
	srand(time(NULL)); // 랜덤을 쓰기위한 srand

	// 배경, 상태이상 등 기본 객체(사진)들 불러오기
	Object background("images/map_skybridge.png"); // 맵 배경
	Object statusbar("images/statusbar.png", { 0, 495.0f }); // 상태이상 바
	Object hpbar("images/bar_gauge.png", { 210.0f, 515.0f }, { 17.0f, 1.0f }); // HP 바
	Object powerbar("images/bar_gauge.png", { 210.0f, 550.0f }, { 0, 1.0f }); // 바 게이지(초기0이므로 안보임)
	Object windbar("images/wind_gauge.png", { 69.0f, 564.0f }, { 0, 0 }); // 바람게이지
	Object selector("images/missile_selector.png", { 650.0f, 565.0f }, { 0.5f, 0.5f }); // 미사일 선택
	Object bomb("images/bomb.png");
	bomb.center();

	// 이겼을 때 객체
	Object win1("images/1pwin.png", { 250.0f, 150.0f });
	Object win2("images/2pwin.png", { 250.0f, 150.0f });
	Object* wins[2] = { &win1, &win2 }; // 승리자 값을 배열로 미리 선언해둠

	// 총알 객체(사진)들 불러오기
	Bullet bullet1("images/missile_canon_1.png", 100.0f, 0.6f); // 캐논탱 총알1
	Bullet bullet2("images/missile_canon_2.png", 20.0f, 2.0f); // 캐논탱 총알2
	Bullet bullet3("images/missile_missile_1.png", 40.0f, 1.2f); // 미사일탱 총알1
	Bullet bullet4("images/missile_missile_2.png", 20.0f, 0.3f); // 미사일탱 총알2
	Bullet bullett[4] = { bullet1, bullet2, bullet3, bullet4 }; // 미사일 선택 배열
	Bullet* bullets[4] = { &bullet1, &bullet2, &bullet3, &bullet4 }; // 발사할 때 표시하는 배열
	//위치지정
	bullett[0].setPosition({ 660.0f, 545.0f }); 
	bullett[1].setPosition({ 710.0f, 545.0f });
	bullett[2].setPosition({ 660.0f, 545.0f });
	bullett[3].setPosition({ 710.0f, 545.0f });

	//기본 탱크 불러오기
	Tank tank1("images/tank_canon.png", 300.0f, 30.0f); // 탱크1은 공격력 300, 방어력 30
	Tank tank2("images/tank_missile.png", 200.0f, 40.0f); // 탱크2는 공격력 200, 방어력 40
	Tank* tanks[2] = { &tank1, &tank2 }; // 탱크도 배열로 선언해둠
	tanks[0]->turn(RIGHT); // 탱크1은 오른쪽을 봄(SetScale적용해서 반대로 돌림)
	tanks[0]->setPosition({ 100.0f, floorY }); // 위치지정
	tanks[1]->setPosition({ 695.0f, floorY }); // 위치지정

	//각도 선 객체
	RectangleShape line({ 50.0f, 1.5f }); // https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php 참조했음
	line.setFillColor(Color::Black); 
	line.setRotation(-30);
	line.setPosition({ 69.0f, 562.0f });

	int wind = 0; // 바람 0
	int power = 0; // 힘 0
	int bullet = 0; // 총알 0
	int explosionTime = 0; // 폭발시간 = 0
	float bulletTime = 0.0f; // 총알시간 = 0.0
	float shootDegree = 30.0f; // 슛 각도(초기30도)
	int turn = LEFT; // 탱크의 회전방향
	bool charging = false; // 초기값을 false로 줘서 스페이스 다시 한번 누르면 true조건문 진입
	bool shooting = false; // charging이 true일때 shooting도 true로 설정
	bool explosion = false; // 포탄이 발사 된 후 true로 설정


	// ==================== 메인 ==================== //
	while (window.isOpen())
	{
		// 이벤트 처리
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close(); // 창을 닫으면 윈도우창이 닫힘
				break;

			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Escape)) // Esc키를 누를때 윈도우 창이 닫힘
				{
					window.close();
				}
				if (Keyboard::isKeyPressed(Keyboard::Q)) // Q를 누를 때
				{
					bullet = turn * 2; // 총알(int) = 1,2번 탄환
					selector.setPosition({ 650.0f, 565.0f }); // 포탄 선택자의 위치를 왼쪽으로 이동시켜줌
				}
				else if (Keyboard::isKeyPressed(Keyboard::W)) // W를 누를 때
				{
					bullet = turn * 2 + 1; // 총알(int) = 3,4번 탄환
					selector.setPosition({ 700.0f, 565.0f }); // 포탄 선택자의 위치를 오른쪽으로 이동시켜줌
				}
				else if (Keyboard::isKeyPressed(Keyboard::Space) && !shooting)
				{
					if (charging)
					{
						bulletTime = 0;
						shooting = true;
						charging = false;

						powerbar.setScale({ 0.0f, 1.0f });
						bullets[bullet]->setPosition(tanks[turn]->getPosition());
						bullets[bullet]->move({ tanks[turn]->direction == RIGHT ? 20.0f : -20.0f, -10.0f });
					}
					else
					{
						power = 0;
						charging = true;
					}
				}
				break;
			default:
				break;
			}
		}

		// 움직임 이벤트는 별도로 처리
		if (!shooting && !charging)
		{
			if (Keyboard::isKeyPressed(Keyboard::Up) && shootDegree < 60.0f)
			{
				shootDegree += 0.5f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) && shootDegree > 30.0f)
			{
				shootDegree -= 0.5f;
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) // 왼쪽 키 누를 때
			{
				Vector2f tankPos = tanks[turn]->getPosition();
				tanks[turn]->turn(LEFT);
				if (tankPos.x > 20) // 화면 밖으로 나가지 않게 해주기 위함
				{
					tanks[turn]->move({ -tankSpeed, 0.0f });
				}
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) // 오른쪽 키 누를 때
			{
				Vector2f tankPos = tanks[turn]->getPosition();
				tanks[turn]->turn(RIGHT);
				if (tankPos.x < windowW - 20) // 화면 밖으로 나가지 않게 해주기 위함
				{
					tanks[turn]->move({ tankSpeed, 0.0f });
				}
			}
			// 각도 회전
			line.setRotation(tanks[turn]->direction == LEFT ? shootDegree - 180 : -shootDegree); 
		}

		// 파워 조절
		if (charging)
		{
			power = (power + 1) % 100;
			powerbar.setScale({ power / 100.0f * 17.0f, 1 });
		}


		// 포탄 발사
		if (shooting && !explosion)
		{
			float realPower = 2.5f + power / 12.5f; // 실제 파워 공식

			// x거리 = 실제 힘 * cos(각도/라디안) * 탱크의 방향이 우측이면 1, 좌측이면 -1 * 바람/3.5f
			float x = realPower * cos(shootDegree / A_PI) * (tanks[turn]->direction == RIGHT ? 1 : -1) + (wind / 3.5f); 
			
			// y거리 = 중력 * 총알시간 - 실제 힘 * sin(각도/라디안);
			float y = G * bulletTime - realPower * sin(shootDegree / A_PI);
			bullets[bullet]->move({ x, y }); 
			bullets[bullet]->setRotation(atan2(y, x) * A_PI);
			bulletTime += 1.0f / frameRate;

			// 포탄 처리
			Vector2f tankPos = tanks[!turn]->getPosition();
			Vector2f bulletPos = bullets[bullet]->getPosition();
			if (abs((tankPos.x) - (bulletPos.x)) < 20.0f &&
				abs((tankPos.y) - (bulletPos.y)) < 20.0f ||
				bulletPos.x < -100 || bulletPos.x > windowW + 100 ||
				bulletPos.y < -50 || bulletPos.y > floorY + 15)
			{
				explosion = true;
				bomb.setPosition(bullets[bullet]->getPosition());
				bomb.setScale({ bullets[bullet]->range / 100.0f, bullets[bullet]->range / 100.0f });
			}
		}



		// 포탄 폭발 시 처리 
		if (explosion && ++explosionTime == 100)
		{
			// 적이 맞았을 때
			Vector2f tankPos = tanks[!turn]->getPosition();
			Vector2f bombPos = bomb.getPosition();
			float x = (bullets[bullet]->range * 3.0f) / 2;
			float y = abs((tankPos.x) - (bombPos.x));
			if (x > y)
			{
				if (y <= 20) y = 0;
				tanks[!turn]->damage(tanks[turn]->attack * bullets[bullet]->attack * (1 - y / x));
			}
			if (tanks[!turn]->hp <= 0)
			{
				shooting = false;
				explosion = false;
				break;
			}

			// 턴 바꿈
			shooting = false;
			explosion = false;
			explosionTime = 0;
			turn = !turn;
			bullet = turn * 2;
			wind = rand() % 15 - 7;
			shootDegree = 30.0f;
			hpbar.setScale({ tanks[turn]->hp / 1000.0f * 17.0f, 1.0f });
			windbar.setScale({ float(wind), 1.0f });
			selector.setPosition({ 650.0f, 565.0f });

			// 내가 맞았을 때
			tankPos = tanks[!turn]->getPosition();
			y = abs((tankPos.x) - (bombPos.x));
			if (x > y)
			{
				if (y <= 20) y = 0;
				tanks[!turn]->damage(tanks[turn]->attack * bullets[bullet]->attack * (1 - y / x));
			}
			if (tanks[!turn]->hp <= 0)
			{
				shooting = false;
				explosion = false;
				break;
			}
			cout << "1P : " << tanks[0]->hp << "\n2P : " << tanks[1]->hp << "\n\n";
		}


		// 그리기
		window.clear();
		window.draw(background);
		window.draw(statusbar);
		window.draw(line);
		window.draw(hpbar);
		window.draw(windbar);
		window.draw(powerbar);
		window.draw(selector);
		window.draw(*tanks[0]);
		window.draw(*tanks[1]);
		window.draw(bullett[turn * 2]);
		window.draw(bullett[turn * 2 + 1]);
		if (explosion)
			window.draw(bomb);
		else if (shooting)
			window.draw(*bullets[bullet]);
		window.display();
	}

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					window.close();
				}
			}
		}

		window.clear();
		window.draw(background);
		window.draw(statusbar);
		window.draw(line);
		window.draw(hpbar);
		window.draw(windbar);
		window.draw(powerbar);
		window.draw(selector);
		window.draw(*tanks[0]);
		window.draw(*tanks[1]);
		window.draw(bullett[turn * 2]);
		window.draw(bullett[turn * 2 + 1]);
		window.draw(*wins[turn]);
		window.display();
	}
}