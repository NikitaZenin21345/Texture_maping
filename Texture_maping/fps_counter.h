#pragma once
#include <chrono>
#include <qobjectdefs.h>
#include <QObject>

constexpr std::chrono::duration m_measure_interval{ std::chrono::milliseconds(400) };

class Fps_counter final : public QObject {
    Q_OBJECT
public:
    [[nodiscard]]
    explicit Fps_counter()
    {
        m_last_update = std::chrono::high_resolution_clock::now();
    }

    void update();

signals:
    void fps_changed(size_t fps);

private:


    size_t m_frames{ 0 };
    size_t m_fps{ 0 };
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_update;
};

