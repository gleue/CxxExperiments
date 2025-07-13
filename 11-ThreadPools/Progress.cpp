#include "Progress.h"

#include <QMetaObject>

void Progress::setTotal(int totalItems) {
  {
    std::scoped_lock lock(mutex);

    if (!isIdle()) return;
    total = totalItems;
    current = 0;
  }

  QMetaObject::invokeMethod(this, [this]() { emit progressChanged(getProgress()); }, Qt::QueuedConnection);
}

void Progress::setCurrent(int currentItems) {
  {
    std::scoped_lock lock(mutex);

    if (isIdle()) return;
    current = std::clamp(currentItems, 0, total.load());
  }

  QMetaObject::invokeMethod(this, [this]() { emit progressChanged(getProgress()); }, Qt::QueuedConnection);
}

bool Progress::updateStatus(ProgressStatus expected, ProgressStatus newStatus) {
  if (!status.compare_exchange_strong(expected, newStatus)) return false;

  QMetaObject::invokeMethod(this, [this, newStatus]() { emit statusChanged(newStatus); }, Qt::QueuedConnection);
  return true;
}
