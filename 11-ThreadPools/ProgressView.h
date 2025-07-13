#pragma once

#include "Progress.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>

#include <memory>

class Progress;

/**
 * A widget to display the progress of a task.
 * It shows a label, a progress bar, and a cancel button.
 */
class ProgressView : public QWidget {

    Q_OBJECT

    QLabel *progressLabel;
    QProgressBar *progressBar;
    QPushButton *cancelButton;
    QGridLayout *layout;

    std::shared_ptr<Progress> progress;

public:

    /**
     * Constructs a ProgressView with the given progress object.
     * @param progress A shared pointer to the Progress object to monitor.
     * @param parent The parent widget.
     */
    explicit ProgressView(std::shared_ptr<Progress> progress, QWidget *parent = nullptr);

    std::shared_ptr<Progress> getProgress() const { return progress; }

    /// @brief Sets the label text format for the progress bar.
    void setLabelFormat(const QString &format) { progressBar->setFormat(format); }

    /// @brief Sets the range for the progress bar.
    void setProgressRange(int min, int max) { progressBar->setRange(min, max); }
signals:

    /// @brief Signal emitted when the task is done (i.e. either completed or cancelled).
    void taskDone();

private slots:

    void onProgressChanged(double value);
    void onStatusChanged(ProgressStatus status);
    void onCancelButtonClicked();
};
