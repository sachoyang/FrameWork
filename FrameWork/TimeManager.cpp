#include "Include.h"

TimeManager::TimeManager() {
    Init();
}

TimeManager::~TimeManager() {}

void TimeManager::Init() {
    ResetTime();
}

void TimeManager::ResetTime() {
    m_HitStopDuration = 0;
    m_GlobalTimeScale = 1.0f;
    m_TimeAccumulator = 0.0f;
    m_TimeSlowDuration = 0;
}

void TimeManager::SetHitStop(DWORD duration) {
    m_HitStopTime = GetTickCount();
    m_HitStopDuration = duration;
}

void TimeManager::SetTimeSlow(float scale, DWORD duration) {
    m_GlobalTimeScale = scale;
    m_TimeSlowStartTime = GetTickCount();
    m_TimeSlowDuration = duration;
}

bool TimeManager::IsTimeStopped() {
    // 1. 역경직(Hitstop) 처리
    if (m_HitStopDuration > 0) {
        if (GetTickCount() - m_HitStopTime < m_HitStopDuration) {
            return true; // 시간 멈춤!
        }
        else {
            m_HitStopDuration = 0;
        }
    }

    // 2. 타임 슬로우 지속 시간 체크 및 해제
    if (m_GlobalTimeScale != 1.0f) {
        if (GetTickCount() - m_TimeSlowStartTime > m_TimeSlowDuration) {
            m_GlobalTimeScale = 1.0f; // 원래 속도 복구
        }
    }

    // 3. 프레임 스킵 (타임 슬로우 연산)
    m_TimeAccumulator += m_GlobalTimeScale;
    if (m_TimeAccumulator < 1.0f) {
        return true; // 프레임 스킵 (느려짐 효과)
    }
    m_TimeAccumulator -= 1.0f;

    return false; // 정상 흐름!
}