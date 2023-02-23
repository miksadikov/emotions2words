#include "comportdialog.h"

#include <QSerialPortInfo>

#include "ui_comportdialog.h"

ComPortDialog::ComPortDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::ComPortDialog) {
  ui->setupUi(this);
}

ComPortDialog::~ComPortDialog() { delete ui; }

void ComPortDialog::Init() {
  ui->comboBoxComBaudrate->setCurrentText("9600");
  ui->comboBoxComBaudrate->setDisabled(true);
  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo& info : infos)
    ui->comboBoxComName->addItem(info.portName());
}

QString ComPortDialog::getPortName() {
  return (ui->comboBoxComName->currentText());
}

void ComPortDialog::on_pushButtonOk_clicked() { done(QDialog::Accepted); }
void ComPortDialog::on_pushButtonCancel_clicked() { done(QDialog::Rejected); }
