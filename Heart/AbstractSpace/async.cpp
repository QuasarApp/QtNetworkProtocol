#include "async.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QThread>
#include <QDebug>

// Private implementation of waitFor functions.
#define waitPrivate(CONDITION, TIMEOUT) \
    auto curmsec = QDateTime::currentMSecsSinceEpoch() + TIMEOUT; \
    while (curmsec > QDateTime::currentMSecsSinceEpoch() && !CONDITION) { \
        QCoreApplication::processEvents(); \
    } \
    QCoreApplication::processEvents(); \
    return CONDITION; \


namespace QH {
Async::Async(QObject *ptr):
    QObject(ptr) {

}

void Async::asyncHandler(Job job,
                          bool *resultOfWork,
                          bool *endOfWork) const {

    bool result = job();

    /*This IS ULTRA BUG
     * For reproduse:
     * 1. run gdb
     * 2. skip the 3 step of the asyncLauncher method.
     * 3. add breackpoint in to this function.
     * 4. operator = for the resultOfWork operand broken the stack of main thread.
     * This is true magick.
     * -\_O_/-
     * */
    qDebug() << resultOfWork << " : " << *resultOfWork;

    if (resultOfWork)
        *resultOfWork = result;

    if (endOfWork) {
        *endOfWork = true;
    }


}

bool Async::waitFor(bool *condition, int timeout) const {
    if (!condition)
        return false;

    waitPrivate(*condition, timeout)
}

bool Async::asyncLauncher(const Async::Job &job, bool await) {
    if (QThread::currentThread() == thread()) {
        return job();
    }

    bool workOfEnd = false, workResult = false;

    bool invockeResult = QMetaObject::invokeMethod(this,
                                                   "asyncHandler",
                                                   Qt::QueuedConnection,
                                                   Q_ARG(QH::Async::Job, job),
                                                   Q_ARG(bool *, &workResult),
                                                   Q_ARG(bool *, &workOfEnd));


    if (!invockeResult)
        return false;

    if (!await) {
        return true;
    }

    if (!waitFor(&workOfEnd)) {
        return false;
    }

    return workResult;
}

bool Async::waitFor(const std::function<bool ()> &condition, int timeout) const {
    waitPrivate(condition(), timeout)
}

}
