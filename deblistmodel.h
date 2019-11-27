/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEBLISTMODEL_H
#define DEBLISTMODEL_H

#include <QAbstractListModel>
#include <QFuture>
#include <QPointer>

#include <QApt/Backend>
#include <QApt/DebFile>
#include <QApt/Transaction>

class PackagesManager;
class DebListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DebListModel(QObject *parent = nullptr);

    enum PackageRole {
        PackageNameRole = Qt::DisplayRole,
        UnusedRole = Qt::UserRole,
        WorkerIsPrepareRole,
        ItemIsCurrentRole,
        PackageVersionRole,
        PackagePathRole,
        PackageInstalledVersionRole,
        PackageDescriptionRole,
        PackageVersionStatusRole,
        PackageDependsStatusRole,
        PackageAvailableDependsListRole,
        PackageFailReasonRole,
        PackageOperateStatusRole,
        PackageReverseDependsListRole,
    };

    enum WorkerStatus {
        WorkerPrepare,
        WorkerProcessing,
        WorkerFinished,
    };

    enum WorkerType {
        Install,
        UnInstall
    };

    enum PackageInstallStatus {
        NotInstalled,
        InstalledSameVersion,
        InstalledEarlierVersion,
        InstalledLaterVersion,
    };

    enum PackageDependsStatus {
        DependsOk,  //依赖满足
        DependsAvailable,
        DependsBreak,  //依赖不满足
    };

    enum PackageOperationStatus {
        Prepare,
        Operating,
        Success,
        Failed,
        Waiting,
    };

    void reset();
    bool isReady() const;
    bool isInstallWorkerPrepare() const;
    bool isUninstallWorkerPrepare() const;

    const QList<QApt::DebFile *> preparedPackages() const;
    QModelIndex first() const;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    int getInstallWorkStatus() const;
    int getUninstallWorkStatus() const;
    bool isInstalling() const;

    WorkerType getWorkerType();

signals:
    void lockForAuth(const bool lock) const;
    void workerFinished() const;
    void workerProgressChanged(const int progress) const;
    void transactionProgressChanged(const int progress) const;
    void appendOutputInfo(const QString &info) const;
    void packageOperationChanged(const QModelIndex &index, int status) const;
    void packageDependsChanged(const QModelIndex &index, int status) const;

    void onChangeOperateIndex(int opIndex);
    void AuthCancel();

public slots:
    void setCurrentIndex(const QModelIndex &idx);
    void installAll();
    void uninstallPackage(const int idx);
    void removePackage(const int idx);
    bool appendPackage(QApt::DebFile *package);
    void onTransactionErrorOccurred();
    void onTransactionStatusChanged(QApt::TransactionStatus stat);

private slots:
    void upWrongStatusRow();
    void onUninstallTransactionFinished();
    void onInstallTransactionFinished();
    void onDependsInstallTransactionFinished();
public:
    int getInstallFileSize();

private:
    void bumpInstallIndex();
    void onTransactionOutput();
    void installNextDeb();
    void refreshOperatingPackageStatus(const PackageOperationStatus stat);
    QString packageFailedReason(const int idx) const;
    void initRowStatus();

private:
    int m_installWorkerStatus;
    int m_uninstallWorkerStatus;
    int m_operatingIndex;
    QModelIndex m_currentIdx;
    PackagesManager *m_packagesManager;

    QPointer<QApt::Transaction> m_currentTransaction;

    QMap<int, int> m_packageOperateStatus;
    QMap<int, int> m_packageFailReason;
    bool m_InitRowStatus;

    WorkerType m_workerType;
};

#endif  // DEBLISTMODEL_H
