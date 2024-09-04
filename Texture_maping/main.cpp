#include "main_wiindow.h"
#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include "filtration.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    auto scene_window = new QWidget;
    auto scene = new Main_widget(scene_window);
    scene->setMinimumSize(720, 720);
    scene->setFocusPolicy(Qt::StrongFocus);

    auto scene_layout = new QHBoxLayout(scene_window);
    scene_layout->addWidget(scene);
    scene_window->setLayout(scene_layout);

    auto controls_window = new QWidget;
    auto controls_layout = new QVBoxLayout(controls_window);

    auto fps_label = new QLabel("", controls_window);
    auto fps = new QLabel(controls_window);

    auto mipmap = new QCheckBox("Mipmap", controls_window);
    auto anisotropy = new QCheckBox("Anisotropy", controls_window);

    auto filtration_label = new QLabel("Filtration:", controls_window);
    auto filtration_nearest_mipmap_nearest = new QRadioButton("NearestMipMapNearest", controls_window);
    auto filtration_nearest_mipmap_linear = new QRadioButton("NearestMipMapLinear", controls_window);
    auto filtration_linear_mipmap_nearest = new QRadioButton("LinearMipMapNearest", controls_window);
    auto filtration_linear_mipmap_linear = new QRadioButton("LinearMipMapLinear", controls_window);

    controls_layout->addStretch();
    controls_layout->addWidget(fps_label);
    controls_layout->addWidget(fps);
    controls_layout->addWidget(mipmap);
    controls_layout->addWidget(anisotropy);
    controls_layout->addWidget(filtration_label);
    controls_layout->addWidget(filtration_nearest_mipmap_nearest);
    controls_layout->addWidget(filtration_nearest_mipmap_linear);
    controls_layout->addWidget(filtration_linear_mipmap_nearest);
    controls_layout->addWidget(filtration_linear_mipmap_linear);
    controls_layout->addStretch();

    QObject::connect(&scene->fps_counter(), &Fps_counter::fps_changed, [=](size_t new_value) {
        fps->setText(QString::number(new_value));
        });
    QObject::connect(mipmap, &QCheckBox::clicked, [=](bool checked) { scene->enable_mipmap(checked); });
    QObject::connect(anisotropy, &QCheckBox::clicked, [=](bool checked) { scene->enable_anisotropy(checked); });

    QObject::connect(filtration_nearest_mipmap_nearest, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            scene->set_filtration(Filtration::NearestMipMapNearest);
        }
        });
    QObject::connect(filtration_nearest_mipmap_linear, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            scene->set_filtration(Filtration::NearestMipMapLinear);
        }
        });
    QObject::connect(filtration_linear_mipmap_nearest, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            scene->set_filtration(Filtration::LinearMipMapNearest);
        }
        });
    QObject::connect(filtration_linear_mipmap_linear, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            scene->set_filtration(Filtration::LinearMipMapLinear);
        }
        });

    fps_label->setAlignment(Qt::AlignHCenter);
    fps->setAlignment(Qt::AlignHCenter);

    mipmap->setChecked(true);
    anisotropy->setChecked(true);
    filtration_linear_mipmap_linear->setChecked(true);

    mipmap->setFocusPolicy(Qt::NoFocus);
    anisotropy->setFocusPolicy(Qt::NoFocus);
    filtration_nearest_mipmap_nearest->setFocusPolicy(Qt::NoFocus);
    filtration_nearest_mipmap_linear->setFocusPolicy(Qt::NoFocus);
    filtration_linear_mipmap_nearest->setFocusPolicy(Qt::NoFocus);
    filtration_linear_mipmap_linear->setFocusPolicy(Qt::NoFocus);

    controls_window->setLayout(controls_layout);
    scene_window->show();
    controls_window->show();

    return a.exec();
}
