void pressAKeyEvent()
{
    QKeyEvent* pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier);
    QCoreApplication::postEvent(QCoreApplication::instance(), pressEvent);

    QTimer::singleShot(2000, [](){
        QKeyEvent* releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier);
        QCoreApplication::postEvent(QCoreApplication::instance(), releaseEvent);
        pressAKeyEvent();
    });
}
