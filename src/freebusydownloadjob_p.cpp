/*
  Copyright (C) 2010 Bertjan Broeksema <broeksema@kde.org>
  Copyright (C) 2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "freebusydownloadjob_p.h"

#include <KIO/Job>
#include <KIO/JobUiDelegate>
#include <KIO/TransferJob>
#include <KJobWidgets/KJobWidgets>

using namespace Akonadi;

FreeBusyDownloadJob::FreeBusyDownloadJob(const QUrl &url, QWidget *parentWidget)
    : mUrl(url), mParent(parentWidget)
{
    setObjectName(QStringLiteral("FreeBusyDownloadJob"));
}

FreeBusyDownloadJob::~FreeBusyDownloadJob()
{
}

void FreeBusyDownloadJob::start()
{
    KIO::TransferJob *job = KIO::get(mUrl, KIO::NoReload, KIO::HideProgressInfo);
    KJobWidgets::setWindow(job, mParent);
    connect(job, &KIO::TransferJob::result, this, &FreeBusyDownloadJob::slotResult);
    connect(job, &KIO::TransferJob::data, this, &FreeBusyDownloadJob::slotData);
}

QByteArray FreeBusyDownloadJob::rawFreeBusyData() const
{
    return mFreeBusyData;
}

QUrl FreeBusyDownloadJob::url() const
{
    return mUrl;
}

void FreeBusyDownloadJob::slotData(KIO::Job *, const QByteArray &data)
{
    mFreeBusyData += data;
}

void FreeBusyDownloadJob::slotResult(KJob *job)
{
    if (job->error()) {
        setErrorText(job->errorText());
    }

    emitResult();
}
