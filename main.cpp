#include "widget.h"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создание главного окна
    QWidget window;
    window.setWindowTitle("Заключение офтальмолога");

    // Создание элементов управления
    QLabel *labelDiagnosis = new QLabel("Диагноз:");
    QTextEdit *textEditDiagnosis = new QTextEdit();
    QPushButton *buttonSave = new QPushButton("Сохранить");

    // Расположение элементов на форме
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(labelDiagnosis);
    layout->addWidget(textEditDiagnosis);
    layout->addWidget(buttonSave);

    // Назначение разметки для главного окна
    window.setLayout(layout);

    // Обработка нажатия кнопки "Сохранить"
    QObject::connect(buttonSave, &QPushButton::clicked, [&]() {
        QString diagnosis = textEditDiagnosis->toPlainText(); // Получение введенного диагноза

        // Запись заключения в текстовый файл
        QFile file("заключение_офтальмолога.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "Заключение офтальмолога:\n";
            stream << "Диагноз: " << diagnosis << "\n";
            file.close();
            QMessageBox::information(&window, "Успех", "Заключение сохранено в файл 'заключение_офтальмолога.txt'.");
        } else {
            QMessageBox::critical(&window, "Ошибка", "Не удалось сохранить заключение.");
        }
    });

    // Отображение главного окна
    window.show();

    return app.exec();
}
