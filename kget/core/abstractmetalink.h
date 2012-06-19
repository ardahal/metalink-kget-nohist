/* This file is part of the KDE project

   Copyright (C) 2012 by Aish Raj Dahal <dahalaishraj@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef ABSTRACTMETALINK_H
#define ABSTRACTMETALINK_H


#include <KIO/Job>

#include "core/datasourcefactory.h"
#include "core/transfer.h"

#include "ui/metalinkcreator/metalinker.h"

class KGET_EXPORT AbstractMetalink : public Transfer
{
    Q_OBJECT

    public:
        AbstractMetalink(TransferGroup * parent, TransferFactory * factory,
                         Scheduler * scheduler, const KUrl & src, const KUrl & dest,
                         const QDomElement * e = 0);
        virtual ~AbstractMetalink();

        virtual void save(const QDomElement &element) = 0;
        virtual void load(const QDomElement *element) = 0;

        int remainingTime() const;

        bool repair(const KUrl &file = KUrl());

        /**
         * Move the download to the new destination
         * @param newDirectory is a directory where the download should be stored
         * @returns true if newDestination can be used
         */
        virtual bool setDirectory(const KUrl &newDirectory);

        QHash<KUrl, QPair<bool, int> > availableMirrors(const KUrl &file) const;
        void setAvailableMirrors(const KUrl &file, const QHash<KUrl, QPair<bool, int> > &mirrors);

        /**
         * @param file for which to get the verifier
         * @return Verifier that allows you to add checksums manually verify a file etc.
         */
        virtual Verifier *verifier(const KUrl &file);

        /**
         * @param file for which to get the signature
         * @return Signature that allows you to add signatures and verify them
         */
        virtual Signature *signature(const KUrl &file);

        virtual QList<KUrl> files() const;

        FileModel *fileModel();

    public Q_SLOTS:
        // --- Job virtual functions ---
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void deinit(Transfer::DeleteOptions options) = 0;

    protected Q_SLOTS:
        /**
         * @return true if initialising worked
         * @note false does not mean that an error happened, it could mean, that the user
         * decided to update the metalink
         */

        void slotUpdateCapabilities();
        void slotDataSourceFactoryChange(Transfer::ChangesFlags change);
        void slotRename(const KUrl &oldUrl, const KUrl &newUrl);
        void slotVerified(bool isVerified);
        void slotSignatureVerified();

    protected :
        virtual void startMetalink() = 0;
        void recalculateTotalSize(DataSourceFactory *sender);
        void recalculateProcessedSize();
        void recalculateSpeed();
        void updateStatus(DataSourceFactory *sender, bool *changeStatus);

    protected:
        FileModel *m_fileModel;
        int m_currentFiles;
        QHash<KUrl, DataSourceFactory*> m_dataSourceFactory;
        bool m_ready;
        int m_speedCount;
        int m_tempAverageSpeed;
        mutable int m_averageSpeed;
};

#endif
