#ifndef MOCK_TEXTPROGRESSBAR_H
#define MOCK_TEXTPROGRESSBAR_H

#include <gmock/gmock.h>

#include "../../src/TextProgressBar.h"


class MockTextProgressBar : public TextProgressBar
{
public:
    MOCK_METHOD0(clear, void());
    MOCK_METHOD0(update, void());
    MOCK_METHOD1(setMessage, void(const QString &message));
    MOCK_METHOD2(setStatus, void(qint64 value, qint64 maximum));

    void parentUpdate() {
        return TextProgressBar::update();
    }

    void parentSetMessage(const QString &m) {
        return TextProgressBar::setMessage(m);
    }

    void parentClear() {
        return TextProgressBar::clear();
    }

    void parentSetStatus(qint64 val, qint64 max) {
        return TextProgressBar::setStatus(val, max);
    }
};

#endif // MOCK_TEXTPROGRESSBAR_H
