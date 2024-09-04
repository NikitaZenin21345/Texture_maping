#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "tile.h"
#include "camera_controler.h"
#include "fps_counter.h"
#include "filtration.h"

class Main_widget final : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Main_widget(QWidget* parent = nullptr);

    const Fps_counter& fps_counter() const noexcept;

    void set_filtration(Filtration filtration);

    void enable_mipmap(bool enable);

    void enable_anisotropy(bool enable);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void set_filtration_nearest_mipmap_nearest();
    void set_filtration_nearest_mipmap_linear();
    void set_filtration_linear_mipmap_nearest();
    void set_filtration_linear_mipmap_linear();

    Camera m_camera;
    Camera_controller m_camera_controller;
    Fps_counter m_fps_counter;
    QVector<Tile> m_tiles;

    std::vector<std::unique_ptr<QOpenGLTexture>> m_textures;
    QOpenGLBuffer m_tile_buffer;
    QOpenGLShaderProgram m_shader;

    const std::unordered_map<Filtration, std::function<void(Main_widget*)>> filtration_methods{
        { Filtration::NearestMipMapNearest, &Main_widget::set_filtration_nearest_mipmap_nearest },
        { Filtration::NearestMipMapLinear, &Main_widget::set_filtration_nearest_mipmap_linear },
        { Filtration::LinearMipMapNearest, &Main_widget::set_filtration_linear_mipmap_nearest },
        { Filtration::LinearMipMapLinear, &Main_widget::set_filtration_linear_mipmap_linear },
    };
};