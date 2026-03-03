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

## 👨‍💻 개발자 (Developer)

* **양평화** - *Main Developer*

---

## 🏗️ Software Architecture

### 1. 시스템 아키텍처 (High-Level Architecture)

게임은 크게 **Engine Core**, **Managers**, **Game Objects**의 3계층 구조로 설계되었습니다.

* **⚙️ Engine Core**
    * `WinMain`에서 시작하여 **Device**(DirectX 초기화), **Camera**(시점 제어), **Sprite**(렌더링)와 같은 하위 시스템을 제어합니다.
    * 게임의 기반이 되는 하드웨어 리소스와 렌더링 파이프라인을 담당합니다.

* **🛠️ Managers (Singleton Pattern)**
    * 게임의 전역 상태를 효율적으로 관리하기 위해 싱글톤 패턴을 사용합니다.
    * `Game` 클래스가 메인 루프(`Update`, `Render`)를 실행하며 각 매니저를 호출합니다.
    * **MapManager**: 절차적 맵 생성 알고리즘의 핵심입니다. 맵 데이터(`MapList`)와 6x6 그리드(`m_Grid`)를 관리합니다.
    * **TimeManager**: `timeScale`을 조절하여 피격 시 히트 스톱(Hit Stop)이나 슬로우 모션 효과를 전역적으로 적용합니다.

* **👾 Game Objects**
    * 실제 게임 월드에 존재하는 객체들입니다.
    * `Knight`(플레이어), `Boss`, `Bird` 등은 공통 부모 클래스(또는 인터페이스)를 가지며, 각자의 `Update`, `Render` 메서드를 통해 독립적으로 행동합니다.

---

### 2. 핵심 알고리즘 상세 (Core Algorithms)

#### A. 🗺️ 절차적 맵 생성 (Procedural Generation)
* **알고리즘**: Random Walk + Grid based Assembly
* **생성 프로세스**:
    1. **초기화**: 6x6 빈 그리드를 생성합니다.
    2. **시작**: 시작 방을 배치하고, 큐(Queue)에 '연결 가능한 문(Open Door)' 위치 정보를 등록합니다.
    3. **확장**: 큐에서 문을 하나 랜덤하게 선택(Pop)하여, 호환되는 프리팹을 배치합니다. (이때 그리드 경계 및 충돌 검사 수행)
    4. **종료 조건**: 맵이 오른쪽 끝(x=5)에 도달하면 보스 대기실을 배치하고 생성을 종료합니다.
    5. **마감 (Wall Capping)**: 생성 완료 후 연결되지 않고 남은 문들은 벽으로 막아 맵을 완성합니다.

#### B. ⚛️ 물리 엔진 (Custom Physics)
* **AABB 충돌 처리**: `IntersectRect` 알고리즘을 사용하여 객체 간의 직사각형 충돌을 감지합니다.
* **축 분리 (Axis Separation)**: 대각선 이동 시 벽 끼임 현상을 방지하기 위해 이동과 충돌 처리를 축별로 분리했습니다.
    > `X축 이동` → `X축 충돌 보정` → `Y축 이동` → `Y축 충돌 보정`
* **상태 보존 (State Preservation)**: 맵(방) 전환 시 플레이어의 상대 좌표와 속도 벡터(`gravity`)를 그대로 유지하여, 끊김 없는 유기적인 월드 이동 경험을 제공합니다.

#### C. 🤖 보스 FSM (Finite State Machine)
보스(`Boss.cpp`)는 현재 상태(`m_State`)에 따라 매 프레임 행동을 결정합니다.

* **States (상태)**:
    * `IDLE`: 대기 및 탐색
    * `TRACE`: 플레이어 추적
    * `ATTACK`: 공격 (근접/원거리)
    * `SKILL`: 특수 스킬 사용
    * `DIE`: 사망 처리
* **Transition (전이)**:
    * 거리, 체력, 쿨타임 등의 조건에 따라 유동적으로 상태가 변경됩니다.
    * *예시: 플레이어와의 거리가 멀어지면 `IDLE` ➡️ `TRACE`로 상태 전이*
