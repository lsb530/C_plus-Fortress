# C++ SFML(Simple and Fast Multimedia Library) Fortress Game

---

## SFML: OpenGL 에 기반을 둔 GUI(Graphical User Interface)를 지원하는 라이브러리

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

#### 좌우방향키: 움직임 / 상하방향키: 각도조절 / q,w: 미사일 선택 / Space: 공격

---

## 코드 요약

### Sprite를 상속받은 Object

##### 내부: 이미지는 Texture, 위치는 Vector2f로 동작

---

## Object 클래스를 상속받은 Bullet, Tank 클래스

##### 내부: 이미지는 Texture, 위치는 Vector2f로 동작

---

## 변수

```
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
```

---

## Event Function

##### 1. 상하좌우방향키

##### 2. Q/W

##### 3. 움직임

##### 4. 탱크 각도 회전

##### 5. 파워 조절

##### 6. 포탄 발사 - 바람 공식 적용, 포탄별 크기/데미지 차등, 유도미사일 구현

##### 7. 포탄 폭발 처리 - 명중 or !명중

##### 8. 턴 교체

##### 9. 그리기

---

## 추가로 구현하고싶은 부분

### 아이템

### 데미지 입을시 스프라이트 변경

### 맵 부서짐 - 추락

### 태풍

### 인공지능

---

## How to Contribute

### SFML.ppt 참고해서 vs에 sfml 라이브러리 설정해서 real_main.cpp 수정/개발 후 `Pull Request` 요청
