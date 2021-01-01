/* ============================== SFML ��Ʈ���� ���� ============================== */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

#define A_PI 57.2957795130823208767 // ����
#define G 9.8f // �߷�
#define LEFT 0 // ����
#define RIGHT 1 // ������

const int frameRate = 60; // ������ �ʴ� 60���� ����
const float tankSpeed = 1.0f; // ��ũ���ǵ� �ʱⰪ
const float tankMaxHP = 1000.0f; // ��ũ ü�� �ʱⰪ
const float windowW = 800.0f; 
const float floorY = 385.0f; 

class Object : public Sprite // ObjectŬ���� : Sprite�� ������
{
public:
	Texture texture;
	Object(const string& path, const Vector2f pos = { 0.0f, 0.0f }, const Vector2f scale = { 1.0f, 1.0f }) // Object ������(���,��ġ,ũ��)
	{
		texture.loadFromFile(path); // ��κҷ�����
		this->setTexture(texture); // �ؽ�������
		this->setPosition(pos); // ��ġ����
		this->setScale(scale); // ũ�� ����
	}
	void center() // ��ũ�� �Ѿ� ���� ��ǥ������ ���� ������� �Ǿ��ִµ� �߰����� �ٲ��ִ� �Լ�
	{
		const Vector2u size = texture.getSize();
		this->setOrigin({ size.x / 2.0f, size.y / 2.0f });
	}
};

class Bullet : public Object // �Ѿ� Ŭ����
{
public:
	float range, attack; // ����, ���ݷ�
	Bullet(const string& path, const float _range, const float _attack) : Object(path) // ���, ����, ���ݷ�
	{
		this->center();
		range = _range; // ��������
		attack = _attack; // ���ݷ�����
	}
};

class Tank : public Object // ��ũ Ŭ����
{
public:
	float attack, defense; // ���ݷ�, ����
	float hp = tankMaxHP; // hp�� �ʱⰪ(1000)���� �ʱ�ȭ
	bool direction = LEFT; // ������ ����
	Tank(const string& path, const float _attack, const float _defense) : Object(path) // TankŬ���� ������(���,���ݷ�,����)
	{
		this->setOrigin(20.0f, 20.0f); // ������ġ�� ������(setScale�ϰ����ڿ�������)
		attack = _attack;
		defense = _defense;
	}
	void turn(bool dir) // ����
	{
		direction = dir; // direction�� dir�� �ʱ�ȭ
		this->setScale({ dir == LEFT ? 1.0f : -1.0f, 1.0f }); // dir�� �����̸� -1.0f , �ƴϸ� 1.0f�� ����
	}
	void damage(float dmg) // ������(dmg)
	{
		//cout << "HP: " << hp << " - ";
		hp -= dmg *= (1.0f - defense / 100.0f); // hp = hp - ������ * (1.0f - ���� / 100.0f);
		//cout << dmg << " = " << hp << endl;
	}
};

int main()
{
	// ==================== �Ұ� ==================== //
	cout << "========== �ȳ��ϼ���! 2015726100 �̽º��� ��Ʈ���� �����Դϴ�!!! ==========" << endl << endl;
	cout << "�� ������ ������ ������, ��ź�� �׻� ���� ��ź���� �����˴ϴ�!!"<<endl << endl;
	cout << "�� ��ź�� �������� �ٸ���, <Q, W>��ư���� ������ �����մϴ�" << endl << endl;
	cout << "�¿� ����Ű�� �̵��� �����ϸ� ���� ����Ű�δ� ���������� �����մϴ�" << endl << endl;
	cout << "SpaceŰ�� �ѹ� ������ �������� ������ �ѹ� �� ������ �߻��մϴ�" << endl << endl;
	cout << "======================= �׷� ��ſ� ���ӵǼ���!!^^�� =======================" << endl << endl;

	// ==================== �ʱ� �۾� ==================== //
	RenderWindow window(VideoMode(800, 600), "SFML works!", Style::Close | Style::Titlebar);
	window.setFramerateLimit(frameRate); // �������� �����ش�
	srand(time(NULL)); // ������ �������� srand

	// ���, �����̻� �� �⺻ ��ü(����)�� �ҷ�����
	Object background("images/map_skybridge.png"); // �� ���
	Object statusbar("images/statusbar.png", { 0, 495.0f }); // �����̻� ��
	Object hpbar("images/bar_gauge.png", { 210.0f, 515.0f }, { 17.0f, 1.0f }); // HP ��
	Object powerbar("images/bar_gauge.png", { 210.0f, 550.0f }, { 0, 1.0f }); // �� ������(�ʱ�0�̹Ƿ� �Ⱥ���)
	Object windbar("images/wind_gauge.png", { 69.0f, 564.0f }, { 0, 0 }); // �ٶ�������
	Object selector("images/missile_selector.png", { 650.0f, 565.0f }, { 0.5f, 0.5f }); // �̻��� ����
	Object bomb("images/bomb.png");
	bomb.center();

	// �̰��� �� ��ü
	Object win1("images/1pwin.png", { 250.0f, 150.0f });
	Object win2("images/2pwin.png", { 250.0f, 150.0f });
	Object* wins[2] = { &win1, &win2 }; // �¸��� ���� �迭�� �̸� �����ص�

	// �Ѿ� ��ü(����)�� �ҷ�����
	Bullet bullet1("images/missile_canon_1.png", 100.0f, 0.6f); // ĳ���� �Ѿ�1
	Bullet bullet2("images/missile_canon_2.png", 20.0f, 2.0f); // ĳ���� �Ѿ�2
	Bullet bullet3("images/missile_missile_1.png", 40.0f, 1.2f); // �̻����� �Ѿ�1
	Bullet bullet4("images/missile_missile_2.png", 20.0f, 0.3f); // �̻����� �Ѿ�2
	Bullet bullett[4] = { bullet1, bullet2, bullet3, bullet4 }; // �̻��� ���� �迭
	Bullet* bullets[4] = { &bullet1, &bullet2, &bullet3, &bullet4 }; // �߻��� �� ǥ���ϴ� �迭
	//��ġ����
	bullett[0].setPosition({ 660.0f, 545.0f }); 
	bullett[1].setPosition({ 710.0f, 545.0f });
	bullett[2].setPosition({ 660.0f, 545.0f });
	bullett[3].setPosition({ 710.0f, 545.0f });

	//�⺻ ��ũ �ҷ�����
	Tank tank1("images/tank_canon.png", 300.0f, 30.0f); // ��ũ1�� ���ݷ� 300, ���� 30
	Tank tank2("images/tank_missile.png", 200.0f, 40.0f); // ��ũ2�� ���ݷ� 200, ���� 40
	Tank* tanks[2] = { &tank1, &tank2 }; // ��ũ�� �迭�� �����ص�
	tanks[0]->turn(RIGHT); // ��ũ1�� �������� ��(SetScale�����ؼ� �ݴ�� ����)
	tanks[0]->setPosition({ 100.0f, floorY }); // ��ġ����
	tanks[1]->setPosition({ 695.0f, floorY }); // ��ġ����

	//���� �� ��ü
	RectangleShape line({ 50.0f, 1.5f }); // https://www.sfml-dev.org/tutorials/2.4/graphics-shape.php ��������
	line.setFillColor(Color::Black); 
	line.setRotation(-30);
	line.setPosition({ 69.0f, 562.0f });

	int wind = 0; // �ٶ� 0
	int power = 0; // �� 0
	int bullet = 0; // �Ѿ� 0
	int explosionTime = 0; // ���߽ð� = 0
	float bulletTime = 0.0f; // �Ѿ˽ð� = 0.0
	float shootDegree = 30.0f; // �� ����(�ʱ�30��)
	int turn = LEFT; // ��ũ�� ȸ������
	bool charging = false; // �ʱⰪ�� false�� �༭ �����̽� �ٽ� �ѹ� ������ true���ǹ� ����
	bool shooting = false; // charging�� true�϶� shooting�� true�� ����
	bool explosion = false; // ��ź�� �߻� �� �� true�� ����


	// ==================== ���� ==================== //
	while (window.isOpen())
	{
		// �̺�Ʈ ó��
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close(); // â�� ������ ������â�� ����
				break;

			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Escape)) // EscŰ�� ������ ������ â�� ����
				{
					window.close();
				}
				if (Keyboard::isKeyPressed(Keyboard::Q)) // Q�� ���� ��
				{
					bullet = turn * 2; // �Ѿ�(int) = 1,2�� źȯ
					selector.setPosition({ 650.0f, 565.0f }); // ��ź �������� ��ġ�� �������� �̵�������
				}
				else if (Keyboard::isKeyPressed(Keyboard::W)) // W�� ���� ��
				{
					bullet = turn * 2 + 1; // �Ѿ�(int) = 3,4�� źȯ
					selector.setPosition({ 700.0f, 565.0f }); // ��ź �������� ��ġ�� ���������� �̵�������
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

		// ������ �̺�Ʈ�� ������ ó��
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

			if (Keyboard::isKeyPressed(Keyboard::Left)) // ���� Ű ���� ��
			{
				Vector2f tankPos = tanks[turn]->getPosition();
				tanks[turn]->turn(LEFT);
				if (tankPos.x > 20) // ȭ�� ������ ������ �ʰ� ���ֱ� ����
				{
					tanks[turn]->move({ -tankSpeed, 0.0f });
				}
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) // ������ Ű ���� ��
			{
				Vector2f tankPos = tanks[turn]->getPosition();
				tanks[turn]->turn(RIGHT);
				if (tankPos.x < windowW - 20) // ȭ�� ������ ������ �ʰ� ���ֱ� ����
				{
					tanks[turn]->move({ tankSpeed, 0.0f });
				}
			}
			// ���� ȸ��
			line.setRotation(tanks[turn]->direction == LEFT ? shootDegree - 180 : -shootDegree); 
		}

		// �Ŀ� ����
		if (charging)
		{
			power = (power + 1) % 100;
			powerbar.setScale({ power / 100.0f * 17.0f, 1 });
		}


		// ��ź �߻�
		if (shooting && !explosion)
		{
			float realPower = 2.5f + power / 12.5f; // ���� �Ŀ� ����

			// x�Ÿ� = ���� �� * cos(����/����) * ��ũ�� ������ �����̸� 1, �����̸� -1 * �ٶ�/3.5f
			float x = realPower * cos(shootDegree / A_PI) * (tanks[turn]->direction == RIGHT ? 1 : -1) + (wind / 3.5f); 
			
			// y�Ÿ� = �߷� * �Ѿ˽ð� - ���� �� * sin(����/����);
			float y = G * bulletTime - realPower * sin(shootDegree / A_PI);
			bullets[bullet]->move({ x, y }); 
			bullets[bullet]->setRotation(atan2(y, x) * A_PI);
			bulletTime += 1.0f / frameRate;

			// ��ź ó��
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



		// ��ź ���� �� ó�� 
		if (explosion && ++explosionTime == 100)
		{
			// ���� �¾��� ��
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

			// �� �ٲ�
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

			// ���� �¾��� ��
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


		// �׸���
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