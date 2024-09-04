#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class Camera final {
public:
    [[nodiscard]]
    Camera(QVector3D position) noexcept :
        m_position(std::move(position)), m_direction({ 0, 0, 1 }), m_direction_normal({ 0, 1, 0 })
    {

    }
    [[nodiscard]]
    explicit Camera() noexcept : Camera({ 0, 0, 0 }){}

    [[nodiscard]]
    const QVector3D& position() const noexcept
    {
        return m_position;
    }

    [[nodiscard]]
    QMatrix4x4 view() const
    {
        QMatrix4x4 view;

        QVector3D looking_at = m_position + m_direction;
        view.lookAt(m_position, looking_at, m_direction_normal);
        return view;
    }

    void position(QVector3D position) noexcept
    {
        m_position = std::move(position);
    }


    void move(QVector3D offset) noexcept {
        m_position += offset;
    }

private:
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_direction_normal;
};


