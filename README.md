# C++ SFML(Simple and Fast Multimedia Library) Fortress Game

---

## SFML: OpenGL �� ����� �� GUI(Graphical User Interface)�� �����ϴ� ���̺귯��

### [SFML library Download Link](https://www.sfml-dev.org/download.php)

### IDE: Microsoft Visual Studio Community 2017

---

> Play ScreenShot
> ![image](https://user-images.githubusercontent.com/39071798/103440318-d1422100-4c87-11eb-8034-556fa1b45450.png)
> ![image](https://user-images.githubusercontent.com/39071798/103440324-d606d500-4c87-11eb-9b6b-9d9ba630916d.png)
> ![image](https://user-images.githubusercontent.com/39071798/103440325-da32f280-4c87-11eb-8fd9-46e8bfc75369.png)

## ![image](https://user-images.githubusercontent.com/39071798/103440327-dbfcb600-4c87-11eb-8c8e-f71234a73381.png)

## How to Play

### `SFML_Project.exe` by release

#### �¿����Ű: ������ / ���Ϲ���Ű: �������� / q,w: �̻��� ���� / Space: ����

---

## �ڵ� ���

### Sprite�� ��ӹ��� Object

##### ����: �̹����� Texture, ��ġ�� Vector2f�� ����

---

## Object Ŭ������ ��ӹ��� Bullet, Tank Ŭ����

##### ����: �̹����� Texture, ��ġ�� Vector2f�� ����

---

## ����

```
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
```

---

## Event Function

##### 1. �����¿����Ű

##### 2. Q/W

##### 3. ������

##### 4. ��ũ ���� ȸ��

##### 5. �Ŀ� ����

##### 6. ��ź �߻� - �ٶ� ���� ����, ��ź�� ũ��/������ ����, �����̻��� ����

##### 7. ��ź ���� ó�� - ���� or !����

##### 8. �� ��ü

##### 9. �׸���

---

## �߰��� �����ϰ���� �κ�

### ������

### ������ ������ ��������Ʈ ����

### �� �μ��� - �߶�

### ��ǳ

### �ΰ�����

---

## How to Contribute

### SFML.ppt �����ؼ� vs�� sfml ���̺귯�� �����ؼ� real_main.cpp ����/���� �� `Pull Request` ��û
