#pragma once
#include <QKeyEvent>
#include "camera.h"

constexpr double sens = 0.0001;

class Camera_controller final {
public:
    void key_press_event(QKeyEvent* event) {
        switch (event->key()) {
        case Qt::Key_W:
            m_moving_up = true;
            break;
        case Qt::Key_A:
            m_moving_left = true;
            break;
        case Qt::Key_S:
            m_moving_down = true;
            break;
        case Qt::Key_D:
            m_moving_right = true;
            break;
        }
    }

    void key_released_event(QKeyEvent* event)
    {
        switch (event->key()) {
        case Qt::Key_W:
            m_moving_up = false;
            break;
        case Qt::Key_A:
            m_moving_left = false;
            break;
        case Qt::Key_S:
            m_moving_down = false;
            break;
        case Qt::Key_D:
            m_moving_right = false;
            break;
        }
    }

    void update(Camera& camera)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_update).count();

        QVector3D offset{ 0, 0, 0 };
        offset[0] += m_moving_right ? -duration * sens : 0;
        offset[0] += m_moving_left ? duration * sens : 0;
        offset[2] += m_moving_up ? duration * sens : 0;
        offset[2] += m_moving_down ? -duration * sens : 0;

        camera.move(std::move(offset));
        m_last_update = now;
    }

private:
    bool m_moving_right{ false };
    bool m_moving_left{ false };
    bool m_moving_up{ false };
    bool m_moving_down{ false };

    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_update;
};