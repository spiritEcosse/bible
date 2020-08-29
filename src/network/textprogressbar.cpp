#include "textprogressbar.h"

#include <QByteArray>

#include <cstdio>

using namespace std;

void TextProgressBar::clear()
{
    printf("\n");
    fflush(stdout);

    value = 0;
    maximum = -1;
    iteration = 0;
}

void TextProgressBar::update() // FIXME: test all method
{
    ++iteration;

    if (maximum > 0) {
        // we know the maximum
        // draw a progress bar
        int percent = value * 100 / maximum;
        int hashes = percent / 2;

        QByteArray progressbar(hashes, '#');
        if (percent % 2)
            progressbar += '>';

//        printf("\r[%-50s] %3d%% %s     ", // WARNING: pass printf to ui
//               progressbar.constData(),
//               percent,
//               qPrintable(message));
    } else {
        // we don't know the maximum, so we can't draw a progress bar
        int center = (iteration % 48) + 1; // 50 spaces, minus 2
        QByteArray before(qMax(center - 2, 0), ' ');
        QByteArray after(qMin(center + 2, 50), ' ');

//        printf("\r[%s###%s]      %s      ", // WARNING: pass printf to ui
//               before.constData(), after.constData(), qPrintable(message));
    }
}

void TextProgressBar::setMessage(const QString &m)
{
    message = m;
}

void TextProgressBar::setStatus(qint64 val, qint64 max)
{
    value = val;
    maximum = max;
}
