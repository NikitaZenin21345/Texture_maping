#pragma once
class Tile final {
public:
    [[nodiscard]]
    explicit Tile(QVector3D position, QVector3D rotation = { 0, 0, 0 }) noexcept :
        m_position(std::move(position)), m_rotation(std::move(rotation)) {}


    [[nodiscard]]
    Tile() = default;

    [[nodiscard]]
    QMatrix4x4 model() const
    {
        QMatrix4x4 model;
        model.translate(m_position);

        model.rotate(m_rotation[0], 1, 0, 0);
        model.rotate(m_rotation[1], 0, 1, 0);
        model.rotate(m_rotation[2], 0, 0, 1);

        return model;
    }

private:
    QVector3D m_position{ 0, 0, 0 };
    QVector3D m_rotation{ 0, 0, 0 };
};