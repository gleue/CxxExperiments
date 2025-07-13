#include "ProgressView.h"

ProgressView::ProgressView(std::shared_ptr<Progress> progressPtr, QWidget* parent)
  : QWidget(parent),
  progressLabel(new QLabel(this)),
  progressBar(new QProgressBar(this)),
  cancelButton(new QPushButton(tr("Cancel"), this)),
  gridLayout(new QGridLayout(this)),
  progress(std::move(progressPtr)) {

  progressBar->setTextVisible(false);
  cancelButton->setEnabled(false);

  gridLayout->addWidget(progressLabel, 0, 0, 1, 2);
  gridLayout->addWidget(progressBar, 1, 0, 1, 1);
  gridLayout->addWidget(cancelButton, 1, 1, 1, 1);
  setLayout(gridLayout);

  connect(progress.get(), &Progress::progressChanged, this, &ProgressView::onProgressChanged);
  connect(progress.get(), &Progress::statusChanged, this, &ProgressView::onStatusChanged);
  connect(cancelButton, &QPushButton::clicked, this, &ProgressView::onCancelButtonClicked);
}

void ProgressView::onProgressChanged(double value) {
  if (progress->isInProgress()) {
    progressBar->setValue(progressBar->minimum() + static_cast<int>(value * (progressBar->maximum() - progressBar->minimum())));
    progressLabel->setText(progressBar->text());
  }
}

void ProgressView::onStatusChanged(ProgressStatus status) {
  switch (status) {
  case ProgressStatus::NotStarted:
    progressLabel->setText(tr("Press 'Start' to begin execution"));
    cancelButton->setText(tr("Start"));
    cancelButton->setEnabled(true);
    break;
  case ProgressStatus::InProgress:
    progressLabel->setText(tr("Execution started..."));
    cancelButton->setText(tr("Cancel"));
    cancelButton->setEnabled(true);
    break;
  case ProgressStatus::Completed:
    progressLabel->setText(tr("Execution complete"));
    cancelButton->setEnabled(false);
    emit taskDone();
    break;
  case ProgressStatus::Cancelled:
    progressLabel->setText(tr("Task cancelled"));
    cancelButton->setEnabled(false);
    emit taskDone();
    break;
  }
}

void ProgressView::onCancelButtonClicked() {
  progress->cancel();
}
