# ⚔️ 2D Action Platformer (Project Name)

절차적 맵 생성 시스템과 정교한 2D 물리 엔진을 탑재한 액션 플랫포머 게임입니다.

## 🎮 게임 소개

이 프로젝트는 고전적인 메트로배니아 스타일의 액션 게임으로, 매 플레이마다 변화하는 던전 구조와 전략적인 보스 전투가 특징입니다.

### ✨ 주요 특징

* **🗺️ 절차적 맵 생성 (Procedural Map Generation)**
    * 6x6 그리드 기반의 랜덤 던전 생성 알고리즘
    * 다양한 크기(1x1, 1x2, 2x1, 2x2)의 5가지 프리팹 조합
    * 보스 대기실 자동 배치 및 연결성 보장
* **🤖 다이내믹 보스 AI (FSM)**
    * 유한 상태 머신(Finite State Machine) 기반의 지능형 보스
    * 다중 페이즈 시스템: 체력에 따라 변화하는 공격 패턴
    * 추적, 공격, 대시 등 다양한 행동 패턴 구현
* **Physics & Combat**
    * 직접 구현한 2D 물리 엔진 (충돌 처리, 중력, 관성)
    * 타격감 강화를 위한 히트 스톱(Hit Stop) 시스템 (TimeManager)
    * 맵 이동 시 자연스러운 좌표 및 관성 보정
    * 할로우 나이트 스타일의 '포고(Pogo)' 메커니즘 (하단 공격 점프)

## 🕹️ 조작 방법

| 키 | 동작 | 설명 |
| :---: | :---: | :--- |
| **이동** | `방향키` | 캐릭터 이동 |
| **점프** | `Z` | 길게 누르면 더 높이 점프 (가변 점프) |
| **공격** | `X` | 상/하/좌/우 공격 가능 |
| **대시** | `C` | 공중 대시 지원 (쿨타임 존재) |

## 🛠️ 기술 스택 (Tech Stack)

* **언어:** C++
* **라이브러리:** WinAPI, DirectX 9 (DXUT), FMOD (사운드)
* **툴:** Visual Studio, Aseprite (픽셀 아트)

## 📂 프로젝트 구조

```text
FrameWork
├── Main.cpp          # 프로그램 진입점
├── Game.cpp          # 메인 게임 루프 및 씬 관리
├── Managers          # 싱글톤 관리자 클래스들
│   ├── MapManager    # 맵 생성 및 관리
│   ├── GameManager   # 게임 상태 관리
│   ├── TimeManager   # 시간 조절 (히트 스톱 등)
│   ├── EffectManager # 이펙트 처리
│   ├── UIManager     # UI 렌더링
│   ├── Sound         # 사운드 재생
│   └── Key           # 키 입력 처리
├── Objects           # 게임 오브젝트
│   ├── Player (Knight) # 플레이어 캐릭터
│   ├── Enemy (Boss, Bird) # 적 캐릭터
│   └── Ground        # 지형 및 플랫폼
└── Core              # 핵심 엔진 코드
    ├── Sprite        # 이미지 렌더링
    ├── Collider      # 충돌 처리
    └── Camera        # 카메라 시스템
```

👨‍💻 개발자
양평화 - Main Developer

🏗️ Software Architecture
1. 시스템 아키텍처 (High-Level Architecture)
게임은 크게 Engine Core, Managers, Game Objects의 3계층 구조로 설계되었습니다.

Engine Core: WinMain에서 시작하여 Device(DirectX 초기화), Camera(시점 제어), Sprite(렌더링)와 같은 하위 시스템을 제어합니다.

Managers (Singleton Pattern): 게임의 전역 상태를 관리합니다. Game 클래스가 메인 루프(Update, Render)를 돌리며 각 매니저를 호출합니다.

MapManager: 절차적 맵 생성 알고리즘의 핵심입니다. 맵 데이터(MapList)와 그리드(m_Grid)를 관리합니다.

TimeManager: timeScale을 조절하여 슬로우 모션 효과를 전역적으로 적용합니다.

Game Objects: 실제 게임 월드에 존재하는 객체들입니다. Knight(플레이어), Boss, Bird 등은 공통 부모 클래스(또는 인터페이스)를 가지며, 각자의 Update, Render 메서드를 가집니다.

2. 핵심 알고리즘 상세
A. 절차적 맵 생성 (Procedural Generation)
알고리즘: Random Walk + Grid based Assembly

프로세스:

6x6 빈 그리드 생성.

시작 방 배치 및 큐(Queue)에 연결 가능한 문(Open Door) 등록.

큐에서 랜덤하게 문을 선택하여 호환되는 프리팹 배치 (공간 검사 포함).

오른쪽 끝(x=5) 도달 시 보스 대기실 배치 및 생성 종료.

생성 후 Wall Capping: 연결되지 않은 문을 벽으로 막음.

B. 물리 엔진 (Custom Physics)
AABB 충돌 처리: IntersectRect를 사용하여 객체 간 충돌을 감지합니다.

축 분리(Axis Separation): X축 이동 처리 -> X축 충돌 보정 -> Y축 이동 처리 -> Y축 충돌 보정 순서로 진행하여 대각선 이동 시 끼임 현상을 방지합니다.

상태 보존(State Preservation): 맵 전환 시 플레이어의 상대 좌표와 속도 벡터(gravity)를 유지하여 끊김 없는 경험을 제공합니다.

C. 보스 FSM (Finite State Machine)
보스(Boss.cpp)는 상태(m_State)에 따라 행동이 결정됩니다.

States: IDLE, TRACE, ATTACK, SKILL, DIE

Transition: 거리, 체력, 쿨타임 등의 조건에 따라 상태가 전이됩니다. (예: 플레이어가 멀어지면 IDLE -> TRACE)
