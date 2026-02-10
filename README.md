# 🚀 Battle Blaster (Unreal Engine 5 C++)

**Udemy의 'Unreal Engine 5 C++ Developer' 과정을 기반으로 확장 제작된 탑다운 아케이드 탱크 슈팅 게임입니다.**
기본적인 Pawn 상속 구조와 발사체(Projectile) 로직을 넘어, **GameInstance를 활용한 데이터 지속성(로그라이크 요소)**, **Enhanced Input 시스템을 통한 정교한 조작 제어**, 그리고 **GameMode 기반의 레벨 관리 시스템**을 통해 완성도 높은 게임플레이 사이클을 구축하는 데 집중했습니다.

---

## 📺 Project Showcase
- **Demonstration Video:** [유튜브 링크]

---

## 🛠 Core Technical Implementations (C++)

### 🎯 Key Source Code: `GameMode`, `GameInstance` & `EnhancedInput`
단순한 슈팅을 넘어, 게임의 규칙을 관리하고 레벨 간 데이터를 유지하는 메커니즘을 C++ 클래스로 구조화하여 구현했습니다.

#### **1. 데이터 지속성 및 로그라이크 시스템 (GameInstance)**
- **Persistent Data:** 레벨이 재시작되거나 변경되어도 데이터가 유지되는 `UGameInstance`를 활용하여 파괴된 타워의 정보를 저장했습니다.
- **Roguelike Element:** 플레이어가 사망하여 레벨이 재시작될 때, 이미 파괴한 타워는 다시 스폰되지 않도록 구현하여 '도전의 연속성'을 부여했습니다. `TArray<FString>`을 사용하여 파괴된 액터의 ID를 관리합니다.

#### **2. 향상된 입력 시스템 제어 (Enhanced Input & Player Controller)**
- **Input Mapping Context:** 기존의 레거시 입력 방식 대신 `UEnhancedInputComponent`를 사용하여 이동, 회전, 발사를 구현했습니다.
- **Dynamic Input Control:** 게임 시작 전 카운트다운(3, 2, 1)이나 게임 오버 시, `RemoveMappingContext`를 호출하여 플레이어의 조작을 물리적으로 차단하고 게임의 흐름(Flow)을 제어했습니다.

#### **3. 게임 루프 및 승리/패배 로직 (GameModeBase)**
- **Timer & Delegate:** `FTimerHandle`을 사용하여 게임 시작 카운트다운, 사망 후 리스폰 딜레이, 레벨 이동 간의 타이밍을 정교하게 조절했습니다.
- **Dynamic Level Counting:** `BeginPlay` 시점에서 맵에 배치된 타워의 개수를 동적으로 파악하되, GameInstance에 저장된 '이미 파괴된 타워'는 카운트에서 제외하는 로직을 추가하여 승리 조건을 정확하게 계산했습니다.

---

## 📦 Assets & Resources
- **Logic:** 탱크 이동, 포탑 회전, 발사체 탄도학, AI 포탑 로직 등 핵심 기능은 100% C++로 직접 구현되었습니다.
- **Environment Assets:** 아케이드 스타일의 비주얼을 위해 외부 에셋 팩을 활용하였습니다.
  - *Toon Tanks Asset Pack* (저장소 용량 최적화를 위해 `.gitignore` 처리됨)

---

## 💡 Problem Solving (Troubleshooting)

프로젝트 개발 과정에서 발생한 주요 이슈와 이를 해결하기 위한 기술적 접근 과정입니다.

### **1. 재시작 시 승리 조건 오류 (Logic Error)**
* **Problem:** 로그라이크 시스템 도입 후, 타워를 파괴하고 재시작했을 때 파괴된 타워는 스폰되지 않지만, `GameMode`가 `BeginPlay`에서 타워 개수를 셀 때 이미 죽은(없는) 타워까지 포함하여 세는 바람에 타워를 다 부숴도 승리(Victory)가 뜨지 않는 현상 발생.
* **Solution:**
    * 타워 개수를 세는 `for` 루프 내부에 `GameInstance->IsTowerDestroyed(TowerName)` 체크 로직을 추가.
    * 이미 파괴된 목록에 있는 타워는 `TowerCount` 집계에서 `continue`로 건너뛰도록 수정하여 정확한 남은 타워 개수를 산출함.

### **2. 게임 시작 전 움직임 제어 (Input Handling)**
* **Problem:** 게임 시작 카운트다운(3, 2, 1)이 진행되는 동안 플레이어가 미리 이동하거나 발사할 수 있어 공정성이 떨어지고 연출이 어색함. 단순 `DisableInput`은 Enhanced Input 시스템에서 간혹 동작하지 않는 문제 확인.
* **Solution:**
    * `Tank` 클래스에 `SetPlayerEnabled(bool)` 함수를 구현하고, `UEnhancedInputLocalPlayerSubsystem`을 호출.
    * `AddMappingContext`와 `RemoveMappingContext`를 사용하여 입력 매핑 자체를 동적으로 연결/해제하는 방식으로 확실하게 조작을 제어함.

### **3. 사망 후 즉시 재시작 문제 (UX Improvement)**
* **Problem:** 플레이어 사망 시 딜레이 없이 0.1초 만에 바로 레벨이 재시작되어, 파괴된 연출을 볼 수 없고 게임의 템포가 너무 급박함.
* **Solution:**
    * `ActorDied` 함수 내에서 즉시 `RestartLevel`을 호출하지 않고, `FTimerHandle`을 사용하여 2.0초의 딜레이를 부여.
    * 딜레이 동안 카메라와 파괴 효과(Particle)는 유지되도록 하여 플레이어가 상황을 인지할 시간을 제공.

### **4. 패키징 후 레벨 이동 불가 (Packaging Issue)**
* **Problem:** 에디터에서는 정상 작동하던 레벨 이동이, `.exe`로 빌드(Packaging)한 후에는 1레벨 클리어 후 다음 레벨로 넘어가지 않고 1레벨이 반복되는 현상.
* **Solution:**
    * 언리얼 엔진이 기본적으로 현재 열려있는 맵만 패키징에 포함하는 특성 확인.
    * `Project Settings` > `Packaging` > `List of maps to include in a packaged build` 항목에 Level 1, 2, 3을 모두 수동으로 등록하여 해결.

---

## 🔧 Tech Stack
- **Engine:** Unreal Engine 5.4
- **Language:** C++ (Visual Studio 2022)
- **Version Control:** Git, GitHub
