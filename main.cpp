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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Заключение офтальмолога");

    QLabel *labelDiagnosis = new QLabel("Диагноз:");
    QTextEdit *textEditDiagnosis = new QTextEdit();
    QPushButton *clearButton = new QPushButton("Очистить");

    QAction *actionSave = new QAction("Сохранить", &window);
    QAction *actionSaveAs = new QAction("Сохранить как", &window);
    QAction *actionDelete = new QAction("Удалить заключение", &window);
    QAction *actionOpen = new QAction("Открыть", &window);

    QAction *actionHelp = new QAction("Справка", &window);
    QAction *actionExit = new QAction("Выход", &window);

    QMenuBar *menuBar = new QMenuBar(&window);
    QMenu *menuFile = new QMenu("Файл");
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionDelete);
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuBar->addMenu(menuFile);

    QMenu *menuHelp = new QMenu("Справка");
    menuHelp->addAction(actionHelp);
    menuBar->addMenu(menuHelp);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(menuBar);
    layout->addWidget(labelDiagnosis);
    layout->addWidget(textEditDiagnosis);
    layout->addWidget(clearButton);
    window.setLayout(layout);



    QString lastSavePath = "";

    QObject::connect(actionSaveAs, &QAction::triggered, [&]() {
        QString diagnosis = textEditDiagnosis->toPlainText();
        QString savePath = QFileDialog::getSaveFileName(&window, "Сохранить как", QDir::homePath(), "Текстовые файлы (*.txt)");
        if (!savePath.isEmpty()) {
            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream << "Заключение офтальмолога:\n";
                stream << "Диагноз: " << diagnosis << "\n\n";
                file.close();
                QMessageBox::information(&window, "Успех", "Заключение сохранено в файл '" + savePath + "'.");
            } else {
                QMessageBox::critical(&window, "Ошибка", "Не удалось сохранить заключение.");
            }
        }
    });

    QObject::connect(actionSave, &QAction::triggered, [&]() {
        QString diagnosis = textEditDiagnosis->toPlainText();

        if (!lastSavePath.isEmpty()) {
            QFile file(lastSavePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
                QTextStream stream(&file);
                stream << "Заключение офтальмолога:\n";
                stream << "Диагноз: " << diagnosis << "\n\n";
                file.close();
                QMessageBox::information(&window, "Успех", "Заключение сохранено в файл '" + lastSavePath + "'.");
            } else {
                QMessageBox::critical(&window, "Ошибка", "Не удалось сохранить заключение.");
            }
        } else {
            QString openPath = QFileDialog::getOpenFileName(&window, "Открыть файл", QDir::homePath(), "Текстовые файлы (*.txt)");
            if (!openPath.isEmpty()) {
                QFile file(openPath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream stream(&file);
                    stream << "Заключение офтальмолога:\n";
                    stream << "Диагноз: " << diagnosis << "\n\n";
                    file.close();
                    lastSavePath = openPath;
                    QMessageBox::information(&window, "Успех", "Заключение сохранено в файл '" + lastSavePath + "'.");
                } else {
                    QMessageBox::critical(&window, "Ошибка", "Не удалось сохранить заключение.");
                }
            }
        }
    });


 QObject::connect(actionDelete, &QAction::triggered, [&]() {
        QString deletePath = QFileDialog::getOpenFileName(&window, "Удалить заключение", QDir::homePath(), "Текстовые файлы (*.txt)");
        if (!deletePath.isEmpty()) {
            QFile file(deletePath);
            if (file.exists()) {
                if (file.remove()) {  QMessageBox::information(&window, "Успех", "Заключение успешно удалено.");
                } else {
                    QMessageBox::critical(&window, "Ошибка", "Не удалось удалить заключение.");
                }
            } else {
                QMessageBox::critical(&window, "Ошибка", "Выбранный файл не найден.");
            }
        }
    });


    // Обработчик события для "Открыть заключение"
    QObject::connect(actionOpen, &QAction::triggered, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Открыть файл", QDir::homePath(), "Текстовые файлы (*.txt)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.exists()) {
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QString contents = file.readAll();
                    file.close();
                    // Создание текстового поля для отображения заключений
                    QPlainTextEdit *plainTextEdit = new QPlainTextEdit();
                    plainTextEdit->setReadOnly(true);
                    plainTextEdit->setPlainText(contents);
                    // Создание нового окна для отображения заключений
                    QDialog dialog(&window);
                    dialog.setWindowTitle("Открыть заключение");
                    // Размещение текстового поля на окне
                    QVBoxLayout *layout = new QVBoxLayout(&dialog);
                    layout->addWidget(plainTextEdit);
                    // Отображение окна
                    dialog.exec();
                } else {
                    QMessageBox::critical(&window, "Ошибка", "Не удалось открыть заключение.");
                }
            } else {
                QMessageBox::critical(&window, "Ошибка", "Заключение не найдено.");
            }
        }
    });


    QObject::connect(clearButton, &QPushButton::clicked, [&]() {
        textEditDiagnosis->clear();
    });


    QObject::connect(actionHelp, &QAction::triggered, [&]() {
        QMessageBox::information(&window, "Справка", "Данная программа предназначена для ведения заключений офтальмолога.\n\n"
                                                     "Для сохранения заключения используйте кнопку 'Сохранить' в меню 'Файл'.\n\n"
                                                     "Для удаления заключения используйте кнопку 'Удалить заключение' в меню 'Файл'.\n\n"
                                                     "Для очистки окно программы используйте кнопку 'Очистить'.\n\n"
                                                     "Для открытия ранее сохраненного заключения используйте кнопку 'Открыть заключение' "
                                                     "в меню 'Файл'.\n\n"
                                                     "Для получения справки об использовании программы, нажмите кнопку 'Справка' в меню 'Справка'.\n\n"
                                                     "Для выхода из программы нажмите кнопку 'Выход' в меню 'Файл'.");
    });



    QObject::connect(actionExit, &QAction::triggered, [&]() {
        QApplication::quit();
    });


    window.show();

    return app.exec();
}
