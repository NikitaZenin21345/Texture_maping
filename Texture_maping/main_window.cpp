#include <QOpenGLTexture>
#include "main_wiindow.h"

namespace {
    struct Vertex {
        QVector3D position;
        QVector3D normal;
        QVector2D texture_coords;
    };

    QVector<Vertex> create_tile_vertices() {
        return {
            { { -0.5f, -0.5f, 0.0f }, { 0, 0, -1 }, { 0, 0 } },
            { { 0.5f, -0.5f, 0.0f }, { 0, 0, -1 }, { 1, 0 } },
            { { 0.5f, 0.5f, 0.0f }, { 0, 0, -1 }, { 1, 1 } },
            { { -0.5f, -0.5f, 0.0f }, { 0, 0, -1 }, { 0, 0 } },
            { { 0.5f, 0.5f, 0.0f }, { 0, 0, -1 }, { 1, 1 } },
            { { -0.5f, 0.5f, 0.0f }, { 0, 0, -1 }, { 0, 1 } }
        };
    }

    QVector<Tile> generate_tiles() {
        QVector<Tile> tiles;
        for (float z = 0; z <= 100.0f; z += 1.0f) {
            tiles.push_back(Tile({ 0, 0, z }, { -90, 0, 0 }));
        }
        return tiles;
    }

    const QVector<QString> texture_files{ "road_1.jpg", "road_2.jpg", "road_3.jpg" };
}

Main_widget::Main_widget(QWidget* parent) : QOpenGLWidget(parent), m_camera({ 0, 0.2, 0 }), m_tiles(generate_tiles()) {}

const Fps_counter& Main_widget::fps_counter() const noexcept {
    return m_fps_counter;
}

void Main_widget::set_filtration(Filtration filtration) {
    filtration_methods.at(filtration)(this);
}

void Main_widget::enable_mipmap(bool enable) {
    for (const auto& texture : m_textures) {
        texture->setMipMaxLevel(enable ? texture->mipLevels() : 0);
    }
}

void Main_widget::enable_anisotropy(bool enable) {
    for (const auto& texture : m_textures) {
        texture->setMaximumAnisotropy(enable ? 16.0f : 1.0f);
    }
}

void Main_widget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexRoad.glsl");
    m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentRoad.glsl");

    const auto tile_vertices = create_tile_vertices();
    m_tile_buffer.create();
    m_tile_buffer.bind();
    m_tile_buffer.allocate(tile_vertices.constData(), tile_vertices.size() * sizeof(Vertex));
    m_tile_buffer.release();

    for (const auto& texture_name : texture_files) {

        m_textures.push_back(std::make_unique<QOpenGLTexture>(QImage(texture_name).mirrored()));
    }

    connect(this, &Main_widget::frameSwapped, [=]() {
        update();
        m_camera_controller.update(m_camera);
        m_fps_counter.update();
        });
}

void Main_widget::resizeGL(int w, int h) {}

void Main_widget::paintGL() {
    //m_camera.move({ 0, 0, 0.001 });

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.bind();
    m_tile_buffer.bind();
    for (size_t i = 0; i < m_textures.size(); ++i) {
        m_textures[i]->bind(i);
    }

    QMatrix4x4 projection;
    projection.perspective(60.0f, (width() * retinaScale) / (height() * retinaScale), 0.1f, 100.0f);

    m_shader.setUniformValue("view", m_camera.view());
    m_shader.setUniformValue("projection", projection);
    m_shader.setUniformValue("view_pos", m_camera.position());
    m_shader.setUniformValue("texture0", 0);
    m_shader.setUniformValue("texture1", 1);
    m_shader.setUniformValue("texture2", 2);

    m_shader.enableAttributeArray(0);
    m_shader.enableAttributeArray(1);
    m_shader.enableAttributeArray(2);

    m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    m_shader.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, sizeof(Vertex));
    m_shader.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, sizeof(Vertex));

    for (const auto& tile : m_tiles) {
        m_shader.setUniformValue("model", tile.model());
        glDrawArrays(GL_TRIANGLES, 0, m_tile_buffer.size() / sizeof(Vertex));
    }

    m_shader.disableAttributeArray(0);
    m_shader.disableAttributeArray(1);
    m_shader.disableAttributeArray(2);

    m_tile_buffer.release();
    m_shader.release();
    for (const auto& texture : m_textures) {
        texture->release();
    }
}

void Main_widget::keyPressEvent(QKeyEvent* event) {
    m_camera_controller.key_press_event(event);
}

void Main_widget::keyReleaseEvent(QKeyEvent* event) {
    m_camera_controller.key_released_event(event);
}

void Main_widget::set_filtration_nearest_mipmap_nearest() {
    for (const auto& texture : m_textures) {
        texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        texture->setMinMagFilters(QOpenGLTexture::NearestMipMapNearest, QOpenGLTexture::NearestMipMapNearest);
    }
}

void Main_widget::set_filtration_nearest_mipmap_linear() {
    for (const auto& texture : m_textures) {
        texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        texture->setMinMagFilters(QOpenGLTexture::NearestMipMapLinear, QOpenGLTexture::NearestMipMapLinear);
    }
}

void Main_widget::set_filtration_linear_mipmap_nearest() {
    for (const auto& texture : m_textures) {
        texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        texture->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
    }
}

void Main_widget::set_filtration_linear_mipmap_linear() {
    for (const auto& texture : m_textures) {
        texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::LinearMipMapLinear);
    }
}
